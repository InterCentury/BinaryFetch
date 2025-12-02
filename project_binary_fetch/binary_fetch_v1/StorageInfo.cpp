/*
===============================================================
  Project: BinaryFetch — System Information & Hardware Insights Tool
  Author:  Maruf Hasan  |  Founder of BinaryOxide
  Language: C++17 (Windows API)
  File: StorageInfo.cpp
  --------------------------------------------------------------
  Overview:
    Handles all disk-related functionalities — identification,
    total/used/free space, read/write speeds, and predicted speeds.
===============================================================
*/

#include "StorageInfo.h"
#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <malloc.h>   // _aligned_malloc / _aligned_free
#include <cstdlib>

using namespace std;

// ============================================================
//  Helper: get_sector_size()
//  Returns the native bytes-per-sector for the volume.
//  Falls back to 4096 if query fails.
// ============================================================
static DWORD get_sector_size(const string& root_path)
{
    DWORD sectorsPerCluster = 0;
    DWORD bytesPerSector = 0;
    DWORD numberOfFreeClusters = 0;
    DWORD totalNumberOfClusters = 0;

    // GetDiskFreeSpaceA wants a root like "C:\\"
    if (GetDiskFreeSpaceA(root_path.c_str(), &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters)) {
        if (bytesPerSector == 0) return 4096;
        return bytesPerSector;
    }
    return 4096;
}

// ============================================================
//  Helper: round_up_to()
// ============================================================
static size_t round_up_to(size_t value, size_t align)
{
    return ((value + align - 1) / align) * align;
}

// ============================================================
//  Function: create_test_file_winapi()
//  Purpose : Ensures a temporary test file of exact size exists.
//            Uses normal buffered write with WRITE_THROUGH so the
//            file is flushed to media and visible for NO_BUFFERING reads.
// ============================================================
static bool create_test_file_winapi(const string& path, size_t requested_size, size_t sector_size, size_t buffer_size = 4 * 1024 * 1024)
{
    const string fname = path + "speed_test.tmp";

    // Align sizes to sector_size
    size_t aligned_file_size = round_up_to(requested_size, sector_size);
    size_t desired_buffer = round_up_to(buffer_size, sector_size);

    HANDLE h = CreateFileA(
        fname.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_WRITE_THROUGH,
        nullptr
    );

    if (h == INVALID_HANDLE_VALUE) return false;

    // Use a simple (possibly unaligned) buffer for creating the file; write-through
    vector<char> buf(desired_buffer, 'x');
    size_t remaining = aligned_file_size;
    DWORD written = 0;

    while (remaining > 0) {
        DWORD toWrite = (DWORD)min<size_t>(buf.size(), remaining);
        if (!WriteFile(h, buf.data(), toWrite, &written, nullptr) || written == 0) {
            CloseHandle(h);
            DeleteFileA(fname.c_str());
            return false;
        }
        remaining -= written;
    }

    // Ensure data on physical media
    FlushFileBuffers(h);
    CloseHandle(h);
    return true;
}

// ============================================================
//  Function: measure_write_speed()
//  - Writes using FILE_FLAG_NO_BUFFERING + WRITE_THROUGH.
//  - Uses sector-aligned buffers and sizes.
//  - Returns MiB/s (MB/s) measured against actual bytes written.
// ============================================================
double measure_write_speed(const string& path)
{
    const string test_file = path + "speed_test.tmp";
    const size_t requested_file_size = 50ull * 1024 * 1024; // 50 MiB base
    // get sector size for alignment
    size_t sector_size = (size_t)get_sector_size(path);

    // choose a buffer size that's a multiple of sector_size (prefer 4MiB)
    size_t desired_buf = 4ull * 1024 * 1024;
    size_t buffer_size = round_up_to(max(desired_buf, sector_size), sector_size);
    size_t file_size = round_up_to(requested_file_size, sector_size);

    // Create aligned buffer
    void* aligned_buf = _aligned_malloc(buffer_size, sector_size);
    if (!aligned_buf) {
        return 0.0;
    }
    memset(aligned_buf, 'x', buffer_size);

    HANDLE h = CreateFileA(
        test_file.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,
        nullptr
    );

    if (h == INVALID_HANDLE_VALUE) {
        _aligned_free(aligned_buf);
        DeleteFileA(test_file.c_str());
        return 0.0;
    }

    size_t remaining = file_size;
    DWORD bytesWritten = 0;

    auto start = chrono::high_resolution_clock::now();

    while (remaining > 0) {
        DWORD toWrite = (DWORD)min<size_t>(buffer_size, remaining);
        // WriteFile with NO_BUFFERING requires aligned buffer and toWrite to be multiple of sector_size
        if (!WriteFile(h, aligned_buf, toWrite, &bytesWritten, nullptr)) {
            CloseHandle(h);
            _aligned_free(aligned_buf);
            DeleteFileA(test_file.c_str());
            return 0.0;
        }
        if (bytesWritten == 0) break;
        remaining -= bytesWritten;
    }

    // Ensure write-through to device
    FlushFileBuffers(h);
    CloseHandle(h);

    auto end = chrono::high_resolution_clock::now();
    double seconds = chrono::duration<double>(end - start).count();

    // Clean up
    _aligned_free(aligned_buf);

    // Compute MiB from file_size (which is the actual number of bytes written)
    double mib = (double)file_size / (1024.0 * 1024.0);
    DeleteFileA(test_file.c_str());

    if (seconds <= 0.0) return 0.0;
    return mib / seconds;
}

// ============================================================
//  Function: measure_read_speed()
//  - Creates a flushed test file (aligned size), then reads it with
//    FILE_FLAG_NO_BUFFERING to avoid OS cache.
//  - Uses aligned buffers and sizes.
//  - Returns MiB/s measured from actual bytes read.
// ============================================================
double measure_read_speed(const string& path)
{
    const string test_file = path + "speed_test.tmp";
    const size_t requested_file_size = 50ull * 1024 * 1024; // 50 MiB base

    // get sector size
    size_t sector_size = (size_t)get_sector_size(path);

    // Align sizes
    size_t desired_buf = 4ull * 1024 * 1024;
    size_t buffer_size = round_up_to(max(desired_buf, sector_size), sector_size);
    size_t file_size = round_up_to(requested_file_size, sector_size);

    // Create the test file (with write-through) so data is on disk
    if (!create_test_file_winapi(path, file_size, sector_size, buffer_size)) {
        DeleteFileA(test_file.c_str());
        return 0.0;
    }

    // Allocate aligned read buffer
    void* aligned_buf = _aligned_malloc(buffer_size, sector_size);
    if (!aligned_buf) {
        DeleteFileA(test_file.c_str());
        return 0.0;
    }

    HANDLE h = CreateFileA(
        test_file.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_NO_BUFFERING,
        nullptr
    );

    if (h == INVALID_HANDLE_VALUE) {
        _aligned_free(aligned_buf);
        DeleteFileA(test_file.c_str());
        return 0.0;
    }

    size_t totalRead = 0;
    DWORD bytesRead = 0;

    auto start = chrono::high_resolution_clock::now();

    while (true) {
        if (!ReadFile(h, aligned_buf, (DWORD)buffer_size, &bytesRead, nullptr)) {
            // On some devices ReadFile may return FALSE when hitting exact EOF with NO_BUFFERING;
            // treat as break if bytesRead == 0.
            if (GetLastError() == ERROR_HANDLE_EOF || bytesRead == 0) break;
            CloseHandle(h);
            _aligned_free(aligned_buf);
            DeleteFileA(test_file.c_str());
            return 0.0;
        }
        if (bytesRead == 0) break;
        totalRead += bytesRead;
        if (totalRead >= file_size) break;
    }

    auto end = chrono::high_resolution_clock::now();
    CloseHandle(h);
    _aligned_free(aligned_buf);
    DeleteFileA(test_file.c_str());

    double seconds = chrono::duration<double>(end - start).count();
    if (seconds <= 0.0 || totalRead == 0) return 0.0;

    double mib = (double)totalRead / (1024.0 * 1024.0);
    return mib / seconds;
}

// ============================================================
//  Function: StorageInfo::get_all_storage_info()
//  Purpose : Returns disks with base, accurate, and predicted info
// ============================================================
vector<storage_data> StorageInfo::get_all_storage_info()
{
    vector<storage_data> all_disks;
    DWORD drive_mask = GetLogicalDrives();
    if (drive_mask == 0) return all_disks;

    char drive_letter = 'A';
    int disk_index = 0;

    while (drive_mask) {
        if (drive_mask & 1) {
            string root_path = string(1, drive_letter) + ":\\";
            ULARGE_INTEGER free_bytes, total_bytes, free_bytes_available;

            if (GetDiskFreeSpaceExA(root_path.c_str(), &free_bytes_available, &total_bytes, &free_bytes))
            {
                double total_gib = total_bytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
                double free_gib = free_bytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
                double used_gib = total_gib - free_gib;
                double used_percent = (used_gib / total_gib) * 100.0;

                char fs_name[MAX_PATH] = { 0 };
                GetVolumeInformationA(root_path.c_str(), nullptr, 0, nullptr, nullptr, nullptr, fs_name, sizeof(fs_name));
                UINT drive_type = GetDriveTypeA(root_path.c_str());
                bool is_external = (drive_type == DRIVE_REMOVABLE);

                ostringstream used_str, total_str, percent_str;
                used_str << fixed << setprecision(2) << used_gib << " GiB";
                total_str << fixed << setprecision(2) << total_gib << " GiB";
                percent_str << "(" << (int)used_percent << "%)";

                storage_data disk;
                disk.drive_letter = "Disk (" + string(1, drive_letter) + ":)";
                disk.used_space = used_str.str();
                disk.total_space = total_str.str();
                disk.used_percentage = percent_str.str();
                disk.file_system = fs_name;
                disk.is_external = is_external;
                disk.serial_number = "SN-" + to_string(1000 + disk_index);

                // Accurate speeds: measure (bypass cache) and format to 2 decimals
                double r = measure_read_speed(root_path);
                double w = measure_write_speed(root_path);

                {
                    ostringstream ss;
                    ss << fixed << setprecision(2) << r << " MB/s";
                    disk.read_speed = ss.str();
                }
                {
                    ostringstream ss;
                    ss << fixed << setprecision(2) << w << " MB/s";
                    disk.write_speed = ss.str();
                }

                // Enhanced Predicted speeds (same heuristics as before)
                if (is_external) {
                    disk.predicted_read_speed = "100 MB/s";
                    disk.predicted_write_speed = "80 MB/s";
                }
                else {
                    if (total_gib >= 500) { // likely HDD
                        disk.predicted_read_speed = "140 MB/s";
                        disk.predicted_write_speed = "120 MB/s";
                    }
                    else { // likely SSD
                        disk.predicted_read_speed = "450 MB/s";
                        disk.predicted_write_speed = "400 MB/s";
                    }
                }

                all_disks.push_back(disk);
                disk_index++;
            }
        }
        drive_letter++;
        drive_mask >>= 1;
    }

    return all_disks;
}

/*
===============================================================
  End of File
===============================================================
*/

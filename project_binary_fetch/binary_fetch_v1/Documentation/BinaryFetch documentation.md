## 1️⃣ Operating System Info (OSInfo.h)

```cpp
✅ string GetOSVersion(); --> returns full Windows version  
✅ string GetOSArchitecture(); --> returns OS bit type (32/64-bit)  
✅ string GetOSName(); --> returns Windows edition name  
✅ string get_os_uptime(); --> shows OS uptime since reboot  
✅ string get_os_install_date(); --> returns installation date  
✅ string get_os_serial_number(); --> returns OS serial number  
✅ string get_os_kernel_info(); --> returns OS Kernel info  
```

## 2️⃣ CPU / Processor Info (CPUInfo.h)

```cpp
✅ string get_cpu_info(); --> returns CPU brand and model  
✅ string get_cpu_utilization(); --> returns CPU usage percent  
✅ string get_cpu_speed(); --> returns current CPU speed  
✅ string get_cpu_base_speed(); --> returns base CPU frequency  
✅ string get_cpu_sockets(); --> returns number of sockets  
✅ string get_cpu_cores(); --> returns number of cores  
✅ string get_cpu_logical_processors(); --> returns number of threads  
✅ string get_cpu_virtualization(); --> shows virtualization status  
✅ string get_cpu_l1_cache(); --> returns L1 cache size  
✅ string get_cpu_l2_cache(); --> returns L2 cache size  
✅ string get_cpu_l3_cache(); --> returns L3 cache size  
✅ string get_system_uptime(); --> shows system uptime  
✅ string get_process_count(); --> returns process count  
✅ string get_thread_count(); --> returns thread count  
✅ string get_handle_count(); --> returns handle count  
```

## 3️⃣ RAM / Memory Info (RAMInfo.h)


```cpp
✅ string get_total_memory(); --> returns total RAM in GB  
✅ string get_free_memory(); --> returns free RAM in GB  
✅ string used_ram(); --> returns used memory percentage  
✅ string get_formatted_memory_info(); --> returns detailed RAM info  
```
## 4️⃣ GPU / Graphics Card Info (GPUInfo.h)

```cpp
<---------------- Basic GPU Info Functions (GPUInfo.h) ----------------->
✅string get_gpu_name;              // returns GPU name
✅string get_gpu_vendor;               // returns GPU vendor
✅string get_gpu_memory_total;      // returns total VRAM
✅string get_gpu_driver_version;    // returns driver version
✅float get_gpu_usage;                 // returns GPU usage percent
  float get_gpu_temperature;           // returns GPU temperature
✅int get_gpu_core_count;              // returns GPU core count

// --- Multi-GPU / Extra Features 
<----------------(DetailedGPUInfo.h / DetailedGPUInfo.cpp) ------------->
✅ vector<GPUData> get_all_gpus();     // returns info of all GPUs in the system
✅ GPUData primary_gpu_info();         // returns info of the primary GPU (GPU 0)

//here's the implementation inside main.cpp ------------------------------

        auto primary = detailed_gpu_info.primary_gpu_info();
        cout << "GPU 0 : " << primary.name
            << " @ " << primary.frequency_ghz << " GHz ( "
            << primary.vram_gb << " GIB)" << endl;
//(output) -----> NVIDIA GeForce RTX 4070 SUPER @ 3.10 GHz (11.72 GiB)

        auto all = detailed_gpu_info.get_all_gpus();
        cout << "GPU 0 : " << primary.name
            << " @ " << primary.frequency_ghz << " GHz ( "
            << primary.vram_gb << " GIB)";
       cout << endl << endl;
//(output) -----> NVIDIA GeForce RTX 4070 SUPER @ 3.10 GHz (11.72 GiB)
               // NVIDIA GeForce RTX 4078 SUPER @ 3.70 GHz (16.00 GiB)  
               (could show multiple gpu list differently)     

```
## 5️⃣ Storage Info (StorageInfo.h)

```cpp
✅ string get_disk_list; --> returns all disks  
✅ string get_disk_size; --> returns total disk size  
✅ string get_disk_free_space; --> returns free disk space  
✅ string get_disk_type; --> returns disk type (SSD/HDD)  
✅ string get_disk_serial_number; --> returns disk serial number  
✅ string get_disk_read_speed; --> returns read speed  
✅ string get_disk_write_speed; --> returns write speed  
✅ string predicted_read_speed; --> returns predicted disk read speed
✅ string predicted_write_speed; --> returns predicted write speed 
```
## 6️⃣ Network / IP Info (NetworkInfo.h)

```cpp
✅ string get_ip_address(); --> returns local IP  
✅ string get_mac_address(); --> returns MAC address  
✅ string get_network_name(); --> returns network name  (SSID)
✅ string get_network_speed(); --> returns network speed  
✅ string get_locale(); --> returns system locale  
✅ string get_public_ip(); --> returns public IP  
```
## 7️⃣ System Uptime / Performance (PerformanceInfo.h)

```cpp
✅ string get_system_uptime(); --> returns uptime  
✅ float get_cpu_usage_percent(); --> CPU usage percent  
✅ float get_ram_usage_percent(); --> RAM usage percent  
✅ float get_disk_usage_percent(); --> Disk usage percent  
float get_gpu_usage_percent(); --> GPU usage percent  (paused for now)
```
## 8️⃣ User / System Info (UserInfo.h)

```cpp
✅ sstring get_username(); --> returns username  
✅ sstring get_computer_name(); --> returns PC name  
✅ sstring get_domain_name(); --> returns domain name  
✅ sstring get_user_groups(); --> returns user groups  
```
## 9️⃣ BIOS / Motherboard / Environment (SystemInfo.h)

```cpp
✅ string get_bios_vendor(); --> returns BIOS vendor  
✅ string get_bios_version(); --> returns BIOS version  
✅ string get_bios_date(); --> returns BIOS date  
✅ string get_motherboard_model(); --> returns motherboard model  
✅ string get_motherboard_manufacturer(); --> returns manufacturer  
✅ string get_environment_variables(); --> returns environment vars  
```
## 🔟 DisplayInfo (DisplayInfo.h)

```cpp
✅ string get_screen_resolution(); --> returns screen resolution  
✅ float get_screen_refresh_rate(); --> returns refresh rate  
```
## 🔟 Optional Extras (ExtraInfo.h)

```cpp
/ 🎧 Returns a list of available audio devices.
/ Includes both input (microphones) and output (speakers/headphones).
/ Marks which device is currently active.
/ Example:
/   Headphone (High Definition Audio) (active)
/   Speaker (High Definition Audio)
/   Microphone (High Definition Audio) (active)
/   Microphone (DroidCam)
string get_audio_devices();

/ 🔋 Returns the system's current power status.
/ For desktop PCs → "Power Status: Wired connection"
/ For laptops → "Power Status: Battery powered (85%) (Charging)"
/               or "Power Status: Battery powered (62%) (Not Charging)"
string get_power_status();
```




# 🧬 BinaryFetch Compact Mode — Class Structure

### 🧠 Overview

This document defines the modular class structure for **BinaryFetch Compact Mode**, where each system information category is represented by a separate class.  
All modules are lightweight and independent, designed for clean integration and scalability.
═══════════════ BinaryFetch Compact Mode ═══════════════

[OS] -> Windows 11 10.0 (Build 22631) (64-bit) (uptime: 1h 25m)  
[CPU] -> AMD Ryzen 5 5600G with Radeon Graphics (6C/12T) @ 3.89 GHz  
[Display 1] -> Generic PnP Monitor (2194 x 1234) @60Hz  
[Display 2] -> HP M22f FHD Monitor (2194 x 1234) @60Hz  
[Memory] -> (total: 47.79 GB) (free: 28.44 GB) ( 40.00% )  
[Audio Input] -> Microphone (High Definition Audio Device)(Active)  
[Audio Output] -> Headphones (High Definition Audio Device)(Active)  
[BIOS] -> American Megatrends Inc. 2423 (08/10/2021)  
[Motherboard] -> ASUSTeK COMPUTER INC. TUF GAMING A520M-PLUS WIFI  
[GPU] -> NVIDIA GeForce RTX 4070 SUPER (9%) (11.99 GB) (@2535 MHz)  
[Performance] -> (CPU: 33%) (GPU: 9%) (RAM: 40%) (Disk: 97%)  
[User] -> @coffee~ -> (Domain: InterStudio) -> (Type: Admin)  
[network] -> (Name: Maruf Hasan) (Type: WiFi) (ip: 182.168.96.1)


------------------------------------------------

{
  "modules": {
    "ExtraInfo": {
      "enabled": true,
      "audio_devices": true,
      "power_status": true
    },

    "DisplayInfo": {
      "enabled": true,
      "monitors": true,
      "show_name_&_model":true,
      "show_refresh_rate": true,
      "show_resolution": true
    },

    "SystemInfo": {
      "enabled": true,
      "bios": {
        "vendor": true,
        "version": true,
        "date": true
      },
      "motherboard": {
        "model": true,
        "manufacturer": true
      }
    },

    "UserInfo": {
      "enabled": true,
      "username": true,
      "computer_name": true,
      "domain_name": true,
      "user_groups": true
    },

    "PerformanceInfo": {
      "enabled": true,
      "uptime": true,
      "cpu_usage": true,
      "ram_usage": true,
      "disk_usage": true,
      "gpu_usage": true
    },

    "GPUInfo": {
      "enabled": true,
      "mode": "expanded",
      "fields": {
        "gpu_name": true,
        "gpu_memory": true,
        "gpu_usage": true,
        "gpu_vendor": true,
        "gpu_driver_version": true,
        "gpu_temperature": true,
        "gpu_core_count": true
      }
    },

    "MemoryInfo": {
      "enabled": true,
      "total_memory": true,
      "free_memory": true,
      "used_percentage": true
    },

    "OSInfo": {
      "enabled": true,
      "version": true,
      "architecture": true,
      "name": true,
      "kernel_version": true,
      "uptime": true,
      "install_date": true,
      "serial_number": true
    },

    "CPUInfo": {
      "enabled": true,
      "brand": true,
      "utilization": true,
      "speed": true,
      "base_speed": true,
      "sockets": true,
      "cores": true,
      "logical_processors": true,
      "virtualization": true,
      "cache": {
        "l1": true,
        "l2": true,
        "l3": true
      }
    },

    "StorageInfo": {
      "enabled": true,
      "summary": true,
      "details": true,
      "predicted_performance": true
    },

    "NetworkInfo": {
      "enabled": true,
      "local_ip": true,
      "mac_address": true,
      "locale": true,
      "public_ip": true,
      "ssid": true,
      "network_speed": true
    }
  },

  "CompactInfo": {
    "enabled": true,
    "order": [
      "OSInfo",
      "CPUInfo",
      "MemoryInfo",
      "GPUInfo",
      "DisplayInfo",
      "ExtraInfo",
      "SystemInfo",
      "UserInfo",
      "PerformanceInfo"
    ],
    "fields": {
      "OSInfo": ["name", "version", "architecture", "uptime"],
      "CPUInfo": ["brand", "cores", "threads", "speed", "utilization"],
      "MemoryInfo": ["total", "free", "used_percentage"],
      "GPUInfo": ["gpu_name", "gpu_memory", "gpu_usage"],
      "DisplayInfo": ["resolution", "refresh_rate", "monitor_count"],
      "ExtraInfo": ["active_output", "active_input", "power_status"],
      "SystemInfo": ["bios_vendor", "motherboard_model"],
      "UserInfo": ["username", "computer_name", "domain_name", "user_groups"],
      "PerformanceInfo": ["cpu_usage", "ram_usage", "disk_usage", "gpu_usage", "uptime"]
    },
    "prefix_icons": {
      "OSInfo": "🧠",
      "CPUInfo": "⚙️",
      "MemoryInfo": "💾",
      "GPUInfo": "🎮",
      "DisplayInfo": "🖥",
      "ExtraInfo": "🔊",
      "SystemInfo": "🔧",
      "UserInfo": "👤",
      "PerformanceInfo": "📈"
    }
  },

  "preferences": {
    "color_scheme": "default",
    "highlight_color": "yellow",
    "display_mode": "expanded" 
  }
}
### 🧠 **CompactOS**

**File:** `CompactOS.h / CompactOS.cpp`

```cpp
class CompactOS {
public:
 ✅ std::string getOSName();  
 ✅ std::string getOSBuild();
 ✅ std::string getArchitecture();
 ✅ std::string getUptime();
};
```

---

### ⚙️ **CompactCPU**

**File:** `CompactCPU.h / CompactCPU.cpp`

```cpp
class CompactCPU {
public:
✅  std::string getCPUName();
✅  std::string getCores();
✅  std::string getThreads();
✅  double getClockSpeedGHz();
✅  double getUsagePercent();
};
```

---

### 💿 **CompactMemory**

**File:** `CompactRAM.h / CompactRAM.cpp`

```cpp
class CompactMemory {
public:
 ✅   double get_total_memory();
 ✅   double get_free_memory();
 ✅   double get_used_memory_percent();
};
```

---

### 🎮 **CompactGPU**

**File:** `CompactGPU.h / CompactGPU.cpp`

```cpp
class CompactGPU {
public:
 ✅   std::string getGPUName();
 ✅   double getVRAMGB();
 ✅   double getGPUUsagePercent();
 ✅   string gpu_frequncy();
};
```

---

### 🖥 **CompactDisplay**

**File:** `CompactDisplay.h / CompactDisplay.cpp`

```cpp
class CompactDisplay {
public:
    struct ScreenInfo {
✅  std::string brand;   // monitor model / brand (e.g. "HP M22f")
✅  std::string resolution;   // "1920 x 1080"
✅  int refresh_rate;         // e.g. 60
};

    std::vector<DisplayInfo> getDisplays();
};
```

---

### 🔊 **CompactAudio**

**File:** `CompactAudio.h / CompactAudio.cpp`

```cpp
class CompactAudio {
public:
 ✅   std::string getOutputDevice();
 ✅   std::string getInputDevice();
 ✅   bool isOutputActive();
 ✅   bool isInputActive();
};
```

---

### 🔋 **CompactPower**

**File:** `CompactPower.h / CompactPower.cpp`

```cpp
class CompactPower {
public:
    bool isPluggedIn();
    int getBatteryPercent();
    std::string getPowerStatus();
};
```

---

### 🔧 **CompactSystem**

**File:** `CompactSystem.h / CompactSystem.cpp`

```cpp
class CompactSystem {
public:
 ✅   std::string getBIOSInfo();
 ✅   std::string getMotherboardInfo();
};
```

---

### 👤 **CompactUser**

**File:** `CompactUser.h / CompactUser.cpp`

```cpp
class CompactUser {
public:
 ✅ std::string getUsername();
 ✅ std::string getDomain();
 ✅ bool isAdmin();
};
```


### 🎮 **CompactNetwork**

**File:** `CompactNetwork.h / CompactNetwork.cpp`

```cpp
class CompactNetwork {
public:
✅std::string get_network_name();  // Adapter name or WiFi SSID
✅std::string get_network_type();  // "WiFi" or "Ethernet"
✅std::string get_network_ip();    // Local IPv4 address
};
```

---

### 📊 **CompactPerformance**

**File:** `CompactPerformance.h / CompactPerformance.cpp`

```cpp
class CompactPerformance {
public:
 ✅   double getCPUUsage();
 ✅   double getRAMUsage();
 ✅   double getDiskUsage();
 ✅   double getGPUUsage();
 ✅   std::string getUptime();
};
```

---




⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⣴⠚⡄⢫⠉⢩⠃⠀⣰⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡄⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠠⡞⠳⡜⠦⠚⠘⠒⠋⠀⢰⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠈⠚⠁⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣤⣄⣀⡀⣠⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⢸⠉⠉⢰⠂
⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⠟⠛⠙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⡇⠀⡰⠃⠀
⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⠃⠀⠀⢰⡆⢻⣿⣿⣿⣿⣿⡿⠛⠛⠻⣿⣿⣿⣿⠀⠀⠀⠀⢰⠁⡰⠁⠀⠀
⠀⠀⠀⠀⣶⣦⣤⠘⣿⣿⣆⠀⠀⢿⠇⣼⣿⣿⣿⣿⠛⢸⡇⠀⠀⠙⣿⣿⣿⣇⠀⠀⠀⠈⠒⠁⠀⠀⠀
⠀⠀⠀⠀⢀⣀⣁⠀⠻⣿⣿⣷⣦⣤⣶⣿⣿⣿⣿⣇⡀⣾⠃⠀⠀⢠⣿⣿⣿⡇⠀⡔⠒⠲⡄⠀⠀
⠀⠀⠀⠘⠛⠋⠉⠀⠀⠈⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣤⣴⣾⣿⣿⠟⠀⠀⠱⠤⣀⠇⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⢀⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠀⠀⣀⡀⠀⠉⠙⠓⠀⠀⠀⠀⠀⠀⠀
⠀⡗⡤⡲⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⣴⣫⢸⢷⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠈⠞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀


# project structure

project_binary_fetch - Copy\
|-->[.history]
|    #-->[binary_fetch_v1]
|         |-->ExtraInfo_20251014031438.cpp
|         |-->ExtraInfo_20251014204925.cpp
|         |-->ExtraInfo_20251014204934.cpp
|         |-->ExtraInfo_20251014204949.cpp
|         |-->ExtraInfo_20251014204957.cpp
|         |-->ExtraInfo_20251014205027.cpp
|         |-->ExtraInfo_20251014205031.cpp
|         |-->ExtraInfo_20251014205034.cpp
|         |-->ExtraInfo_20251014205038.cpp
|         |-->ExtraInfo_20251014205044.cpp
|         |-->ExtraInfo_20251014205048.cpp
|         |-->ExtraInfo_20251014210147.cpp
|         |-->ExtraInfo_20251014210157.cpp
|         |-->ExtraInfo_20251014210204.cpp
|         |-->ExtraInfo_20251014210207.cpp
|         |-->ExtraInfo_20251014210209.cpp
|         |-->ExtraInfo_20251014210614.cpp
|         |-->ExtraInfo_20251014210617.cpp
|         |-->ExtraInfo_20251014210633.cpp
|         #-->ExtraInfo_20251014210646.cpp
|-->[.vscode]
|    |-->c_cpp_properties.json
|    |-->launch.json
|    #-->settings.json
#-->[binary_fetch_v1]
     |-->[.vs]
     |    #-->[binary_fetch_v1]
     |         |-->ColorByRegexConfig.txt
     |         |-->[FileContentIndex]
     |         |    |-->1794f7b8-8a13-4f9f-9fda-38906b656faf.vsidx
     |         |    |-->2f3bee28-9246-4977-a545-dcaa7cee570b.vsidx
     |         |    |-->7ca79fb9-0aed-4eeb-8e6b-08a5d3f1a188.vsidx
     |         |    |-->7dfaa8cd-2ed0-4808-b26a-db47e62c466a.vsidx
     |         |    #-->913e5547-8981-4eeb-827c-2b335f9ff81a.vsidx
     |         #-->[v17]
     |              |-->.suo
     |              |-->Browse.VC.db
     |              |-->Browse.VC.db-shm
     |              |-->Browse.VC.db-wal
     |              |-->Browse.VC.opendb
     |              |-->DocumentLayout.backup.json
     |              |-->DocumentLayout.json
     |              |-->Solution.VC.db
     |              |-->fileList.bin
     |              #-->[ipch]
     |                   #-->[AutoPCH]
     |                        |-->[12234ec71e6866af]
     |                        |-->[14425e87f00ed387]
     |                        |-->[15a593a05dc459f0]
     |                        |    #-->STORAGEINFO.ipch
     |                        |-->[1651bcd7e47d4a4e]
     |                        |-->[1726e23b034ac3e9]
     |                        |    #-->NETWORKINFO.ipch
     |                        |-->[183fda512fefa70a]
     |                        |-->[1895343a350d9530]
     |                        |    #-->COMPACTAUDIO.ipch
     |                        |-->[1cdacb36f5f64047]
     |                        |-->[2058a7acbc39782f]
     |                        |-->[2084cb5061693180]
     |                        |-->[26741934a048a92a]
     |                        |-->[28958ce4e537baa2]
     |                        |-->[2c2d26a95f25dda6]
     |                        |-->[2c6862a49d56d26a]
     |                        |-->[333a7ded43208cc8]
     |                        |    #-->MAIN.ipch
     |                        |-->[3368e5b8a2d7e2e2]
     |                        |    #-->USERINFO.ipch
     |                        |-->[339245fae64e923a]
     |                        |-->[35675c0cf41f2d36]
     |                        |    #-->COMPACTOS.ipch
     |                        |-->[377f1fb52b9377ab]
     |                        |-->[38aab1c63bb4b271]
     |                        |    #-->DISPLAYINFO.ipch
     |                        |-->[3cfc55e6c6200f50]
     |                        |-->[3dbc6c352d1fc992]
     |                        |-->[3e57b611fc24a27c]
     |                        |-->[45d533a2b26093cb]
     |                        |    #-->COMPACTNETWORK.ipch
     |                        |-->[4733d3dca1dfb15a]
     |                        |-->[4bd42a35328170b8]
     |                        |-->[4cf53ec3023d9e5f]
     |                        |    #-->PERFORMANCEINFO.ipch
     |                        |-->[4e13d5c043d4033d]
     |                        |    #-->CPUINFO.ipch
     |                        |-->[4fd6f130cbe2b199]
     |                        |-->[5197895645119c5e]
     |                        |-->[56cacfcaaf262d03]
     |                        |-->[586fccd770366ee4]
     |                        |-->[59cc4d6150f693d7]
     |                        |-->[5c6868d22400e22]
     |                        |-->[5dbc219f7b1f1da4]
     |                        |    #-->ASCIIART.ipch
     |                        |-->[5f09e24cef17122d]
     |                        |    #-->COMPACTUSER.ipch
     |                        |-->[64672f853b4bfb80]
     |                        |    #-->COMPACTSCREEN.ipch
     |                        |-->[662ff0359085c728]
     |                        |    #-->DISPLAYINFO.ipch
     |                        |-->[66e9782c95bcb3e9]
     |                        |-->[6b5d386e46ed2086]
     |                        |    #-->COMPACTGPU.ipch
     |                        |-->[6bd53eedffb188ca]
     |                        |    #-->MEMORYINFO.ipch
     |                        |-->[78344169b2010f09]
     |                        |-->[7c427c90fc994f9b]
     |                        |-->[7cb05e799330c3a6]
     |                        |-->[817418e1ba077b77]
     |                        |    #-->OSINFO.ipch
     |                        |-->[81846f0c13ebc3ac]
     |                        |    #-->COMPACTDISPLAY.ipch
     |                        |-->[819e933538641383]
     |                        |-->[84a3340e28343194]
     |                        |-->[84e773b22c115c0e]
     |                        |-->[899e8e43de7eb2d8]
     |                        |-->[8a0638d9620f57ca]
     |                        |-->[8b97699255e957ca]
     |                        |    #-->DTAILEDGPUINFO.ipch
     |                        |-->[8c2ee13e50bc5703]
     |                        |-->[8c766e354ea06264]
     |                        |-->[8d381eed98080518]
     |                        |-->[90e831a28fb7d676]
     |                        |-->[910d0c8d02d37fc4]
     |                        |-->[935b278ecae8978b]
     |                        |-->[9725a63f95378a51]
     |                        |    #-->COMPACTMEMORY.ipch
     |                        |-->[993f5efd13ed593]
     |                        |    #-->GPUINFO.ipch
     |                        |-->[9c3222fb3937cc96]
     |                        |    #-->COMPACTNETWORK.ipch
     |                        |-->[9cc5b166e38c49e6]
     |                        |    #-->COMPACTDISPLAY.ipch
     |                        |-->[9cddcffc3bcaff7]
     |                        |-->[9f33b00a9010e6cc]
     |                        |    #-->COMPACTGPU.ipch
     |                        |-->[a51413011be751ea]
     |                        |    #-->COMPACTSCREEN.ipch
     |                        |-->[a7cb165e2c23f75a]
     |                        |-->[a96696f66245920e]
     |                        |    #-->MAIN.ipch
     |                        |-->[acac63f572eadd2d]
     |                        |    #-->ASCIIART.ipch
     |                        |-->[af25f469592d4aa4]
     |                        |    #-->COMPACTUSER.ipch
     |                        |-->[b3b4ed992d656d84]
     |                        |-->[b5090a5be8ab093f]
     |                        |-->[b6bc5ffc0375030c]
     |                        |-->[b8bf90da2aea28f4]
     |                        |    #-->SYSTEMINFO.ipch
     |                        |-->[bcc2c23ff8999393]
     |                        |-->[c07b5be3d36417f3]
     |                        |-->[c30374b7ebfaaeb1]
     |                        |    #-->GPUINFO.ipch
     |                        |-->[c4bc2e02fd88390d]
     |                        |-->[c661ae1640e0519]
     |                        |-->[c74d85ce8a2deb76]
     |                        |-->[cbf74c3f67a21e51]
     |                        |-->[ccfd165a96fc8193]
     |                        |-->[cf53804eaba6bb80]
     |                        |-->[d68ddaebbecd8c45]
     |                        |-->[d72caa60270d2598]
     |                        |-->[d892e531c6a09cb6]
     |                        |-->[d8d5b0d31d1a2a12]
     |                        |-->[d8f133ffe7615c82]
     |                        |    #-->COMPACTAUDIO.ipch
     |                        |-->[d922fd6c87bbe026]
     |                        |-->[dbe673ebbc013a16]
     |                        |-->[df838178c8511af3]
     |                        |-->[e099c0bafb5f9520]
     |                        |-->[e1256465f2be34f]
     |                        |-->[e2109c67f8f778a9]
     |                        |    #-->EXTRAINFO.ipch
     |                        |-->[e4d2938647cb7ab0]
     |                        |    #-->COMPACTPERFORMANCE.ipch
     |                        |-->[ea87f331a6dea34e]
     |                        |-->[ebb50e9e7e88eca7]
     |                        |-->[ec594a5e90a685ec]
     |                        |-->[ecdf932ba9780cb]
     |                        |-->[ed3a2d1f3ee7dcf7]
     |                        |    #-->COMPACTSYSTEM.ipch
     |                        |-->[eea63fa9b6c1bed]
     |                        |    #-->CENTRALCONTROL.ipch
     |                        |-->[f3fbee9cdf4a9bb8]
     |                        |-->[f43abe9bcb461456]
     |                        |    #-->COMPACTPERFORMANCE.ipch
     |                        |-->[f66d0164cc1ea300]
     |                        |-->[f90c65e7ad70303]
     |                        #-->[fcce0ac01d8ded52]
     |                             #-->COMPACTCPU.ipch
     |-->AsciiArt.cpp
     |-->AsciiArt.h
     |-->AsciiArt.txt
     |-->CPUInfo.cpp
     |-->CPUInfo.h
     |-->CentralControl.cpp
     |-->CentralControl.h
     |-->CompactAudio.cpp
     |-->CompactAudio.h
     |-->CompactCPU.cpp
     |-->CompactCPU.h
     |-->CompactDisplay.cpp
     |-->CompactDisplay.h
     |-->CompactGPU.cpp
     |-->CompactGPU.h
     |-->CompactMemory.cpp
     |-->CompactMemory.h
     |-->CompactNetwork.cpp
     |-->CompactNetwork.h
     |-->CompactOS.cpp
     |-->CompactOS.h
     |-->CompactPerformance.cpp
     |-->CompactPerformance.h
     |-->CompactScreen.cpp
     |-->CompactScreen.h
     |-->CompactSystem.cpp
     |-->CompactSystem.h
     |-->CompactUser.cpp
     |-->CompactUser.h
     |-->ConfigReader.cpp
     |-->[ConfigReader.h]
     |-->ConsoleUtils.cpp
     |-->Core
     |-->DetailedGPUInfo.h
     |-->DisplayInfo.cpp
     |-->DisplayInfo.h
     |-->DtailedGPUInfo.cpp
     |-->ExtraInfo.cpp
     |-->ExtraInfo.h
     |-->GPUInfo.cpp
     |-->GPUInfo.h
     |-->Helpers.cpp
     |-->MemoryInfo.cpp
     |-->MemoryInfo.h
     |-->NetworkInfo.cpp
     |-->NetworkInfo.h
     |-->OSInfo.cpp
     |-->OSInfo.h
     |-->PerformanceInfo.cpp
     |-->PerformanceInfo.h
     |-->StorageInfo.cpp
     |-->StorageInfo.h
     |-->[StringUtlis.h]
     |    #-->cpp
     |-->SystemInfo.cpp
     |-->SystemInfo.h
     |-->UserInfo.cpp
     |-->UserInfo.h
     |-->Y
     |-->[binary_fetch]
     |    #-->[x64]
     |         #-->[Debug]
     |              |-->AsciiArt.obj
     |              |-->CPUInfo.obj
     |              |-->CentralControl.obj
     |              |-->CompactAudio.obj
     |              |-->CompactCPU.obj
     |              |-->CompactDisplay.obj
     |              |-->CompactGPU.obj
     |              |-->CompactMemory.obj
     |              |-->CompactNetwork.obj
     |              |-->CompactOS.obj
     |              |-->CompactPerformance.obj
     |              |-->CompactScreen.obj
     |              |-->CompactSystem.obj
     |              |-->CompactUser.obj
     |              |-->ConfigReader.obj
     |              |-->ConsoleUtils.obj
     |              |-->DetailedGPUInfo.obj
     |              |-->DisplayInfo.obj
     |              |-->DtailedGPUInfo.obj
     |              |-->ExtraInfo.obj
     |              |-->GPUInfo.obj
     |              |-->Helpers.obj
     |              |-->MemoryInfo.obj
     |              |-->NetworkInfo.obj
     |              |-->OSInfo.obj
     |              |-->PerformanceInfo.obj
     |              |-->StorageInfo.obj
     |              |-->SystemInfo.obj
     |              |-->UserInfo.obj
     |              |-->binary_fetch.Build.CppClean.log
     |              |-->binary_fetch.exe.recipe
     |              |-->binary_fetch.ilk
     |              |-->[binary_fetch.tlog]
     |              |    |-->CL.command.1.tlog
     |              |    |-->CL.read.1.tlog
     |              |    |-->CL.write.1.tlog
     |              |    |-->Cl.items.tlog
     |              |    |-->binary_fetch.lastbuildstate
     |              |    |-->link.command.1.tlog
     |              |    |-->link.read.1.tlog
     |              |    |-->link.secondary.1.tlog
     |              |    |-->link.write.1.tlog
     |              |    |-->rc.command.1.tlog
     |              |    |-->rc.read.1.tlog
     |              |    #-->rc.write.1.tlog
     |              |-->binary_fetch_v1.log
     |              |-->binary_fetch_v1.vcxproj.FileListAbsolute.txt
     |              |-->main.obj
     |              |-->personalization_info.obj
     |              |-->vc143.idb
     |              |-->vc143.pdb
     |              #-->version_info.res
     |-->[binary_fetch_v1]
     |    #-->[x64]
     |         |-->[Debug]
     |         |    |-->CPUInfo.obj
     |         |    |-->CentralControl.obj
     |         |    |-->CompactAudio.obj
     |         |    |-->CompactCPU.obj
     |         |    |-->CompactDisplay.obj
     |         |    |-->CompactGPU.obj
     |         |    |-->CompactMemory.obj
     |         |    |-->CompactNetwork.obj
     |         |    |-->CompactOS.obj
     |         |    |-->CompactPerformance.obj
     |         |    |-->CompactScreen.obj
     |         |    |-->CompactSystem.obj
     |         |    |-->CompactUser.obj
     |         |    |-->ConfigReader.obj
     |         |    |-->ConsoleUtils.obj
     |         |    |-->DetailedGPUInfo.obj
     |         |    |-->DisplayInfo.obj
     |         |    |-->DtailedGPUInfo.obj
     |         |    |-->ExtraInfo.obj
     |         |    |-->GPUInfo.obj
     |         |    |-->Helpers.obj
     |         |    |-->MemoryInfo.obj
     |         |    |-->NetworkInfo.obj
     |         |    |-->OSInfo.obj
     |         |    |-->PerformanceInfo.obj
     |         |    |-->StorageInfo.obj
     |         |    |-->SystemInfo.obj
     |         |    |-->UserInfo.obj
     |         |    |-->binary_fetch_v1.Build.CppClean.log
     |         |    |-->binary_fetch_v1.exe.recipe
     |         |    |-->binary_fetch_v1.ilk
     |         |    |-->binary_fetch_v1.log
     |         |    |-->[binary_fetch_v1.tlog]
     |         |    |    |-->CL.command.1.tlog
     |         |    |    |-->CL.read.1.tlog
     |         |    |    |-->CL.write.1.tlog
     |         |    |    |-->Cl.items.tlog
     |         |    |    |-->binary_fetch_v1.lastbuildstate
     |         |    |    |-->link.command.1.tlog
     |         |    |    |-->link.read.1.tlog
     |         |    |    |-->link.secondary.1.tlog
     |         |    |    |-->link.write.1.tlog
     |         |    |    |-->rc.command.1.tlog
     |         |    |    |-->rc.read.1.tlog
     |         |    |    #-->rc.write.1.tlog
     |         |    |-->binary_fetch_v1.vcxproj.FileListAbsolute.txt
     |         |    |-->main.obj
     |         |    |-->vc143.idb
     |         |    |-->vc143.pdb
     |         |    #-->version_info.res
     |         #-->[Release]
     |              |-->CPUInfo.obj
     |              |-->ConfigReader.obj
     |              |-->ConsoleUtils.obj
     |              |-->DetailedGPUInfo.obj
     |              |-->DisplayInfo.obj
     |              |-->DtailedGPUInfo.obj
     |              |-->GPUInfo.obj
     |              |-->Helpers.obj
     |              |-->MemoryInfo.obj
     |              |-->NetworkInfo.obj
     |              |-->OSInfo.obj
     |              |-->StorageInfo.obj
     |              |-->SystemInfo.obj
     |              |-->binary_fetch_v1.exe.recipe
     |              |-->binary_fetch_v1.iobj
     |              |-->binary_fetch_v1.ipdb
     |              |-->binary_fetch_v1.log
     |              |-->[binary_fetch_v1.tlog]
     |              |    |-->CL.command.1.tlog
     |              |    |-->CL.read.1.tlog
     |              |    |-->CL.write.1.tlog
     |              |    |-->Cl.items.tlog
     |              |    |-->binary_fetch_v1.lastbuildstate
     |              |    |-->link.command.1.tlog
     |              |    |-->link.read.1.tlog
     |              |    |-->link.secondary.1.tlog
     |              |    |-->link.write.1.tlog
     |              |    |-->rc.command.1.tlog
     |              |    |-->rc.read.1.tlog
     |              |    #-->rc.write.1.tlog
     |              |-->main.obj
     |              |-->vc143.pdb
     |              #-->version_info.res
     |-->binary_fetch_v1.aps
     |-->binary_fetch_v1.cpp
     |-->binary_fetch_v1.rc
     |-->binary_fetch_v1.sln
     |-->binary_fetch_v1.vcxproj
     |-->binary_fetch_v1.vcxproj.filters
     |-->binary_fetch_v1.vcxproj.user
     |-->config.json
     |-->json.hpp
     |-->main.cpp
     |-->[output]
     |-->personalization_info.cpp
     |-->personalization_info.h
     |-->resource.h
     |-->version_info.aps
     |-->version_info.rc
     #-->[x64]
          |-->[Debug]
          |    |-->binary_fetch.exe
          |    |-->binary_fetch.pdb
          |    |-->binary_fetch_v1.exe
          |    #-->binary_fetch_v1.pdb
          #-->[Release]
               |-->binary_fetch_v1.exe
               #-->binary_fetch_v1.pdb




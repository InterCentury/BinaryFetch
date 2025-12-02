#pragma once
#include <iostream>
#include <string>
#include <vector>
using  namespace std;

struct storage_data {
    string drive_letter;
    string used_space;
    string total_space;
    string used_percentage;
    string file_system;
    bool is_external;

    // extra disk infos (extended) 
    string serial_number;
    string read_speed;
    string write_speed;

    // extra disk infos (extended & predectied)

    string predicted_read_speed;
    string predicted_write_speed;
};

class StorageInfo {
public:
    vector<storage_data> get_all_storage_info();
};

// ExtraInfo.h
// ============================
// Project: BinaryFetch
// Module: ExtraInfo
// Description: Provides additional optional system information for BinaryFetch,
//              including audio devices, power status, running processes, and installed applications.
// Author: Maruf Hasan
// Date: 2025-10-14
// Version: 1.0
// Dependencies: Windows.h, string, vector, optional Windows APIs
// ============================

#pragma once
#include <string>
#include <vector>
using namespace std;

class ExtraInfo
{
public:
	// ?? Returns a list of available audio devices.
	// Includes both input (microphones) and output (speakers/headphones).
	// Marks which device is currently active.
	// Example:
	//   Headphone (High Definition Audio) (active)
	//   Speaker (High Definition Audio)
	//   Microphone (High Definition Audio) (active)
	//   Microphone (DroidCam)
	string get_audio_devices();

	// ?? Returns the system's current power status.
	// For desktop PCs ? "Power Status: Wired connection"
	// For laptops ? "Power Status: Battery powered (85%) (Charging)"
	//               or "Power Status: Battery powered (62%) (Not Charging)"
	string get_power_status();
};



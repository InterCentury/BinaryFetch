#include "ExtraInfo.h"
#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <powrprof.h>
#include <sstream>
using namespace std;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

string ExtraInfo::get_audio_devices()
{
    HRESULT hr;
    CoInitialize(nullptr);
    IMMDeviceEnumerator* pEnum = nullptr;
    IMMDeviceCollection* pDevices = nullptr;

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
    if (FAILED(hr))
    {
        CoUninitialize();
        return "Audio Devices: Failed to enumerate.";
    }


    hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &pDevices);
    if (SUCCEEDED(hr))
    {
        UINT count = 0;
        pDevices->GetCount(&count);

        IMMDevice* pDefaultOut = nullptr;
        pEnum->GetDefaultAudioEndpoint(eRender, eConsole, &pDefaultOut);

        for (UINT i = 0; i < count; i++)
        {
            IMMDevice* pDevice = nullptr;
            IPropertyStore* pProps = nullptr;
            LPWSTR deviceId = nullptr;

            pDevices->Item(i, &pDevice);
            pDevice->OpenPropertyStore(STGM_READ, &pProps);

            PROPVARIANT name;
            PropVariantInit(&name);
            pProps->GetValue(PKEY_Device_FriendlyName, &name);

            pDevice->GetId(&deviceId);
            wstring wname(name.pwszVal);
            string deviceName(wname.begin(), wname.end());

            // Check if active (default output)
            bool isActive = false;
            if (pDefaultOut)
            {
                LPWSTR defId = nullptr;
                pDefaultOut->GetId(&defId);
                if (wcscmp(deviceId, defId) == 0)
                    isActive = true;
                CoTaskMemFree(defId);
            }

            cout << "  " << deviceName;
            if (isActive)
            {
                setColor(10); // Green
                cout << " (active)";
                setColor(7); // Default
            }
            cout << endl;

            PropVariantClear(&name);
            if (pProps) pProps->Release();
            if (pDevice) pDevice->Release();
            CoTaskMemFree(deviceId);
        }

        if (pDefaultOut) pDefaultOut->Release();
        pDevices->Release();
    }

    

    hr = pEnum->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &pDevices);
    if (SUCCEEDED(hr))
    {
        UINT count = 0;
        pDevices->GetCount(&count);

        IMMDevice* pDefaultIn = nullptr;
        pEnum->GetDefaultAudioEndpoint(eCapture, eConsole, &pDefaultIn);

        for (UINT i = 0; i < count; i++)
        {
            IMMDevice* pDevice = nullptr;
            IPropertyStore* pProps = nullptr;
            LPWSTR deviceId = nullptr;

            pDevices->Item(i, &pDevice);
            pDevice->OpenPropertyStore(STGM_READ, &pProps);

            PROPVARIANT name;
            PropVariantInit(&name);
            pProps->GetValue(PKEY_Device_FriendlyName, &name);

            pDevice->GetId(&deviceId);
            wstring wname(name.pwszVal);
            string deviceName(wname.begin(), wname.end());

            bool isActive = false;
            if (pDefaultIn)
            {
                LPWSTR defId = nullptr;
                pDefaultIn->GetId(&defId);
                if (wcscmp(deviceId, defId) == 0)
                    isActive = true;
                CoTaskMemFree(defId);
            }

            cout << "  " << deviceName;
            if (isActive)
            {
                setColor(11); // Sky blue for brackets
                cout << "(";
                setColor(10); // Green for "active"
                cout << "active";
                setColor(11); // Sky blue for closing bracket
                cout << ")";
                setColor(7);  // Default
            }
            cout << endl;

            PropVariantClear(&name);
            if (pProps) pProps->Release();
            if (pDevice) pDevice->Release();
            CoTaskMemFree(deviceId);
        }

        if (pDefaultIn) pDefaultIn->Release();
        pDevices->Release();
    }

    if (pEnum) pEnum->Release();
    CoUninitialize();

    return "";
}

// Power status
string ExtraInfo::get_power_status()
{
    SYSTEM_POWER_STATUS sps;
    if (!GetSystemPowerStatus(&sps))
        return "Power Status: Unknown";

    bool hasBattery = (sps.BatteryFlag != 128);

    cout << "Power Status: ";
    if (!hasBattery)
    {
        setColor(11); // Sky blue
        cout << "[Wired connection]";
        setColor(7);
        cout << endl;
        return "";
    }

    cout << "Battery powered ";
    setColor(11); cout << "(";           // Sky blue bracket
    setColor(14); cout << (int)sps.BatteryLifePercent << "%"; // Orange percentage
    setColor(11); cout << ")";           // Sky blue closing bracket
    setColor(7);

    if (sps.ACLineStatus == 1)
    {
        cout << " ";
        setColor(10); cout << "(";      // Sky blue bracket
        setColor(10); cout << "Charging"; // Green text
        setColor(10); cout << ")";      // Sky blue closing bracket
        setColor(7);
    }
    else
    {
        cout << " ";
        setColor(11); cout << "(";      // Sky blue bracket
        setColor(12); cout << "Not Charging"; // Red text
        setColor(11); cout << ")";      // Sky blue closing bracket
        setColor(7);
    }
    cout << endl;
    return "";
}

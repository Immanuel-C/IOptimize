#include "IOptimize.h"

#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>

struct _IOptimizeSetRegistryDwordValuesInfo {
    HKEY hKey;
    LPCSTR subKey;
    const char** valueNames;
    size_t valueNamesSize;
    const DWORD* valueData;
    DWORD cbSizePerIndex;
};

struct _IOptimizeDeleteRegistryValuesInfo {
    HKEY hKey;
    LPCSTR subKey;
    const char** valueNames;
    size_t valueNamesSize;

};

static void _IOptimizeSetRegistryDwordValues(struct _IOptimizeSetRegistryDwordValuesInfo* info) {
    HKEY key;
    LRESULT result = 0;

    result = RegCreateKeyExA(info->hKey, info->subKey, NULL, NULLPTR, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULLPTR, &key, NULLPTR);
    if (result != ERROR_SUCCESS) {
        char info[512] = { 0 };
        vsprintf(info, "RegCreateKeyEx failed\nError Code: %i", result);
        MessageBoxA(NULL, info, "Error!", MB_OK);
        return;
    }

    for (size_t i = 0; i < info->valueNamesSize; i++) {
        result = RegSetValueExA(key, info->valueNames[i], NULL, REG_DWORD, (LPBYTE)&info->valueData[i], info->cbSizePerIndex);
        if (result != ERROR_SUCCESS) {
            MessageBoxA(NULL, "RegSetValueExA failed", "Error!", MB_OK);
            return;
        }
    }

    RegCloseKey(key);
}

static void _IOptimizeDeleteRegistryValues(struct _IOptimizeDeleteRegistryValuesInfo* info) {
    HKEY key;
    LRESULT result;
    result = RegCreateKeyExA(info->hKey, info->subKey, NULL, NULLPTR, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULLPTR, &key, NULLPTR);
    if (result != ERROR_SUCCESS) {
        char info[512] = { 0 };
        vsprintf_s(info, 512, "RegCreateKeyEx failed\nError Code: %i", result);
        MessageBoxA(NULL, info, "Error!", MB_OK);
        return;
    }
    for (size_t i = 0; i < info->valueNamesSize; i++) RegDeleteValueA(key, info->valueNames[i]);
    RegCloseKey(key);
}


// TODO: Complete function
void IOptimizeSetGpuMsiMode(int msi) {
    int result;
    
    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &GUID_DEVCLASS_DISPLAY, // Only get display adaptors (GPU, APU, etc...)
        NULL,                   // ?
        NULL,                   // Parent window
        DIGCF_PRESENT           // Device(s) has to be currently present in the system
    );

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        result = GetLastError();
        return;
    }

    SP_DEVINFO_DATA deviceInfoData = { sizeof(SP_DEVINFO_DATA) };

    for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); i++) {
        DWORD reqBufsize = 0;
        const char* fakeInstanceId = '\0';
        SetupDiGetDeviceInstanceIdA(hDevInfo, &deviceInfoData, (PBYTE)fakeInstanceId, 1, &reqBufsize);

        const char* subKey = malloc(reqBufsize + 256);
        size_t subKeySize = reqBufsize + 256;
        memset(subKey, 0, subKeySize);

        const char* instanceId = malloc(reqBufsize);
        DWORD instanceIdSize = reqBufsize;
        memset(instanceId, 0, instanceIdSize);

        // get device description information
        if (!SetupDiGetDeviceInstanceIdA(hDevInfo, &deviceInfoData, (PBYTE)instanceId, instanceIdSize, &reqBufsize)) {
            result = GetLastError();
            return;
        }
        
        char temp[512] = { 0 };
        sprintf_s(temp, 512, "Display device %d: Device Instance ID: %s\n", i, instanceId);
        puts(temp);

        strcat(subKey, "SYSTEM\\CurrentControlSet\\Enum\\");
        strcat(subKey, instanceId);
        strcat(subKey, "\\Device Parameters\\Interrupt Management\\MessageSignaledInterruptProperties\\");

        const char* valueName = "MSISupported";

        if (msi) {
            DWORD valueData = 0x00000001;

            struct _IOptimizeSetRegistryDwordValuesInfo setMsiModeRegistryValue = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = subKey,
                .valueNames = &valueName,
                .valueNamesSize = 1,
                .valueData = &valueData,
                .cbSizePerIndex = sizeof(DWORD)
            };

            _IOptimizeSetRegistryDwordValues(&setMsiModeRegistryValue);
        }
        else if (!msi) {
            DWORD valueData = 0x00000000;

            struct _IOptimizeSetRegistryDwordValuesInfo setMsiModeRegistryValue = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = subKey,
                .valueNames = &valueName,
                .valueNamesSize = 1,
                .valueData = &valueData,
                .cbSizePerIndex = sizeof(DWORD)
            };

            _IOptimizeSetRegistryDwordValues(&setMsiModeRegistryValue);
        }

        free(subKey);
        free(instanceId);
    }
}


void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType) {


    const char* valueNamesPriorities[] = {
        "CpuPriorityClass",
        "IoPriority"
    };

    size_t valueNamesPrioritiesSize = sizeof(valueNamesPriorities) / sizeof(valueNamesPriorities[0]);

    DWORD valueCsrssPrioritiesData[] = {
        0x00000004,
        0x00000003
    };

    DWORD valueGamePrioritiesData[] = {
        0x00000003,
        0x00000003
    };

    const char* priorityControlValueNames = "Win32PrioritySeparation";

    DWORD priorityControlValueData = 0x16;

    if (optimizeType & IOPTIMIZE_TYPE_LATENCY) {
        {
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\csrss.exe\\PerfOptions",
                .valueNames = valueNamesPriorities,
                .valueNamesSize = valueNamesPrioritiesSize,
                .valueData = valueCsrssPrioritiesData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
        {
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SYSTEM\\ControlSet001\\Control\\PriorityControl",
                .valueNames = &priorityControlValueNames,
                .valueNamesSize = 1,
                .valueData = &priorityControlValueData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
    }

    const char* multimediaValueNames[] = {
        "SystemResponsiveness",
        "NetworkThrottlingIndex"
    };

    size_t multimediaValueSize = sizeof(multimediaValueNames) / sizeof(multimediaValueNames[0]);

    DWORD multimediaValueData[] = {
        0x0,
        0xffffffff
    };


    const char* queueSizeValueNames[] = {
        "KeyboardDataQueueSize",
        "MouseDataQueueSize"
    };

    // Keep it at a safe(ish) value
    DWORD queueSizeValueData = 0x30;
    


    if (optimizeType & IOPTIMIZE_TYPE_FPS ) {
        {
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile",
                .valueNames = multimediaValueNames,
                .valueNamesSize = multimediaValueSize,
                .valueData = multimediaValueData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
        {
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SYSTEM\\ControlSet001\\Services\\kbdclass\\Parameters",
                .valueNames = &queueSizeValueNames[0],
                .valueNamesSize = 1,
                .valueData = &queueSizeValueData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);

            setRegistryInfo.subKey = "SYSTEM\\ControlSet001\\Services\\mouclass\\Parameters";
            setRegistryInfo.valueNames = &queueSizeValueNames[1];

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }


        if (optimizeType & IOPTIMIZE_TYPE_FORTNITE) {
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FortniteClient-Win64-Shipping.exe\\PerfOptions",
                .valueNames = valueNamesPriorities,
                .valueNamesSize = valueNamesPrioritiesSize,
                .valueData = valueGamePrioritiesData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
        if (optimizeType & IOPTIMIZE_TYPE_APEX_LEGENDS) {
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\r5apex.exe\\PerfOptions",
                .valueNames = valueNamesPriorities,
                .valueNamesSize =  1,
                .valueData = valueGamePrioritiesData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);

            // For DX12 version of Apex
            setRegistryInfo.subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\r5apex_dx12.exe\\PerfOptions";
            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);

        }
    }


    
    if (optimizeType & IOPTIMIZE_TYPE_REVERT) {
        struct _IOptimizeDeleteRegistryValuesInfo removeRegistryInfo = {
            .hKey = HKEY_LOCAL_MACHINE,
            .subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\csrss.exe\\PerfOptions",
            .valueNames = valueNamesPriorities,
            .valueNamesSize = valueNamesPrioritiesSize,
        };

        _IOptimizeDeleteRegistryValues(&removeRegistryInfo);

        removeRegistryInfo.subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\FortniteClient-Win64-Shipping.exe\\PerfOptions";
        _IOptimizeDeleteRegistryValues(&removeRegistryInfo);

        removeRegistryInfo.subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\r5apex.exe\\PerfOptions";
        _IOptimizeDeleteRegistryValues(&removeRegistryInfo);

        removeRegistryInfo.subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\r5apex_dx12.exe\\PerfOptions";
        _IOptimizeDeleteRegistryValues(&removeRegistryInfo);

        {
            DWORD oldMultimediaValues[] = {
                0x14,
                0xA,
            };

            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile",
                .valueNames = multimediaValueNames,
                .valueNamesSize = multimediaValueSize,
                .valueData = oldMultimediaValues,
                .cbSizePerIndex = sizeof(DWORD),
            };
            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
        {

            queueSizeValueData = 0x64;

            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SYSTEM\\ControlSet001\\Services\\kbdclass\\Parameters",
                .valueNames = &queueSizeValueNames[0],
                .valueNamesSize = 1,
                .valueData = &queueSizeValueData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);

            setRegistryInfo.subKey = "SYSTEM\\ControlSet001\\Services\\mouclass\\Parameters";
            setRegistryInfo.valueNames = &queueSizeValueNames[1];

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
        {
            priorityControlValueData = 0x2;
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SYSTEM\\ControlSet001\\Control\\PriorityControl",
                .valueNames = &priorityControlValueNames,
                .valueNamesSize = 1,
                .valueData = &priorityControlValueData,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
    }

}
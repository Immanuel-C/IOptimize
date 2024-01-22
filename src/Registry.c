#include "IOptimize.h"

#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>
#include <tlhelp32.h>
#include <stdlib.h>

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

// Credit to amitxv
// https://github.com/amitxv/TimerResolution/blob/main/SetTimerResolution/SetTimerResolution/SetTimerResolution.cpp
static int _IOptimizeCheckInstances(int maxInstanceCount, const wchar_t* processName) {
    int count = 0;

    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapShot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 processEntry = { 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapShot, &processEntry)) {
        CloseHandle(snapShot);
        return count;
    }

    while (Process32Next(snapShot, &processEntry)) 
        if (wcscmp(processEntry.szExeFile, processName)) count++;

    CloseHandle(snapShot);

    return count > maxInstanceCount;
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


    const char* globalTimerResolutionRequestsName = "GlobalTimerResolutionRequests";

    DWORD globalTimerResolutionRequestsValue = 0x1;


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
        {
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel",
                .valueNames = &globalTimerResolutionRequestsName,
                .valueNamesSize = 1,
                .valueData = &globalTimerResolutionRequestsValue,
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
                .valueNamesSize = valueNamesPrioritiesSize,
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
        {
            globalTimerResolutionRequestsValue = 0x0;
            struct _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel",
                .valueNames = &globalTimerResolutionRequestsName,
                .valueNamesSize = 1,
                .valueData = &globalTimerResolutionRequestsValue,
                .cbSizePerIndex = sizeof(DWORD),
            };

            _IOptimizeSetRegistryDwordValues(&setRegistryInfo);
        }
    }
}

extern NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);
extern NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);

struct IOptimizeTimerResolutionValues IOptimizeQueryTimerResolution() {
    ULONG minResolution, maxResolution, currResolution;

    if (NtQueryTimerResolution(&minResolution, &maxResolution, &currResolution)) {
        MessageBoxA(NULLPTR, "NtQueryTimerResolution failed!", "Error!", MB_OK | MB_ICONERROR);
        struct IOptimizeTimerResolutionValues failed = { 0, 0, 0 };
        return failed;
    }

    struct IOptimizeTimerResolutionValues values = {
        .minResolution  = minResolution,
        .maxResolution  = maxResolution,
        .currResolution = currResolution
    };


    return values;
}

void IOptimizeSetTimerResolution(uint32_t resolutionMs) {
    if (!_IOptimizeCheckInstances(1, L"IOptimize.dll")) {
        MessageBoxA(NULLPTR, "IOptimizeSetTimerResolution requires that only one instance of IOptimize is running!", "Error!", MB_OK | MB_ICONERROR);
        return;
    }

    if (resolutionMs < 0) {
        MessageBoxA(NULLPTR, "IOptimizeSetTimerResolution(uint32_t) requires that parameter uint32_t is not negative!", "Error!", MB_OK | MB_ICONERROR);
        return;
    }

    // Tell windows always honour timer resolution changes this process makes
    PROCESS_POWER_THROTTLING_STATE powerThrottling = { 0 };
    powerThrottling.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
    powerThrottling.ControlMask = PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION;
    powerThrottling.StateMask = 0;

    SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &powerThrottling, sizeof(PROCESS_POWER_THROTTLING_STATE));

    struct IOptimizeTimerResolutionValues values = IOptimizeQueryTimerResolution();


    if (NtSetTimerResolution(resolutionMs, TRUE, &values.currResolution)) {
        MessageBoxA(NULLPTR, "NtSetTimerResolution failed!", "Error!", MB_OK | MB_ICONERROR);
        return;
    }

    printf("Timer resolution set to: %.4f\n", ((double)values.currResolution / 10000.0));
}


struct _TimerResoltionDelta {
    int resolution;
    double averageDelta;
};

int cmpfunc(const void* a, const void* b) {

    return (*(struct _TimerResoltionDelta*)a).averageDelta - (*(struct _TimerResoltionDelta*)b).averageDelta;
}

// Credit to amitxv 
// https://github.com/amitxv/TimerResolution/blob/main/micro-adjust-benchmark.ps1
// https://github.com/amitxv/PC-Tuning/blob/main/docs/research.md#micro-adjusting-timer-resolution-for-higher-precision
uint32_t IOptimizeMicroAdjustTimerResolution() {
    uint32_t start = 5000, end = 8000, increment = 20, samples = 20;

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);


    if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
        printf("SetPriorityClass failed!\n");
        return 0;
    }

    size_t resolutionDeltasSize = (end - start) / increment;
    struct _TimerResoltionDelta* resolutionDeltas = malloc(resolutionDeltasSize);
    if (!resolutionDeltas) {
        printf("Malloc failed!");
        return 0;
    }

    int index = 0;
    for (int i = start; i < end; i += increment) {
        printf("Benchmarking: %.4f\n", (float)i);

        LARGE_INTEGER start, end;

        IOptimizeSetTimerResolution((uint32_t)i);

        Sleep(1);

        //double* samplesDelta = malloc(samples);
        double samplesDelta[20] = { 0 };
        if (!samplesDelta) {
            printf("Malloc failed!");
            return 0;
        }

        for (int y = 0;; y++) {
            QueryPerformanceCounter(&start);
            Sleep(1);
            QueryPerformanceCounter(&end);

            double deltaSeconds = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
            double deltaMiliSeconds = deltaSeconds * 1000;
            double deltaExtraTimeSlept = deltaMiliSeconds - 1;

            samplesDelta[y] = deltaExtraTimeSlept;

            if (y == samples) break;

            Sleep(100);
        }

        // first trial is almost always invalid. Disacard.
        size_t size = samples - 1;

        //double* samplesDeltaNew = malloc(size);
        double samplesDeltaNew[19] = { 0 };
        if (!samplesDeltaNew) {
            printf("Malloc failed!");
            return 0;
        }

        memcpy(samplesDeltaNew, samplesDelta + 1, size * sizeof(double));

        double sum = 0.0;

        for (int y = 0; y < size; y++) 
            sum += samplesDeltaNew[y];

        double average = sum / size;

        struct _TimerResoltionDelta resolutionDelta = {
            .resolution = i,
            .averageDelta = average,
        };

        resolutionDeltas[index] = resolutionDelta;
        
        index++;

        //free(samplesDelta);
        //free(samplesDeltaNew);
    }


    qsort(resolutionDeltas, resolutionDeltasSize, sizeof(struct _TimerResoltionDelta), cmpfunc);

    uint32_t newResolution = resolutionDeltas[0].resolution;

    free(resolutionDeltas);

    printf("New Resolution: %f.4ms\n", (float)(newResolution / 10000.0));

    return newResolution;

}

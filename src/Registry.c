#include "IOptimize.h"

#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>
#include <tlhelp32.h>
#include <stdlib.h>

typedef struct _IOptimizeSetRegistryDwordValuesInfo_T {
    HKEY hKey;
    LPCSTR subKey;
    const char** valueNames;
    size_t valueNamesSize;
    const DWORD* valueData;
    DWORD cbSizePerIndex;
} _IOptimizeSetRegistryDwordValuesInfo;

typedef struct _IOptimizeDeleteRegistryValuesInfo_T {
    HKEY hKey;
    LPCSTR subKey;
    const char** valueNames;
    size_t valueNamesSize;

} _IOptimizeDeleteRegistryValuesInfo;

static void _IOptimizeSetRegistryDwordValues(_IOptimizeSetRegistryDwordValuesInfo* info) {
    HKEY key;
    LRESULT result = 0;

    result = RegCreateKeyExA(info->hKey, info->subKey, 0, NULLPTR, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULLPTR, &key, NULLPTR);
    if (result != ERROR_SUCCESS) {
        char info[512] = { 0 };
        IOptimizeSprintf(info, 512, "RegCreateKeyEx failed\nError Code: %i", result);
        MessageBoxA(NULL, info, "Error!", MB_OK);
        return;
    }

    for (size_t i = 0; i < info->valueNamesSize; i++) {
        result = RegSetValueExA(key, info->valueNames[i], 0, REG_DWORD, (LPBYTE)&info->valueData[i], info->cbSizePerIndex);
        if (result != ERROR_SUCCESS) {
            MessageBoxA(NULL, "RegSetValueExA failed", "Error!", MB_OK);
            return;
        }
    }

    RegCloseKey(key);
}

static void _IOptimizeDeleteRegistryValues( _IOptimizeDeleteRegistryValuesInfo* info) {
    HKEY key;
    LRESULT result;
    result = RegCreateKeyExA(info->hKey, info->subKey, 0, NULLPTR, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULLPTR, &key, NULLPTR);
    if (result != ERROR_SUCCESS) {
        char info[512] = { 0 };
        sprintf(info, "RegCreateKeyEx failed\nError Code: %i", result);
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

    if (snapShot == INVALID_HANDLE_VALUE) return 2;

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
void IOptimizeSetGpuMsiMode(IOptimizeBool msi) {
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

        char* subKey = malloc(reqBufsize + 256);
        size_t subKeySize = reqBufsize + 256;
        memset(subKey, 0, subKeySize);

        char* instanceId = malloc(reqBufsize);
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

        if (msi == IOPTIMIZE_TRUE) {
            DWORD valueData = 0x00000001;

             _IOptimizeSetRegistryDwordValuesInfo setMsiModeRegistryValue = {
                .hKey = HKEY_LOCAL_MACHINE,
                .subKey = subKey,
                .valueNames = &valueName,
                .valueNamesSize = 1,
                .valueData = &valueData,
                .cbSizePerIndex = sizeof(DWORD)
            };

            _IOptimizeSetRegistryDwordValues(&setMsiModeRegistryValue);
        }
        else if (msi == IOPTIMIZE_FALSE) {
            DWORD valueData = 0x00000000;

             _IOptimizeSetRegistryDwordValuesInfo setMsiModeRegistryValue = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
         _IOptimizeDeleteRegistryValuesInfo removeRegistryInfo = {
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

             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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

             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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
             _IOptimizeSetRegistryDwordValuesInfo setRegistryInfo = {
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

IOptimizeTimerResolutionValues IOptimizeQueryTimerResolution() {
    ULONG minResolution, maxResolution, currResolution;

    if (NtQueryTimerResolution(&minResolution, &maxResolution, &currResolution)) {
        MessageBoxA(NULLPTR, "NtQueryTimerResolution failed!", "Error!", MB_OK | MB_ICONERROR);
        IOptimizeTimerResolutionValues failed = { 0, 0, 0 };
        return failed;
    }

    IOptimizeTimerResolutionValues values = {
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

    IOptimizeTimerResolutionValues values = IOptimizeQueryTimerResolution();


    if (NtSetTimerResolution(resolutionMs, TRUE, &values.currResolution)) {
        MessageBoxA(NULLPTR, "NtSetTimerResolution failed!", "Error!", MB_OK | MB_ICONERROR);
        return;
    }

    printf("Timer resolution set to: %.4f\n", ((double)values.currResolution / 10000.0));
}

typedef struct _IOptimizeTimerResoltionDelta_T {
    uint32_t resolution;
    double averageDelta;
} _IOptimizeTimerResoltionDelta;


static IOptimizeBool _IOptimizeBenchmarkTimerResolution(double* averageDeltaFromSleep, uint32_t samples, uint32_t sleepTimeBetweenQueries, uint32_t* minRes, uint32_t* maxRes, uint32_t* currentRes) {
    IOPTIMIZE_ASSERT(sleepTimeBetweenQueries > 0, "_IOptimizeBenchmarkTimerResolution() param sleepTimeBetweenQueries must be higher than 0!");
    IOPTIMIZE_ASSERT(samples > 1, "_IOptimizeBenchmarkTimerResolution() param samples must be higher than 1!");

    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);

    if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
        IOptimizeLogErr("SetPriorityClass failed! Failed to set process priority to realtime.\n");
        return IOPTIMIZE_FALSE;
    }

    ULONG minResolution, maxResolution, currentResolution;

    size_t sleepDeltasSize = (size_t)samples + 1;
    double* sleepDeltas = (double*)malloc(sizeof(double) * sleepDeltasSize);

    if (sleepDeltas == NULLPTR) {
        IOptimizeLogErr("Malloc failed!\n");
        return IOPTIMIZE_FALSE;
    }

    for (uint32_t i = 0; i < sleepDeltasSize; i++) {
        if (NtQueryTimerResolution(&minResolution, &maxResolution, &currentResolution) != 0) {
            IOptimizeLogErr("NtQueryTimerResolution failed!\n");
            return IOPTIMIZE_FALSE;
        }

        QueryPerformanceCounter(&start);
        Sleep(sleepTimeBetweenQueries);
        QueryPerformanceCounter(&end);

        double deltaSeconds = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        double deltaMs = deltaSeconds * 1000;
        double deltaFromSleep = deltaMs - sleepTimeBetweenQueries;

        sleepDeltas[i] = deltaFromSleep;

        Sleep(100);
    }

    // Discard first samples since almost invalid.
    size_t sleepDeltasNewsize = (size_t)sleepDeltasSize - 1;

    double sum = 0.0;

    for (uint32_t i = 1; i < sleepDeltasNewsize; i++) 
        sum += sleepDeltas[i];
    
    (*averageDeltaFromSleep) = sum / sleepDeltasNewsize;

    if (minRes != NULLPTR) 
        (*minRes) = (uint32_t)minResolution;
    if (maxRes != NULLPTR)
        (*maxRes) = (uint32_t)maxResolution;
    if (currentRes != NULLPTR)
        (*currentRes) = (uint32_t)currentResolution;

    free(sleepDeltas);

    return IOPTIMIZE_TRUE;
}

static int _IOptimizeTimerResoltionDeltaCmpFun(const void* a, const void* b) {
    const _IOptimizeTimerResoltionDelta* ta = (_IOptimizeTimerResoltionDelta*)a;
    const _IOptimizeTimerResoltionDelta* tb = (_IOptimizeTimerResoltionDelta*)b;

    if (ta->averageDelta > tb->averageDelta) return 1;
    else if (ta->averageDelta < tb->averageDelta) return -1;
    else return 0;
}


uint32_t IOptimizeMicroAdjustTimerResolution(uint32_t start, uint32_t end, uint32_t increment, uint32_t samples) {
    {
        ULONG minResolution, maxResolution, currentResolution;

        if (NtQueryTimerResolution(&minResolution, &maxResolution, &currentResolution) != 0) {
            IOptimizeLogErr("NtQueryTimerResolution failed!\n");
            return IOPTIMIZE_FALSE;
        }

        IOPTIMIZE_ASSERT(start >= maxResolution, "IOptimizeMicroAdjustTimerResolution param start must be higher or equal than the max timer resolution");
        IOPTIMIZE_ASSERT(end <= minResolution, "IOptimizeMicroAdjustTimerResolution param end must be lower or equal than the min timer resolution");
    }

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
        IOptimizeLogErr("SetPriorityClass failed! Failed to set process priority to realtime.\n");
        return 0;
    }

    size_t timerResolutionDeltasSize = (size_t)((double)(end - start) / increment);
    _IOptimizeTimerResoltionDelta* timerResolutionDeltas = (_IOptimizeTimerResoltionDelta*)malloc(sizeof(_IOptimizeTimerResoltionDelta) * timerResolutionDeltasSize);

    if (timerResolutionDeltas == NULLPTR) {
        IOptimizeLogErr("malloc failed!\n");
        return 0;
    }

    for (uint32_t desiredResolution = start, i = 0; desiredResolution < end; desiredResolution += increment, i++) {
        ULONG currResolution;
        NtSetTimerResolution((ULONG)desiredResolution, TRUE, &currResolution);

        IOptimizeLog("Benchmarking timer resolution: %u, samples: %u\n", desiredResolution, samples);

        double averageDeltaFromSleep;
        if (_IOptimizeBenchmarkTimerResolution(&averageDeltaFromSleep, samples, 1, NULLPTR, NULLPTR, NULLPTR) == IOPTIMIZE_FALSE) {
            IOptimizeLogErr("_IOptimizeBenchmarkTimerResolution failed!");
            return 0;
        }

        IOptimizeLog("Average delta from Sleep(%u): %0.4f\n", 1, averageDeltaFromSleep);

        _IOptimizeTimerResoltionDelta timerResolutionDelta = {
            .resolution = desiredResolution,
            .averageDelta = averageDeltaFromSleep
        };

        timerResolutionDeltas[i] = timerResolutionDelta;
    }

    qsort(timerResolutionDeltas, timerResolutionDeltasSize, sizeof(_IOptimizeTimerResoltionDelta), _IOptimizeTimerResoltionDeltaCmpFun);

    IOptimizeLog("Chosen timer resolution: %u, Average delta from Sleep(%u): %0.4f\n", timerResolutionDeltas[0].resolution, 1, timerResolutionDeltas[0].averageDelta);

    uint32_t chosenResolution = timerResolutionDeltas[0].resolution;

    free(timerResolutionDeltas);

    return chosenResolution;
}
#include "IOptimize.h"
#include "Utils.h"

#include <string.h>
#include <Windows.h>



void __IOPTIMIZE_STDCALL IOptimizeSetDisplayResolution(int width, int height) {
    RECT oldScreenRes = { 0 };
    GetWindowRect(GetDesktopWindow(), &oldScreenRes);

    DEVMODEA desiredMode = { 0 };
    desiredMode.dmSize = sizeof(DEVMODE);
    desiredMode.dmPelsWidth = (DWORD)width;
    desiredMode.dmPelsHeight = (DWORD)height;
    desiredMode.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH;
    int res = ChangeDisplaySettingsA(&desiredMode, CDS_UPDATEREGISTRY | CDS_GLOBAL | CDS_RESET);
    if (res != DISP_CHANGE_SUCCESSFUL) {
        char message[512] = { 0 };
        sprintf_s(message, 512, "Failed to change display resolution!\nError Code: %i\nSee https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-changedisplaysettingsa for more details for more info?", res);
        if (MessageBoxA(NULL, message, "Error!", MB_YESNO | MB_ICONERROR) == IDYES)
            ShellExecuteA(0, 0, "https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa", 0, 0, SW_SHOW);
        return;
    }
    if (MessageBoxA(NULL, "Revert changes?", "Revert?", MB_YESNO | MB_ICONQUESTION) == IDYES) {
        desiredMode.dmPelsWidth = (DWORD)oldScreenRes.right;
        desiredMode.dmPelsHeight = (DWORD)oldScreenRes.bottom;
        ChangeDisplaySettingsA(&desiredMode, CDS_UPDATEREGISTRY | CDS_GLOBAL | CDS_RESET);
    }

}
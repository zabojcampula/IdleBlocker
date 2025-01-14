#include <windows.h> 
#include <shellapi.h>
#include <stdio.h>
#include "IdleBlocker.h"

#define WM_TRAYICON (WM_USER+1)
#define CMD_EXIT 1
#define CMD_ABOUT 2
#define CMD_RESTORE 3

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_TRAYICON:
         if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP) {
             HMENU hMenu = CreatePopupMenu();
             AppendMenu(hMenu, MF_STRING, CMD_EXIT, "Exit");
             AppendMenu(hMenu, MF_STRING, CMD_ABOUT, "About");
             AppendMenu(hMenu, MF_STRING, CMD_RESTORE, "Restore");
             POINT pt;
             GetCursorPos(&pt);
             SetForegroundWindow(hwnd);
             TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
             DestroyMenu(hMenu);
         }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case CMD_EXIT:
            DestroyWindow(hwnd);
            break;
        case CMD_ABOUT:
            MessageBox(hwnd, "Idle Blocker prevents your computer from going idle.", "Idle Blocker", MB_OK);
            break;
        case CMD_RESTORE:
            SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
            break;
        }
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nShowCmd) 
{ 
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "IdleBlocker";
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Idle Blocker", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
 
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    strcpy(nid.szTip, "Idle Blocker");
    Shell_NotifyIcon(NIM_ADD, &nid);
  
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
 
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    SetThreadExecutionState(ES_CONTINUOUS);

    return 0; 
}
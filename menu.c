/*
 * menu_demo.c
 *
 * A minimal Win32 GUI application that creates a window with a
 * top menu bar (File / Edit / Help), each containing a few items.
 *
 * No external libraries required — only the standard Windows SDK
 * headers (windows.h), which come bundled with MinGW-w64, MSVC,
 * or any other Windows C toolchain.
 *
 * Build with MinGW:
 *   gcc menu_demo.c -o menu_demo.exe -mwindows
 *
 * Build with MSVC (Developer Command Prompt):
 *   cl menu_demo.c /link user32.lib gdi32.lib
 */

#include <windows.h>

/* ---- Menu item IDs ---- */
#define ID_FILE_NEW     1001
#define ID_FILE_OPEN    1002
#define ID_FILE_EXIT    1003
#define ID_EDIT_CUT     2001
#define ID_EDIT_COPY    2002
#define ID_EDIT_PASTE   2003
#define ID_HELP_ABOUT   3001

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HMENU CreateMainMenu(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    const wchar_t CLASS_NAME[] = L"MenuDemoWindowClass";

    WNDCLASSW wc = {0};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Window registration failed!", L"Error", MB_ICONERROR);
        return 1;
    }

    HMENU hMenu = CreateMainMenu();

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Win32 Menu Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 350,
        NULL,
        hMenu,          /* attach the menu to the window */
        hInstance,
        NULL
    );

    if (!hwnd) {
        MessageBoxW(NULL, L"Window creation failed!", L"Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

/* Builds the File / Edit / Help menu bar and returns the handle. */
HMENU CreateMainMenu(void)
{
    HMENU hMenuBar = CreateMenu();

    HMENU hFileMenu = CreatePopupMenu();
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_NEW,  L"&New\tCtrl+N");
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_OPEN, L"&Open...\tCtrl+O");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFileMenu, MF_STRING, ID_FILE_EXIT, L"E&xit\tAlt+F4");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");

    HMENU hEditMenu = CreatePopupMenu();
    AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_CUT,   L"Cu&t\tCtrl+X");
    AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_COPY,  L"&Copy\tCtrl+C");
    AppendMenuW(hEditMenu, MF_STRING, ID_EDIT_PASTE, L"&Paste\tCtrl+V");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hEditMenu, L"&Edit");

    HMENU hHelpMenu = CreatePopupMenu();
    AppendMenuW(hHelpMenu, MF_STRING, ID_HELP_ABOUT, L"&About");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hHelpMenu, L"&Help");

    return hMenuBar;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_FILE_NEW:
                    MessageBoxW(hwnd, L"File > New clicked", L"Info", MB_OK);
                    break;
                case ID_FILE_OPEN:
                    MessageBoxW(hwnd, L"File > Open clicked", L"Info", MB_OK);
                    break;
                case ID_FILE_EXIT:
                    DestroyWindow(hwnd);
                    break;
                case ID_EDIT_CUT:
                    MessageBoxW(hwnd, L"Edit > Cut clicked", L"Info", MB_OK);
                    break;
                case ID_EDIT_COPY:
                    MessageBoxW(hwnd, L"Edit > Copy clicked", L"Info", MB_OK);
                    break;
                case ID_EDIT_PASTE:
                    MessageBoxW(hwnd, L"Edit > Paste clicked", L"Info", MB_OK);
                    break;
                case ID_HELP_ABOUT:
                    MessageBoxW(hwnd, L"Win32 Menu Demo\nBuilt with plain Win32 API, no external libraries.",
                                L"About", MB_ICONINFORMATION | MB_OK);
                    break;
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}
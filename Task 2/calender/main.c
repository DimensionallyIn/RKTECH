#include <windows.h>
#include <commctrl.h>


#define MCS_SINGLESEL 0x0004


HWND g_hWndCalendar;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_DATE_CLASSES;
    if (!InitCommonControlsEx(&icc)) {
        MessageBox(NULL, TEXT("Error initializing common controls."), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }


    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TEXT("CalendarClass");
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, TEXT("Error registering window class."), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }


    HWND hWnd = CreateWindow(TEXT("CalendarClass"), TEXT("Calendar"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, TEXT("Error creating window."), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }


    g_hWndCalendar = CreateWindowEx(0, MONTHCAL_CLASS, NULL, WS_CHILD | WS_VISIBLE | MCS_DAYSTATE | MCS_SINGLESEL,
        10, 10, 360, 240, hWnd, NULL, hInstance, NULL);

    if (!g_hWndCalendar) {
        MessageBox(NULL, TEXT("Error creating calendar control."), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

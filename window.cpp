#include <windows.h>
// inspired by http://www.winprog.org/tutorial/simple_window.htm
const char g_szClassName[] = "myWindowClass";// a window class is not a c++ class and cannot have a main

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam); // the window only does what DefWindowProc tells it to do
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclassex; // part of registering our window class
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wndclassex.cbSize        = sizeof(WNDCLASSEX);
    wndclassex.style         = 0;
    wndclassex.lpfnWndProc   = WndProc;
    wndclassex.cbClsExtra    = 0;
    wndclassex.cbWndExtra    = 0;
    wndclassex.hInstance     = hInstance;
    wndclassex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclassex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclassex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclassex.lpszMenuName  = NULL;
    wndclassex.lpszClassName = g_szClassName;
    wndclassex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wndclassex))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            g_szClassName,
            "The title of my window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
            NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);// show the window
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }


    return Msg.wParam;
}

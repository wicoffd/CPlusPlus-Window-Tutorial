#include <windows.h>
#include <wingdi.h>
#include <uxtheme.h>

// inspired by http://www.winprog.org/tutorial/simple_window.htm
const char g_szClassName[] = "myWindowClass";// a window class is not a c++ class and cannot have a main

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT paintstruct;
    HDC hdc;
    RECT rc;
    HBRUSH hbrWhite;
    HBRUSH hbrGray;
    switch(msg)
    {
        case WM_LBUTTONDOWN: // show the user what the name of our program is when they click on our window
        // this code does not stand on it's own, it can't just be slapped into our code any old place
        { // curly braces are required when declaring variables inside a switch() statement
            char szFileName[MAX_PATH];// MAX_PATH is a handy macro included via <windows.h> that is defined to the maximum length of a buffer needed to store a filename under Win32
            HINSTANCE hInstance = GetModuleHandle(NULL); // HINSTANCE refers to the executable module (our program, the .exe file)
                                                                     // GetModuleHandle() gives us this
            GetModuleFileName(hInstance, szFileName, MAX_PATH); // null terminated string

            MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
            PostMessage(hwnd, WM_CLOSE, 0, 0);// closes the window use with a message box to delay close
            //Each windows message may have up to two parameters, wParam and lParam. Originally wParam was 16 bit and lParam was 32 bit,
            // but in Win32 they are both 32 bit. Not every message uses these parameters, and each message uses them differently. For
            // example the ///////WM_CLOSE message doesn't use either, and you should ignore them both.///// The WM_COMMAND message uses both,
            // wParam contains two values, HIWORD(wParam) is the notification message (if applicable) and LOWORD(wParam) is the control
            // or menu id that sent the message. lParam is the HWND (window handle) to the control which sent the message or NULL
            // if the messages isn't from a control. HIWORD() and LOWORD() are macros defined by windows that single out the two
            // high bytes (High Word) of a 32 bit value (0xFFFF0000) and the low word (0x0000FFFF) respectively. In Win32 a WORD
            // is a 16bit value, making DWORD (or Double Word) a 32bit value.
            // Messages
            // PostMessage() puts the message into the Message Queue and returns immediatly.
            // SendMessage() sends the message directly to the window and does not return until the window has finished processing it.

            //First part of third argument decides the button message
        }        break;               //show the user what the name of our program is when they click on our window
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT: // https://learn.microsoft.com/en-us/windows/win32/gdi/using-the-wm-paint-message
            hdc = BeginPaint(hwnd, &paintstruct);
            TextOut(hdc, 0, 0, " This is a WM_PAINT message!", 29); // c is the length of the string
            EndPaint(hwnd, &paintstruct);
            return 0L;
            break;
        case WM_CREATE:
            hbrWhite = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
            hbrGray  = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
            return 0L;

        case WM_ERASEBKGND: // If you set the class background brush to NULL, however,
                            // the system sends a WM_ERASEBKGND message to your window
                            // procedure whenever the window background must be drawn,
                            // letting you draw a custom background.
            hdc = (HDC) wParam;
            GetClientRect(hwnd, &rc);
            SetMapMode(hdc, MM_ANISOTROPIC);
            SetWindowExtEx(hdc, 100, 100, NULL);
            SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
            FillRect(hdc, &rc, hbrWhite);

            for (int i = 0; i < 13; i++)
            {
                int x = (i * 40) % 100;
                int y = ((i * 40) / 100) * 20;
                SetRect(&rc, x, y, x + 20, y + 20);
                FillRect(hdc, &rc, hbrGray);
            }
            return 1L;
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

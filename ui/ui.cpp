#include <Windows.h>

class UI {
        LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    public:
        const char *g_szClassName = "UI_0_0";
        char *title;
        int width;
        int height;
        HFONT hFont;
        HWND hwnd;

        int load() {
            WNDCLASSEXA wc;
            MSG Msg;

            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = 0;
            wc.lpfnWndProc = WndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = GetModuleHandle(NULL);
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = g_szClassName;
            wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

            if (!RegisterClassExA(&wc))
            {
                MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
                ExitProcess(0);
            }

            hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, g_szClassName, "Synth4", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, wc.hInstance, NULL);

            if (hwnd == NULL)
            {
                MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
                return 0;
            }
            hFont = CreateFontA(15, 0, 0, 0, FW_DONTCARE, 0, 0, 0, 0, 0,
                CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "verdana");

            //button_init();

            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);

            while (GetMessage(&Msg, hwnd, 0, 0) > 0)
            {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }

            return 0;
        }
    private:
        LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            PAINTSTRUCT ps;

            switch (msg)
            {
            case WM_PAINT:
                BeginPaint(hWnd, &ps);
                Paint(hWnd, &ps);
                EndPaint(hWnd, &ps);
                break;
            case WM_CLOSE:
                //synth_off();
                //keyboard_stop();
                DestroyWindow(hWnd);
                ExitProcess(0);
                break;
            default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
            return 0;
        }
};
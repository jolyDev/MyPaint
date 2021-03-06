// MyPaint_v2.1.0.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyPaint_v2.1.0.h"
#include "MenuManager.h"
#include "Painter.h"
#include "misc.h"

#define MAX_LOADSTRING 100

const int window_width = 1500;
const int window_height = 1500;

const int image_width = 1500;
const int image_height = 1500;

void _UpdateWindow(HWND, HDC *, Painter *);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPAINTV210, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINTV210));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINTV210));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPAINTV210);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// initialize requireable components

	static MenuManager * mHandler;
	static Painter * painter;

	static HBRUSH hBackgroundBrush;
	static HDC hImageDC;
	static HBITMAP hImageBitmap, hOldImageBitmap;
	static bool left_button_down = true;


    switch (message)
    {
	case WM_CREATE:

		hImageDC = CreateCompatibleDC(GetDC(hWnd));
		hImageBitmap = CreateCompatibleBitmap(GetDC(hWnd), image_width, window_height);
		hOldImageBitmap = (HBITMAP)SelectObject(hImageDC, hImageBitmap);
		PatBlt(hImageDC, 0, 0, image_width, window_height, WHITENESS);
		ReleaseDC(hWnd, GetDC(hWnd));

		mHandler = new MenuManager(hWnd);
		mHandler->addMenus();
		painter  = new Painter(hWnd, &hImageDC);

		break;

	case WM_PAINT:
		_UpdateWindow(hWnd, &hImageDC, painter);
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId) {
			case MII_RED:
			case MII_BLACK:
			case MII_GREEN:
			case MII_BLUE:
			case MII_YELLOW:
			case MII_PINK:
			case MII_DEFAULT:
				painter->setColor(wmId);
				break;
			case MII_TINY:
			case MII_THIN:
			case MII_STANDART:
			case MII_MIDDLE:
			case MII_BIG: 
			case MII_HUGE:
				painter->setLineWidth(wmId);
				break;
			case MII_CURVE:
			case MII_LINE:
			case MII_RECTANGLE:
			case MII_ELIPS:
				painter->setFigureToDraw(wmId);
				break;
			case MII_SAVE:
				// misc::HDCToFile("standart.jpg", hImageDC, RECT());
				MenuManager::savePicture(hWnd);
				break;
			case MII_EXIT:
            case IDM_EXIT:
				if (1 == MessageBox(hWnd, L"Are You sure you want to close the window\nall unsaved info will be unavailable to restore", L"Warning!", MB_OKCANCEL))
					DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_MOUSEMOVE:
		mHandler->UpdateCursorCoordinates();
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_SIZE:
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONDOWN:
	{
		painter->allowDrawing(true); // only for curve
		painter->renew_X_Y_pos();
		left_button_down = true;
	}
	break;

	case WM_LBUTTONUP:
	{
		painter->Draw(&hImageDC);
		painter->allowDrawing(false); // only for curve
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;
    case WM_DESTROY:		
		SelectObject(hImageDC, hOldImageBitmap);
		DeleteDC(hImageDC);
		DeleteObject(hImageBitmap);
		PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void _UpdateWindow(HWND hWnd, HDC * hImageDC, Painter * painter) {
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, window_width, window_height);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	PatBlt(hMemDC, 0, 0, window_width, window_height, WHITENESS);
	SetBkMode(hMemDC, TRANSPARENT);

	BitBlt(hMemDC, 0, 0, image_width, image_height, *hImageDC, 0, 0, SRCCOPY);

	/*!!!*/
	painter->Draw(&hMemDC);
	/*!!!*/

	BitBlt(hDC, 0, 0, window_width, window_height, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	EndPaint(hWnd, &ps);
}
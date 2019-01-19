#include "stdafx.h"
#include "MenuManager.h"
#include <string>
#include <Windows.h>
#include <windows.h>
#include <Commdlg.h>
#include <iostream>
#include <fstream>


MenuManager::MenuManager(HWND parentWindow)
{
	addMenus();
	initDial();
	hWnd = parentWindow;
}


void MenuManager::initDial() {
	dial = CreateWindowW(L"static", L"0:0", WS_VISIBLE | WS_CHILD, 0, 0, 100, 20, hWnd, NULL, NULL, NULL);
}

void MenuManager::UpdateCursorCoordinates() {
	POINT mouse;
	char istr[5];
	char istr2[5];
	std::string str_dial;

	if (GetCursorPos(&mouse)) {

		_ltoa_s(mouse.x, istr, 10);
		_ltoa_s(mouse.y, istr2, 10);

		str_dial += istr;
		str_dial += " : ";
		str_dial += istr2;

		SetWindowTextA(dial, (str_dial).c_str());
	}
}

void MenuManager::addMenus() {

	initDial();

	HMENU hMenu = CreateMenu();
	HMENU hDrawMenu = CreateMenu();
	HMENU hSizeMenu = CreateMenu();
	HMENU hColorMenu = CreateMenu();

	AppendMenu(hColorMenu, MF_STRING, MII_RED, L"Red");
	AppendMenu(hColorMenu, MF_STRING, MII_BLACK,  L"Black");
	AppendMenu(hColorMenu, MF_STRING, MII_GREEN,  L"Green");
	AppendMenu(hColorMenu, MF_STRING, MII_BLUE, L"Blue");
	AppendMenu(hColorMenu, MF_STRING, MII_YELLOW, L"Yellow");
	AppendMenu(hColorMenu, MF_STRING, MII_PINK, L"Pink");
	AppendMenu(hColorMenu, MF_STRING, MII_DEFAULT, L"default");

	AppendMenu(hSizeMenu, MF_STRING, MII_TINY, L"Tiny");
	AppendMenu(hSizeMenu, MF_STRING, MII_THIN, L"Thin");
	AppendMenu(hSizeMenu, MF_STRING, MII_STANDART, L"Standart");
	AppendMenu(hSizeMenu, MF_STRING, MII_MIDDLE, L"Middle");
	AppendMenu(hSizeMenu, MF_STRING, MII_BIG, L"Big");
	AppendMenu(hSizeMenu, MF_STRING, MII_HUGE, L"Huge");

	AppendMenu(hDrawMenu, MF_STRING, MII_CURVE, L"Curve");
	AppendMenu(hDrawMenu, MF_STRING, MII_LINE, L"Line");
	AppendMenu(hDrawMenu, MF_STRING, MII_RECTANGLE, L"Rectangle");
	AppendMenu(hDrawMenu, MF_STRING, MII_ELIPS, L"Elips");

	/////////////////////////////////////////////////////

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hDrawMenu, L"Draw");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hColorMenu, L"Colors");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSizeMenu, L"Brush Size");
	AppendMenu(hMenu, MF_STRING, MII_SAVE, L"Save");
	AppendMenu(hMenu, MF_STRING, MII_EXIT, L"Exit");

	SetMenu(hWnd, hMenu);
}

void MenuManager::saveBitmap(HWND window, LPCWSTR filePath) {
	int im_width = 1280;
	int im_heith = 1024;

	COLORREF** colref = new COLORREF*[im_width];

	for (int j = 0; j < im_width; j++)
		colref[j] = new COLORREF[im_heith];


	HDC hdc = GetDC(window);
	HDC hdcBit = CreateCompatibleDC(hdc);
	HBITMAP hBit = CreateCompatibleBitmap(hdc, im_width, im_heith);
	SelectObject(hdcBit, hBit);
	BitBlt(hdcBit, 0, 0, im_width, im_heith, hdc, 0, 0, SRCCOPY);

	DWORD* byteImage = new DWORD[im_width * im_heith];

	BITMAPINFO* bmInfo = new BITMAPINFO();

	bmInfo->bmiHeader.biWidth = im_width;
	bmInfo->bmiHeader.biHeight = im_heith;
	bmInfo->bmiHeader.biPlanes = 1;
	bmInfo->bmiHeader.biCompression = BI_RGB;
	bmInfo->bmiHeader.biBitCount = 24;
	bmInfo->bmiHeader.biSize = sizeof(bmInfo->bmiHeader);

	GetDIBits(hdcBit, hBit, 0, im_width, byteImage, bmInfo, DIB_RGB_COLORS);

	BITMAPFILEHEADER* bmfh = new BITMAPFILEHEADER();

	bmfh->bfType = 0x4d42;
	bmfh->bfReserved1 = 0;
	bmfh->bfReserved2 = 0;
	bmfh->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + im_width * im_heith;

	DWORD m;
	HANDLE hfile = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hfile, (char*)bmfh, sizeof(BITMAPFILEHEADER), &m, NULL);
	WriteFile(hfile, (char*)bmInfo, sizeof(BITMAPINFO), &m, NULL);
	WriteFile(hfile, (char*)byteImage, im_heith * im_width * 4, &m, NULL);

	CloseHandle(hfile);

	DeleteDC(hdc);

	delete byteImage;
	delete bmInfo;
	delete[] colref;
}

void MenuManager::savePicture(HWND hWnd) {
	OPENFILENAME ofn;

	char szFileName[MAX_PATH] = "\0";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Text Files (*.bpm)\0*.bpm\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"bpm";

	GetSaveFileName(&ofn);

	saveBitmap(hWnd, ofn.lpstrFile);
}

MenuManager::~MenuManager()
{
}

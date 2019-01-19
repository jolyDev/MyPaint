#pragma once

enum MenuItemsId { MII_CURVE, MII_DRAW, MII_LINE, MII_ELIPS, MII_RECTANGLE, MII_SAVE, MII_EXIT,
					MII_RED, MII_BLACK, MII_GREEN, MII_BLUE, MII_YELLOW, MII_PINK, MII_DEFAULT,
					MII_TINY, MII_THIN, MII_STANDART, MII_MIDDLE, MII_BIG, MII_HUGE
};

class MenuManager
{
private:

	HWND dial;
	HWND hWnd;

	void initDial();

public:
	MenuManager(HWND);

	void addMenus();

	void UpdateCursorCoordinates();

	static void saveBitmap(HWND, LPCWSTR);

	static void savePicture(HWND);

	~MenuManager();
};


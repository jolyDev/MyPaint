#include "stdafx.h"
#include "Painter.h"
#include "misc.h"

Painter::Painter(HWND hWnd, HDC * hImageDC)
{
	this->hWnd = hWnd;
	this->typeToDraw = FTD_CURVE;
	this->hDC = hImageDC;

	curr_x = curr_y = 0;
	x_prev = y_prev = 0;

	x_curve_prev = y_curve_prev = 0;

	Color = RGB(44, 89, 75);
	width = 3;
}

Painter::~Painter()
{
}

void Painter::setFigureToDraw(int code) {
	this->typeToDraw = code;
}

void Painter::renew_X_Y_pos() {
	POINT cursor;
	GetCursorPos(&cursor);

	x_prev_prev = x_prev;
	x_prev = curr_x;
	curr_x = (int)cursor.x;

	y_prev_prev = y_prev;
	y_prev = curr_y;
	curr_y = (int)cursor.y;

	x_curve_prev = cursor.x;
	y_curve_prev = cursor.y;

}

void Painter::Draw(HDC * surface) {
	if (isPaint)
		if (typeToDraw == MII_CURVE)				this->DrawCurve(surface);
		else if (typeToDraw == MII_RECTANGLE)		this->DrawRectangle(surface);
		else if (typeToDraw == MII_ELIPS)			this->DrawEclipse(surface);
		else if (typeToDraw == MII_LINE)			this->DrawLine(surface);
		else MessageBeep(MB_ERR_INVALID_CHARS);
}

void Painter::allowDrawing(bool allowDrawing) {
	this->isPaint = allowDrawing;
}

void Painter::DrawCurve(HDC * surface) {
	if (GetCursorPos(&mouse)) {
		DrawLine(hDC, x_curve_prev, y_curve_prev, mouse.x, mouse.y);
		x_curve_prev = mouse.x;
		y_curve_prev = mouse.y;
		// drawPoint(*hDC, mouse.x, mouse.y);
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

void Painter::drawPoint(HDC hDC, int x, int y) {
	for (int i = x - width; i <= x + width; i++)
		for (int j = y - width; j <= y + width; j++)
			SetPixel(hDC, i, j - delta_y, Color);
}

void Painter::DrawLine(HDC * surface , int x_begin, int y_begin, int x_end, int y_end ) {
	misc::bhm_line(this, surface, x_begin, y_begin, x_end, y_end, width);
}

void Painter::DrawLine(HDC * surface) {
	/* I am using Bresenham’s Line Drawing Algorithm*/

	GetCursorPos(&mouse);

	DrawLine(surface, curr_x, curr_y, mouse.x, mouse.y);
}


void Painter::DrawEclipse(HDC * surface) {
	int y = 0;		int a = 0;
	int x = 0;		int b = 0;

	if (GetCursorPos(&mouse)) {
		a = mouse.y - curr_y;
		b = mouse.x - curr_x;

		int r = sqrt(a ^ 2 + b ^ 2);
		if (a == 0 || b == 0)
			drawPoint(*surface, mouse.x, mouse.y);
		else
			for (int theta = 0; theta < 361; theta++) {
				x = curr_x + a * cos(theta);
				y = curr_y + b * sin(theta);
				drawPoint(*surface, x, y);
			}
	}
}


void Painter::setLineWidth(int enumId) {

	if (enumId == MII_TINY) width = 1;
	else if (enumId == MII_THIN) width = 2;
	else if (enumId == MII_MIDDLE) width = 4;
	else if (enumId == MII_BIG) width = 6;
	else if (enumId == MII_HUGE) width = 8;
	else width = 3;
}

void Painter::setColor(int enumId) {

	if (enumId == MII_RED) Color = RGB(255, 0, 0);
	else if (enumId == MII_BLACK) Color = RGB(0,0,0);
	else if (enumId == MII_GREEN) Color = RGB(0, 255, 0);
	else if (enumId == MII_BLUE) Color = RGB(0,0,255);
	else if (enumId == MII_YELLOW) Color = RGB(255,255,0);
	else if (enumId == MII_PINK) Color = RGB(255, 0, 127);
	else Color = RGB(44, 89, 75);
}

void Painter::DrawRectangle(HDC * surface) {
	// i had trouble with lib, so i decided implement by myself
	if (GetCursorPos(&mouse)) {
		DrawLine(surface, curr_x, curr_y - delta_y, mouse.x - width, curr_y - delta_y);
		DrawLine(surface, mouse.x, curr_y - delta_y, mouse.x, mouse.y - width);
		DrawLine(surface, curr_x, mouse.y, mouse.x - width, mouse.y);
		DrawLine(surface, curr_x, mouse.y - width, curr_x, curr_y - delta_y);
	}
}
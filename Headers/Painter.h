#pragma once


#include "MenuManager.h"

#include <iostream>
#include <fstream>

enum FiguresToDraw { FTD_CURVE, FTD_LINE, FTD_ELIPS, FTD_RECTANGLE };

class Painter
{

private:

	HDC * hDC;

	HWND hWnd;

	long Color;

	int width;

	POINT mouse;

	bool isPaint;

	int typeToDraw;
	int brush_width;

	static const int delta_y = 43;

	int x_curve_prev;
	int y_curve_prev;

	int curr_x;
	int curr_y;

	int x_prev;
	int y_prev;

	int x_prev_prev;
	int y_prev_prev;

	void DrawCurve(HDC *);

	void DrawRectangle(HDC *);

	void DrawEclipse(HDC *);

	void DrawLine(HDC *);

	void DrawLine(HDC *, int, int, int, int);

public:

	void drawPoint(HDC, int, int);

	Painter(HWND, HDC *);

	void Draw(HDC *);

	void allowDrawing(bool);

	void setFigureToDraw(int);

	void renew_X_Y_pos();

	void setColor(int);

	void setLineWidth(int);

	~Painter();

};


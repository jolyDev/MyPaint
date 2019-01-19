#pragma once

#include "Painter.h"

class misc
{
public:
	misc();

	static bool HDCToFile(const char* , HDC, RECT, uint16_t BitsPerPixel = 24);

	static void bhm_line(Painter *, HDC *, int, int, int, int, int);

	~misc();
};


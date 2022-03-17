#include "Headers.h"

#ifndef _BACKGROUND_LIB  // флаг подключения
#define _BACKGROUND_LIB

class Background{
public:
	float x;
	float y;
	float valX;
	float valY;
	float dirX;
	float dirY;

	int width;
	int height;

	ALLEGRO_BITMAP *image;
};
int Width = -1, Height = -1;

void InitBackgroundSize(int w, int h);
void InitBackground(Background &back, float x, float y, float valX, float valY, float dirX, float dirY, int width, int height, ALLEGRO_BITMAP *image);
void UpdateBackground(Background &back);
void DrawBackground(Background &back, bool control);

void InitBackgroundSize(int w, int h)
{
	Width = w; Height = h;
}
void InitBackground(Background &back, float x, float y, float valX, float valY, float dirX, float dirY, int width, int height, ALLEGRO_BITMAP *image)
{
	back.x = x;
	back.y = y;
	back.valX = valX;
	back.valY = valY;
	back.dirX = dirX;
	back.dirY = dirY;
	back.width = width;
	back.height = height;
	back.image = image;
}
void UpdateBackground(Background &back)
{
	back.x += back.valX * back.dirX;
	back.y += back.valY * back.dirY;
	if(back.x > 0) 
	{
		int x1 = back.x;

		back.x -= x1;

		x1 = x1 % back.width;

		back.x += x1;
		back.x -= back.width;
	}
	if(back.x + back.width <= 0)
	{
		int x1 = back.x;

		back.x -= x1;

		x1 = x1 % back.width;

		back.x += x1;
	}
	if(back.y > 0) 
	{
		int y1 = back.y;

		back.y -= y1;

		y1 = y1 % back.height;

		back.y += y1;
		back.y -= back.height;
	}
	if(back.y + back.height <= 0)
	{
		int y1 = back.y;

		back.y -= y1;

		y1 = y1 % back.height;

		back.y += y1;
	}
}
void DrawBackground(Background &back, bool control)
{
	if(Width == -1 || Height == -1) return;
	for(int p = back.x; p < Width; p += back.width)
		for(int i = back.y; i < Height; i += back.height)
		{
			al_draw_bitmap(back.image, p, i, 0);
			if(control) al_draw_rectangle(p, i, p + back.width, i + back.height, al_map_rgba(96, 0, 191, 128), 2);
		}
}

#endif
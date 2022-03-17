#include "Headers.h"
#ifndef _EGORCOLOR_LIB  // флаг подключения
#define _EGORCOLOR_LIB

enum colors {BLACK, WHITE, RED, BLUE, GREEN, YELLOW, ORANGE, GREY, LIGHTBLUE, PURPLE, BROWN, BLASTEROIDES, ALPHA};
ALLEGRO_COLOR colors[255];
void init_colors()
{
	colors[WHITE] = al_map_rgb(255, 255, 255);
	colors[BLACK] = al_map_rgb(0, 0, 0);
	colors[RED] = al_map_rgb(255, 0, 0);
	colors[BLUE] = al_map_rgb(0, 0, 255);
	colors[GREEN] = al_map_rgb(0, 255, 0);
	colors[YELLOW] = al_map_rgb(255, 255, 0);
	colors[GREY] = al_map_rgb(127, 127, 127);
	colors[ALPHA] = al_map_rgba(0, 0, 0, 0);
	colors[ORANGE] = al_map_rgb(255, 127, 39);
	colors[LIGHTBLUE] = al_map_rgb(0, 170, 255);
	colors[PURPLE] = al_map_rgb(96, 0, 191);
	colors[BROWN] = al_map_rgb(140, 82, 53);
	colors[BLASTEROIDES] = al_map_rgb(186, 201, 50);
}
#endif
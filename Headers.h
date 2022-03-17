#define ALLEGRO_STATICLINK
#define _CRT_SECURE_NO_WARNINGS

#ifndef _HEADERS_LIB  // флаг подключения
#define _HEADERS_LIB

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int W = 800,H = 600;

#define MAX(a, b) (((a) > (b)) ? (a): (b))
#define MIN(a, b) (((a) < (b)) ? (a): (b))
#define WIDTH 800 
#define HEIGHT 600 
#define FPS 60.0
#define SECOND (FPS)
#define MINUTE (SECOND*60)
#define HOUR (MINUTE*60)
#define DAY (HOUR*24)
#define YEAR (DAY*365)
#define FROMRADTOGRAD (180.0/3.1415926)
//#define KOLKNOP 50

#endif
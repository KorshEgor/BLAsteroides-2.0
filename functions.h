#include "Headers.h"

#ifndef _FUNCTIONS_LIB  // флаг подключения
#define _FUNCTIONS_LIB

void abort_example(char const *format, ...);
void message(char const *format, ...);
float lenFrom1to2 (float x1, float y1, float x2, float y2);
int Collide_detect(int x1,int y1,int w1,int h1, int x2,int y2,int w2,int h2);
int Collide_detect(int x1, int y1, int w1, int h1, int r1, int x2, int y2, int w2, int h2, int r2);
int Collide_detect(float x1, float y1, float r1, float x2, float y2, float r2);
int Collide_detect(int x1, int y1, int w1, int h1, int x2, int y2, int r2);
float AngleToTarget(double x1, double y1, double x2, double y2);
ALLEGRO_COLOR Spector(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2, float otn);
int random(int min, int max);
bool chanse(float procent, int tochn);

void draw_text(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, ALLEGRO_COLOR color, int dx, int dy, int flag, char *text);
void draw_textf(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, ALLEGRO_COLOR color, int dx, int dy, int flag,  char *text2,  ...);
void get_textElcoord(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, char *text, int dx, int dy, int elNum, int &retx, int &rety);
void get_textMax(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, char *text, int dx, int dy, int &retx, int &rety);
void get_textfMax(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, int dx, int dy, int &retx, int &rety, char *text2, ...);
char get_rusChar(char b);
char get_engChar(char b);

void create_labirint_el(bool **right, bool **down, int w, int h);

bool keys[ALLEGRO_KEY_MAX];
bool LastKeyKeyboad[ALLEGRO_KEY_MAX];
//enum KEYS{DOWN, LEFT, RIGHT, UP, SPACE, ESCAPE, KEY_Q, KEY_E, KEY_1, KEY_2, KEY_R};
enum MKEYS{MLEFT,MRIGHT,MCENTRE};
enum REJIM{SQUARE, CIRCLE};

class mouse
{
public:
	float x, y, z;
	bool lastkey[5];
	mouse(){for(int p = 0; p < 5; p++){keys[p] = false; lastkey[p] = false;} x = WIDTH; y = HEIGHT;z = 0;}
	bool keys[5];
	ALLEGRO_COLOR color;
};

mouse cursor;

void abort_example(char const *format, ...)
{
   char str[1024];
   va_list args;
   ALLEGRO_DISPLAY *display;

   va_start(args, format);
   vsnprintf(str, sizeof str, format, args);
   va_end(args);

   if (al_init_native_dialog_addon()) {
      display = al_is_system_installed() ? al_get_current_display() : NULL;
      al_show_native_message_box(display, "Error", "Cannot run example", str, NULL, 0);
   }
   else {
      fprintf(stderr, "%s", str);
   }
   exit(1);
}
void message(char const *format, ...)
{
   char str[1024];
   va_list args;
   ALLEGRO_DISPLAY *display;

   va_start(args, format);
   vsnprintf(str, sizeof str, format, args);
   va_end(args);

   if (al_init_native_dialog_addon()) {
      display = al_is_system_installed() ? al_get_current_display() : NULL;
      al_show_native_message_box(display, "Warning", "", str, NULL, 0);
   }
   else {
      fprintf(stderr, "%s", str);
   }
}

int Collide_detect(int x1,int y1,int w1,int h1, int x2,int y2,int w2,int h2)
{
	int xmax1 = x1 + w1, ymax1 = y1 + h1;
	int xmax2 = x2 + w2, ymax2 = y2 + h2;
	int xmin = MAX(x1,x2);
	int ymin = MAX(y1,y2);
	int xmax = MIN(xmax1, xmax2);
	int ymax = MIN(ymax1, ymax2);
	if (xmax <= xmin || ymax <= ymin)
		return 0;
	return 1;
}
int Collide_detect(int x1, int y1, int w1, int h1, int r1, int x2, int y2, int w2, int h2, int r2)
{
	int xmax1 = x1 + w1, ymax1 = y1 + h1;
	int xmax2 = x2 + w2, ymax2 = y2 + h2;
	int xmin = MAX(x1,x2);
	int ymin = MAX(y1,y2);
	int xmax = MIN(xmax1, xmax2);
	int ymax = MIN(ymax1, ymax2);
	if (xmax <= xmin || ymax <= ymin)
		return 0;
	if(r1 + r2 >= lenFrom1to2(x1+w1/2, y1+h1/2, x2+w2/2, y2+h2/2)) 
		return 1;

	return 0;
}
int Collide_detect(float x1, float y1, float r1, float x2, float y2, float r2)
{
	if(r1 + r2 >= lenFrom1to2(x1, y1, x2, y2)) 
		return 1;
	return 0;
}
int Collide_detect(int x1, int y1, int w1, int h1, int x2, int y2, int r2)
{
	int xmax1 = x1 + w1, ymax1 = y1 + h1;
	int xmax2 = x2 + r2, ymax2 = y2 + 1;
	int xmin = MAX(x1,x2-r2);
	int ymin = MAX(y1,y2);
	int xmax = MIN(xmax1, xmax2);
	int ymax = MIN(ymax1, ymax2);
	if(xmax <= xmin || ymax <= ymin){
		xmin = MAX(x1,x2);
		ymin = MAX(y1,y2-r2);
		xmax2 = x2 + 1; ymax2 = y2 + r2;
		xmax = MIN(xmax1, xmax2);ymax = MIN(ymax1, ymax2);
		if(xmax <= xmin || ymax <= ymin){
			if(r2 < lenFrom1to2(x1, y1, x2, y2)){
				if(r2 < lenFrom1to2(x1+w1, y1, x2, y2)){
					if(r2 < lenFrom1to2(x1, y1+h1, x2, y2)){
						if(r2 < lenFrom1to2(x1+w1, y1+h1, x2, y2)){
							return 0;
						}
					}
				}
			}
		}
	}
	return 1;
}

float lenFrom1to2 (float x1, float y1, float x2, float y2)
{
	 return sqrt((x1 - x2)*(x1 - x2)+(y1 - y2)*(y1 - y2));
}

int random(int min, int max)
{
	if(min == max) return min;
	return rand() % (max-min+1) + min;
}
float AngleToTarget(double x1, double y1, double x2, double y2)
{
	float deltaX = x2 - x1;
	float deltaY = y2 - y1;
	return (atan2(deltaY, deltaX)) * FROMRADTOGRAD;
}
ALLEGRO_COLOR Spector(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2, float otn)
{
	ALLEGRO_COLOR ret;
	float a,r,g,b;

	a = color1.a;
	r = color1.r;
	g = color1.g;
	b = color1.b;

	a+=(color2.a - color1.a)*otn;
	r+=(color2.r - color1.r)*otn;
	g+=(color2.g - color1.g)*otn;
	b+=(color2.b - color1.b)*otn;

	ret = al_map_rgba_f(r,g,b,a);
	return ret;
}

char get_engChar(char b)
{
	char mass1[] = {'а','А','б','Б','в','В','г','Г','д','Д','е','Е','ё','Ё','ж','Ж','з','З','и','И','й','Й','к','К','л','Л','м','М','н','Н','о','О','п','П','р','Р','с','С','т',
				   'Т','у','У','ф','Ф','х','Х','ц','Ц','ч','Ч','ш','Ш','щ','Щ','ъ','Ъ','ы','Ы','ь','Ь','э','Э','ю','Ю','я','Я'};
	char mass2[] = {'f','F',',','<','d','D','u','U','l','L','t','T','`','~',';',':','p','P','b','B','q','Q','r','R','k','K','v','V','y','Y','j','J','g','G','h','H','c','C','n',
				   'N','e','E','a','A','[','{','w','W','x','X','i','I','o','O',']','}','s','S','m','M','\'','\"','.','>','z','Z'};
	int z = 0;
	for(z = 0; z < 66; z++)
	{
		if(b == mass1[z]) break;
	}
	if(z == 66) return b;
	return mass2[z];
}

char get_rusChar(char b)
{
	char mass1[] = {'a','А','б','Б','в','В','г','Г','д','Д','е','Е','ё','Ё','ж','Ж','з','З','и','И','й','Й','к','К','л','Л','м','М','н','Н','о','О','п','П','р','Р','с','С','т',
				   'Т','у','У','ф','Ф','х','Х','ц','Ц','ч','Ч','ш','Ш','щ','Щ','ъ','Ъ','ы','Ы','ь','Ь','э','Э','ю','Ю','я','Я'};
	char mass2[] = {'f','F',',','<','d','D','u','U','l','L','t','T','`','~',';',':','p','P','b','B','q','Q','r','R','k','K','v','V','y','Y','j','J','g','G','h','H','c','C','n',
				   'N','e','E','a','A','[','{','w','W','x','X','i','I','o','O',']','}','s','S','m','M','\'','\"','.','>','z','Z'};
	int z = 0;
	for(z = 0; z < 66; z++)
	{
		if(b == mass2[z]) break;
	}
	if(z == 66) return b;
	return mass1[z];
}

void draw_text(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, ALLEGRO_COLOR color, int dx, int dy, int flag, char *text)
{
	if(flag == 2)
	{
		int mx, my;
		get_textMax(font, font_rus, text, 0, 0, mx, my);
		dx -= mx;
	}else if(flag == 1){
		int mx, my;
		get_textMax(font, font_rus, text, 0, 0, mx, my);
		dx -= mx/2;
	}
	ALLEGRO_FONT *dFont;
	int sx = dx, sy = dy;
	int p = 0;
	char dC;
	for(p = 0; text[p] != '\0'; p++)
	{
		if( (text[p] >= -64 && text[p] <= -1) || (text[p] == -72) || (text[p] == -88))
		{
			dFont = font_rus;
			dC = get_engChar(text[p]);
		}
		else
		{
			dC = text[p];
			dFont = font;
		}
		if(dC == '\n')
		{
			sx = dx;
			sy += al_get_font_line_height(dFont);
			continue;
		}
		/*if(dC == '*')
		{
			continue;
		}*/

		al_draw_text(dFont, color, sx, sy, 0, &dC);
		sx += al_get_text_width(dFont, &dC);
	}
}
void draw_textf(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, ALLEGRO_COLOR color, int dx, int dy, int flag,  char *text2, ...)
{
	char text[1024];// = new char[1000000];
	va_list args;

	va_start(args, text2);
	vsnprintf(text, sizeof text, text2, args);
	va_end(args);

	if(flag == 2)
	{
		int mx, my;
		get_textMax(font, font_rus, text, 0, 0, mx, my);
		dx -= mx;
	}else if(flag == 1){
		int mx, my;
		get_textMax(font, font_rus, text, 0, 0, mx, my);
		dx -= mx/2;
	}

	ALLEGRO_FONT *dFont;
	int sx = dx, sy = dy;
	int p = 0;
	char dC;
	for(p = 0; text[p] != '\0' && p < 1024; p++)
	{
		if( (text[p] >= -64 && text[p] <= -1) || (text[p] == -72) || (text[p] == -88))
		{
			dFont = font_rus;
			dC = get_engChar(text[p]);
		}
		else
		{
			dC = text[p];
			dFont = font;
		}
		if(dC == '\n')
		{
			sx = dx;
			sy += al_get_font_line_height(dFont);
			continue;
		}
		/*if(dC == '*')
		{
			continue;
		}*/

		al_draw_text(dFont, color, sx, sy, 0, &dC);
		sx += al_get_text_width(dFont, &dC);
	}
}
void get_textElcoord(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, char *text, int dx, int dy, int elNum, int &retx, int &rety)
{
	ALLEGRO_FONT *dFont;
	int sx = dx, sy = dy;
	int p = 0;
	char dC;
	for(p = 0; p < elNum; p++)
	{
		if( (text[p] >= -64 && text[p] <= -1) || (text[p] == -72) || (text[p] == -88))
		{
			dFont = font_rus;
			dC = get_engChar(text[p]);
		}
		else
		{
			dC = text[p];
			dFont = font;
		}
		if(dC == '\n')
		{
			sx = dx;
			sy += al_get_font_line_height(dFont);
			continue;
		}
		if(dC == '*')
		{
			continue;
		}
		sx += al_get_text_width(dFont, &dC);
	}
	retx = sx; rety = sy;
}
void get_textMax(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, char *text, int dx, int dy, int &retx, int &rety)
{
	ALLEGRO_FONT *dFont = font;
	int sx = dx, sy = dy + al_get_font_line_height(dFont);;
	retx = dx; rety = dy;
	int p = 0;
	char dC;
	for(p = 0; text[p] != '\0'; p++)
	{
		if( (text[p] >= -64 && text[p] <= -1) || (text[p] == -72) || (text[p] == -88))
		{
			dFont = font_rus;
			dC = get_engChar(text[p]);
		}
		else
		{
			dC = text[p];
			dFont = font;
		}
		if(dC == '\n')
		{
			retx = MAX(sx,retx);
			sx = dx;
			sy += al_get_font_line_height(dFont);
			continue;
		}
		if(dC == '*')
		{
			continue;
		}
		sx += al_get_text_width(dFont, &dC);
	}
	retx = MAX(sx,retx);
	rety = sy;
}
void get_textfMax(ALLEGRO_FONT *font, ALLEGRO_FONT *font_rus, int dx, int dy, int &retx, int &rety, char *text2, ...)
{
	char text[1024];// = new char[1000000];
	va_list args;

	va_start(args, text2);
	vsnprintf(text, sizeof text, text2, args);
	va_end(args);

	ALLEGRO_FONT *dFont = font;
	int sx = dx, sy = dy + al_get_font_line_height(dFont);;
	retx = dx; rety = dy;
	int p = 0;
	char dC;
	for(p = 0; text[p] != '\0'; p++)
	{
		if( (text[p] >= -64 && text[p] <= -1) || (text[p] == -72) || (text[p] == -88))
		{
			dFont = font_rus;
			dC = get_engChar(text[p]);
		}
		else
		{
			dC = text[p];
			dFont = font;
		}
		if(dC == '\n')
		{
			retx = MAX(sx,retx);
			sx = dx;
			sy += al_get_font_line_height(dFont);
			continue;
		}
		if(dC == '*')
		{
			continue;
		}
		sx += al_get_text_width(dFont, &dC);
	}
	retx = MAX(sx,retx);
	rety = sy;
}

bool chanse(float procent, int tochn = 1)
{
	int step = 1;
	for(; tochn > 1; tochn--)
	{
		step*=10;
	}
	procent*=step;
	step*=100;
	int res = random(1, step);
	return res <= procent;
}

void create_labirint_el(bool **right, bool **down, int w, int h){
	int mass_num_p[100], *mass_num;
	int mass_kol_p[100], *mass_kol;
	if(w <= 100){
		mass_num = mass_num_p;
		mass_kol = mass_kol_p;
	}else{
		mass_num = new int[w];
		if(!mass_num) abort_example("Error add memory (lab El)");
		mass_kol = new int[w];
		if(!mass_kol) abort_example("Error add memory (lab El)");
	}

	for(int u = 0; u < w; ++u){
		mass_num[u] = -1;
		mass_kol[u] = 0;
	}
	for(int t = 0; t < h-1; ++t){
		for(int u = 0; u < w; ++u){
			if(mass_num[u] == -1)
				for(int p = 0; p < w; ++p)
					if(mass_kol[p] == 0){
						mass_num[u] = p;
						mass_kol[p] = 1;
						break;
					}
		}
		for(int u = 0; u < w-1; ++u){
			right[u][t] = (mass_num[u] == mass_num[u+1]) || (random(0,1));
			if(!right[u][t]){
				int isk = mass_num[u+1];
				mass_kol[mass_num[u]] += mass_kol[isk];
				mass_kol[isk] = 0;
				for(int p = 0; p < w; ++p)
					if(mass_num[p] == isk)
						mass_num[p] = mass_num[u];
			}
		}
		right[w-1][t] = true;
		for(int u = 0; u < w; ++u){
			down[u][t] = (mass_kol[mass_num[u]]-1) && (random(0,1));
			if(down[u][t]){
				mass_kol[mass_num[u]]--;
				mass_num[u] = -1;
			}
		}
	}

	if(mass_num[0] == -1) 
		for(int p = 0; p < w; ++p)
			if(mass_kol[p] == 0){
				mass_num[0] = p;
				break;
			} 
	for(int u = 0; u < w-1; ++u){
		right[u][h-1] = (mass_num[u] == mass_num[u+1]);
		if(!right[u][h-1]){
			if(mass_num[u+1] != -1){
				int isk = mass_num[u+1];
				for(int p = u+1; p < w; ++p)
					if(mass_num[p] == isk)
						mass_num[p] = mass_num[u];
			}else{
				mass_num[u+1] = mass_num[u];
			}
		}
		down[u][h-1] = true;
	}

	if(w > 100){
		delete mass_num;
		delete mass_kol;
	}
}
#endif
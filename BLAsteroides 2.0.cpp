#include "Headers.h"
#include "functions.h"
#include "Knopki 2.0.h"
#include "EgorColor.h"
#include "Background.h"
#include "MyGeometry.h"
#include "Space.h"

#define KOLSAMPLESMUSIC 5 
#define KOLSAMPLESEFFECTS 5

#define KOLSAMPLES (KOLSAMPLESMUSIC + KOLSAMPLESEFFECTS)

#define SIZESTREL 30

#define KOLMENU 4
#define KOLKNOPMENU 5
#define KOLKNOPOPT 6
#define KOLKNOPPLAYOPT 3
#define KOLKNOPREC 2

#define KOLKNOP (KOLKNOPMENU + KOLKNOPOPT + KOLKNOPPLAYOPT + KOLKNOPREC)

void exit_game();
void choice_gameMode();
void play();
void options();
void recordes();
void music_next();
void ship_next();
void music_last();
void ship_last();
void music_fix();
void soundEff_fix();
void time_fixX();
void back();
void ship_next_rec();
void ship_last_rec();

void set_playMode(int m);
void UpdateMuisc();

void load_flip();
void load_flip_reset(int kol);

void Bonus_Heal();
void Bonus_Shield();
void Bonus_Money();
void Bonus_Power();

Bonus Bonus_act;

FILE *fIn = NULL;
int records[KOLSHIPS][11];

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT *FontRu25, *FontEn25, *FontRu30, *FontEn30;

ALLEGRO_SAMPLE *samples[KOLSAMPLES];
ALLEGRO_SAMPLE_INSTANCE *instances[KOLSAMPLES];

int score = 0;

bool done = false;
bool F1 = false;
bool F2 = false;
float kolNow = -1, kolMax = -1;
float ship_respawn = 0;
void (*Bonus_functions[KOLBONUS])() = {
	Bonus_Heal, Bonus_Shield, Bonus_Money, Bonus_Power,
};

enum REJIMIGRI{PLAY,MENU,PAUSE,OPTIONS,RECORDS,ENDGAME,PLAYOPT};

enum SAMPLES{
	DESTR, TIME, INERTIA, MERCURY, ZANT,
	CLICK, BLASTEX, ASTEREX, SHIPEX, BLAST,
};
char *NameMusic[KOLSAMPLESMUSIC] = {
	"Destroyer",
	"Go_through_the_time",
	"Inertia",
	"Mercury",
	"Z-antidote",
};
char *Sample_file_name[KOLSAMPLES] = {
	"Destroyer.ogg",
	"Go_through_the_time.ogg",
	"Inertia.ogg",
	"Mercury.ogg",
	"Z-antidote.ogg",
	"Click_On.ogg",
	"Blast.wav",
	"Bomb.wav",
	"Exploding.wav",
	"Laser_Blasts.wav",
};

enum MYKNOPKI{
	KNOPKA_PLAY, KNOPKA_OPT, KNOPKA_REC, KNOPKA_QUIT, KNOPKA_CONTINUE,
	KNOPKA_LAST, KNOPKA_NEXT, KNOPKA_BEGYN_TIME, KNOPKA_BEGYN_MUSIC, KNOPKA_BEGYN_SOUND_EFF, KNOPKA_BACK,
	KNOPKA_LAST_SHIP, KNOPKA_NEXT_SHIP, KNOPKA_START,
	KNOPKA_LAST_SHIP_REC, KNOPKA_NEXT_SHIP_REC,
};

int iRejIg = -1, lastPlayMode = -1;
tochka back_coord;
float gain_music = 0.5, gain_soundEff = 0.75, time_music = 0, knopka_music_fixX = -1000,
	knopka_soundEff_fixX = -1000, knopka_time_fixX = -1000;
int backGroundMusic = -1;

char *menu_texts[KOLKNOP] = {
	"Играть", "Настройки", "Рекорды", "Выход", "Продолжить",
	"Пред.", "След.", "100", "100", "100", "Назад",
	"", "", "Старт!",
	"", "",
};
void (*functions[KOLKNOP])() = {
	choice_gameMode, options, recordes, exit_game, play,
	music_last, music_next, time_fixX, music_fix, soundEff_fix, back, 
	ship_last, ship_next, play,
	ship_last_rec, ship_next_rec,
};
int menu_starts[KOLMENU][2] = {
	{W/2,H/2-70},
	{W/2,H/2-175},
	{W/2,H/2-105},
	{W/2,H/2-105},
};
int menu_changes[KOLMENU][2] = {
	{0,35},
	{0,35},
	{0,35},
	{0,35},
};
int kolKnopMenu[KOLMENU] = {
	KOLKNOPMENU,
	KOLKNOPOPT,
	KOLKNOPPLAYOPT,
	KOLKNOPREC,
};
knopka *menu_knopMass[KOLKNOP];

long long int timeFlip = 0;

ALLEGRO_BITMAP *back_image;
ALLEGRO_BITMAP *gameBgImage;
Background gameBG;

int main()
{
	ALLEGRO_EVENT_QUEUE *event_queue = NULL; 
	ALLEGRO_EVENT ev;
	ALLEGRO_COLOR fon;
	ALLEGRO_TIMER *fps = NULL;
	ALLEGRO_MONITOR_INFO info;
	ALLEGRO_BITMAP *bitmap_collide, *bitmap_no_collide, *bitmap_block, *mini_map;
	ALLEGRO_BITMAP *bgimage, *mgimage, *fgimage;
	Background BG, MG, FG, gameFG;
	char file_name[1024];

	bool draw = false, naj = false;
	int r, g, b, a;
	r = g = b = a = 0;

	// проверка аллегро
	if(!al_init())
		abort_example("Error init");
	// проверка дисплея
	al_get_monitor_info(0, &info);
	W = info.x2 - info.x1;
	H = info.y2 - info.y1;
	display = al_create_display(W,H);
	if(!display)
		abort_example("Error creating display");

	al_set_window_position(display,0,0);
	al_set_window_title(display,"Бластеройды 2.0");
	al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
	
	// installs и inits
	al_install_mouse(); 
	al_install_keyboard();
	al_install_audio();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();
	init_colors();

	al_reserve_samples(1);

	//loads
	al_clear_to_color(colors[BLACK]);
	al_flip_display();

	FontEn12 = al_load_font("Resources/arial2.ttf",20,0);
	if(!FontEn12) abort_example("Error load 'Resources/arial2.ttf'");
	FontEn25 = al_load_font("Resources/arial2.ttf",25,0);
	if(!FontEn25) abort_example("Error load 'Resources/arial2.ttf'");
	FontRu25 = al_load_font("Resources/arial.ttf",25,0);
	if(!FontRu25) abort_example("Error load 'Resources/arial.ttf'");
	FontEn30 = al_load_font("Resources/arial2.ttf",30,0);
	if(!FontEn30) abort_example("Error load 'Resources/arial2.ttf'");
	FontRu30 = al_load_font("Resources/arial.ttf",30,0);
	if(!FontRu30) abort_example("Error load 'Resources/arial.ttf'");

	al_clear_to_color(colors[BLACK]);
	draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W, H - 30, ALLEGRO_ALIGN_RIGHT, "Загрузка файлов игры...");
	al_flip_display();

	for(int g = 0; g < KOLASTEROIDS-1; g++)
	{
		sprintf_s(&file_name[0], 1024, "Resources/Asteroid%d_polygon.txt", g+1);
		fIn = fopen(file_name, "r");
		if(!fIn) abort_example("Error load file '%s'", file_name);

		int size, sizeT;

		if(!fscanf(fIn, "%i", &size)) abort_example("Error read from file '%s'", file_name);
		Asteroid_col_polygon[g] = new tochka*[size];
		if(!Asteroid_col_polygon[g]) abort_example("Error new memory");
		Asteroid_col_polygon_size[g] = new int[size];
		if(!Asteroid_col_polygon_size[g]) abort_example("Error new memory");
		for(int u = 0; u < size; u++)
		{
			if(!fscanf(fIn, "%i", &sizeT)) abort_example("Error read from file '%s'", file_name);
			Asteroid_col_polygon[g][u] = new tochka[sizeT];
			if(!Asteroid_col_polygon[g][u]) abort_example("Error new memory");
			Asteroid_col_polygon_size[g][u] = sizeT;
			for(int p = 0; p < sizeT; p++)
			{
				float x1, y1;
				if(!fscanf(fIn, "%f", &x1)) abort_example("Error read from file '%s'", file_name);
				if(!fscanf(fIn, "%f", &y1)) abort_example("Error read from file '%s'", file_name);
				Asteroid_col_polygon[g][u][p] = tochka(x1,y1);
			}
		}

		fclose(fIn);
	}

	fIn = fopen("Resources/records.txt", "r");
	
	bool f = (fIn == NULL);

	if(!f)
	{
		for(int u = 0; u < KOLSHIPS; u++)
		{
			fscanf(fIn, "%i", &records[u][0]);
			int k = 1;
			while(!feof(fIn) && k < 10)
			{
				fscanf(fIn, "%i", &records[u][k]);
				if(records[u][k] > records[u][k-1]) break;
				k++;
			}
			if(!feof(fIn)) fscanf(fIn, "%i", &records[u][10]);
			if(k != 10 || (!feof(fIn) && u == KOLSHIPS-1))
			{
				f = true;
				break;
			}
		}
		if(!f)
		{
			for(int u = 0; u < KOLSHIPS; u++)
			{
				int control = 0;
				for(int k = 0; k < 10; k++)
				{
					control += records[u][k];
				}
				if(control*17+274 != records[u][10])
				{
					f = true;
					break;
				}
			}
		}
	}
	if(f)
	{
		fIn = fopen("Resources/records.txt", "w");
		for(int u = 0; u < KOLSHIPS; u++)
		{
			fprintf(fIn, "0");
			records[u][0] = 0;
			for(int i = 1; i < 10; i++)
			{
				fprintf(fIn, "\n0");
				records[u][i] = 0;
			}
			fprintf(fIn, "\n274");
			records[u][10] = 274;
			if(u < KOLSHIPS-1)
			{
				fprintf(fIn, "\n");
			}
		}
	}
	fclose(fIn);

	al_clear_to_color(colors[BLACK]);
	draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W, H - 30, ALLEGRO_ALIGN_RIGHT, "Загрузка картинок...");
	al_flip_display();

	for(int p = 0; p < KOLSHIPS; p++)
	{
		ship_bitmaps[p] = NULL;
	}

	ship_bitmaps[0] = al_load_bitmap("Resources/spaceship.png");
	if(!ship_bitmaps[0]) abort_example("Error load 'Resources/spaceship.png'");

	for(int g = 0; g < KOLASTEROIDS; g++)
	{
		sprintf_s(&file_name[0], 1024, "Resources/asteroid%d.png", g+1);
		Asteroid_bitmaps[g] = al_load_bitmap(file_name);
		if(!Asteroid_bitmaps[g]) abort_example("Error load '%s'", file_name);
		Asteroid_size[g][0] = al_get_bitmap_width(Asteroid_bitmaps[g]) / Asteroid_flips_in_bitmap[g][0];
		Asteroid_size[g][1] = al_get_bitmap_height(Asteroid_bitmaps[g]) / Asteroid_flips_in_bitmap[g][1];
	}
	for(int g = 0; g < KOLBONUS; g++)
	{
		sprintf_s(&file_name[0], 1024, "Resources/bonus%d.png", g+1);
		Bonus_bitmaps[g] = al_load_bitmap(file_name);
		if(!Bonus_bitmaps[g]) abort_example("Error load '%s'", file_name);
		al_convert_mask_to_alpha(Bonus_bitmaps[g], colors[BLACK]);
	}
	for(int g = 0; g < KOLEXPLOSION; g++)
	{
		sprintf_s(&file_name[0], 1024, "Resources/%s", Explosion_bitmaps_names[g]);
		Explosion_bitmaps[g] = al_load_bitmap(file_name);
		if(!Explosion_bitmaps[g]) abort_example("Error load '%s'", file_name);
		Explosion_size[g][0] = al_get_bitmap_width(Explosion_bitmaps[g]) / Explosion_flips_in_bitmap[g][0];
		Explosion_size[g][1] = al_get_bitmap_height(Explosion_bitmaps[g]) / Explosion_flips_in_bitmap[g][1];
	}
	for(int g = 0; g < KOLGALAXY; g++)
	{
		sprintf_s(&file_name[0], 1024, "Resources/galaxy%d.png", g+1);
		galaxy_bitmaps[g] = al_load_bitmap(file_name);
		if(!galaxy_bitmaps[g]) abort_example("Error load '%s'", file_name);
	}
	for(int g = 0; g < KOLPLANET; g++)
	{
		sprintf_s(&file_name[0], 1024, "Resources/planet%d.png", g+1);
		planet_bitmaps[g] = al_load_bitmap(file_name);
		if(!planet_bitmaps[g]) abort_example("Error load '%s'", file_name);
	}

	bgimage = al_load_bitmap("Resources/back0.png");
	if(!bgimage) abort_example("Error load bitmap Resources/back0.png");
	mgimage = al_load_bitmap("Resources/back1.png");
	if(!mgimage) abort_example("Error load bitmap Resources/back1.png");
	fgimage = al_load_bitmap("Resources/back2.png");
	if(!fgimage) abort_example("Error load bitmap Resources/back2.png");

	al_clear_to_color(colors[BLACK]);
	draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W, H - 65, ALLEGRO_ALIGN_RIGHT, "Загрузка музыки...");
	load_flip_reset(KOLSAMPLESMUSIC);
	for(int u = 0; u < KOLSAMPLESMUSIC; u++)
	{
		load_flip();
		sprintf_s(&file_name[0], 1024, "Resources/Sound/%s", Sample_file_name[u]);
		samples[u] = al_load_sample(file_name);
		if(!samples[u]) abort_example("Error load sample '%s'", file_name);
	}

	al_clear_to_color(colors[BLACK]);
	draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W, H - 65, ALLEGRO_ALIGN_RIGHT, "Загрузка звуковых эффектов...");
	load_flip_reset(KOLSAMPLESEFFECTS);
	for(int u = KOLSAMPLESMUSIC; u < KOLSAMPLES; u++)
	{
		load_flip();
		sprintf_s(&file_name[0], 1024, "Resources/Sound/%s", Sample_file_name[u]);
		samples[u] = al_load_sample(file_name);
		if(!samples[u]) abort_example("Error load sample '%s'", file_name);
	}
	//creates
	al_clear_to_color(colors[BLACK]);
	draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W, H - 30, ALLEGRO_ALIGN_RIGHT, "Загрузка...");
	al_flip_display();

	srand(time(NULL));

	Ship.create();

	menu_starts[0][0] = W/2; menu_starts[0][1] = H/2-70;
	menu_starts[1][0] = W/2; menu_starts[1][1] = H/2-175;
	menu_starts[2][0] = W/2; menu_starts[2][1] = H/2-105;
	menu_starts[3][0] = W/2; menu_starts[3][1] = H/2-105;


	for(int x = 0; x < KOLSAMPLES; x++)
	{
		instances[x] = al_create_sample_instance(samples[x]);
		al_attach_sample_instance_to_mixer(instances[x], al_get_default_mixer());
	}
	for(int p = 0; p < KOLSAMPLESMUSIC; p++)
	{
		al_set_sample_instance_gain(instances[p], gain_music);
	}
	for(int p = KOLSAMPLESMUSIC; p < KOLSAMPLES; p++)
	{
		al_set_sample_instance_gain(instances[p], gain_soundEff);
	}

	InitBackground(BG, 0, 0, 1, 0, -1, 1, al_get_bitmap_width(bgimage), al_get_bitmap_height(bgimage), bgimage);
	InitBackground(MG, 0, 0, 3, 0, -1, 1, W * 2, al_get_bitmap_height(mgimage), mgimage);
	InitBackground(FG, 0, 0, 5, 0, -1, 1, al_get_bitmap_width(fgimage), al_get_bitmap_height(fgimage), fgimage);
	InitBackground(gameFG, 0, 0, 2, 2, 0, 0, al_get_bitmap_width(fgimage), al_get_bitmap_height(fgimage), fgimage);
	back_image = al_get_target_bitmap();

	ship_bitmaps[1] = al_create_bitmap(40,40);
	if(!ship_bitmaps[1]) abort_example("Error creating bitmap");

	al_set_target_bitmap(ship_bitmaps[1]);
	al_clear_to_color(colors[WHITE]);
	al_draw_filled_rectangle(30, 13, 40, 27, colors[BLUE]);

	mini_map = al_create_bitmap(W/4, H/4);
	if(!mini_map) abort_example("Error creating bitmap");

	al_set_target_bitmap(mini_map);
	al_clear_to_color(colors[BLACK]);

	gameBgImage = al_create_bitmap(W*2, H*2);
	if(!gameBgImage) abort_example("Error creating bitmap");

	int sizeX, sizeY;
	int dK = 0;
	for (int p = 0; p < KOLMENU; p++)
	{
		int dX = menu_starts[p][0], dY = menu_starts[p][1];
		for(int u = 0; u < kolKnopMenu[p]; u++)
		{
			knopka *now = add_knopka();
			if(!now) abort_example("Error creating knopka");

			get_textMax(FontEn25, FontRu25, menu_texts[dK + u], 0, 0, sizeX, sizeY);

			if(KNOPKA_BEGYN_TIME <= dK + u &&  dK + u  <= KNOPKA_BEGYN_SOUND_EFF) {sizeX += 10; sizeY += 6;}
			if(KNOPKA_LAST_SHIP <= dK + u &&  dK + u  <= KNOPKA_NEXT_SHIP ||
				KNOPKA_LAST_SHIP_REC <= dK + u &&  dK + u  <= KNOPKA_NEXT_SHIP_REC) {sizeX = SIZESTREL; sizeY = SIZESTREL;}

			bitmap_collide = al_create_bitmap(sizeX, sizeY);
			if(!bitmap_collide) abort_example("Error creating bitmap");
			bitmap_no_collide = al_create_bitmap(sizeX, sizeY);
			if(!bitmap_no_collide) abort_example("Error creating bitmap");
			bitmap_block = al_create_bitmap(sizeX, sizeY);
			if(!bitmap_block) abort_example("Error creating bitmap");
			if(KNOPKA_BEGYN_TIME <= dK + u &&  dK + u  <= KNOPKA_BEGYN_SOUND_EFF) {
				al_set_target_bitmap(bitmap_collide);
				al_draw_filled_rounded_rectangle(1, 1, sizeX, sizeY, 5, 5, al_map_rgb(214, 223, 125));

				al_set_target_bitmap(bitmap_no_collide);
				al_draw_filled_rounded_rectangle(1, 1, sizeX, sizeY, 5, 5, al_map_rgb(186, 201, 50));

				al_set_target_bitmap(bitmap_block);
				al_draw_filled_rounded_rectangle(1, 1, sizeX, sizeY, 5, 5, al_map_rgb(143, 155, 38));
			}else if(KNOPKA_LAST_SHIP == dK + u || KNOPKA_LAST_SHIP_REC == dK + u ){

				al_set_target_bitmap(bitmap_collide);
				al_draw_filled_triangle(1, SIZESTREL/2, SIZESTREL, 1, SIZESTREL, SIZESTREL, al_map_rgb(214, 223, 125));

				al_set_target_bitmap(bitmap_no_collide);
				al_draw_filled_triangle(1, SIZESTREL/2, SIZESTREL, 1, SIZESTREL, SIZESTREL, al_map_rgb(186, 201, 50));

				al_set_target_bitmap(bitmap_block);
				al_draw_filled_triangle(1, SIZESTREL/2, SIZESTREL, 1, SIZESTREL, SIZESTREL, al_map_rgb(143, 155, 38));
			}else if(KNOPKA_NEXT_SHIP == dK + u || KNOPKA_NEXT_SHIP_REC == dK + u ){
				al_set_target_bitmap(bitmap_collide);
				al_draw_filled_triangle(1, 1, 1, SIZESTREL, SIZESTREL, SIZESTREL/2, al_map_rgb(214, 223, 125));

				al_set_target_bitmap(bitmap_no_collide);
				al_draw_filled_triangle(1, 1, 1, SIZESTREL, SIZESTREL, SIZESTREL/2, al_map_rgb(186, 201, 50));

				al_set_target_bitmap(bitmap_block);
				al_draw_filled_triangle(1, 1, 1, SIZESTREL, SIZESTREL, SIZESTREL/2, al_map_rgb(143, 155, 38));

			}else{
				al_set_target_bitmap(bitmap_collide);
				draw_text(FontEn25, FontRu25, al_map_rgb(214, 223, 125), 0, 0, 0, menu_texts[dK + u]);

				al_set_target_bitmap(bitmap_no_collide);
				draw_text(FontEn25, FontRu25, al_map_rgb(186, 201, 50), 0, 0, 0, menu_texts[dK + u]);

				al_set_target_bitmap(bitmap_block);
				draw_text(FontEn25, FontRu25, al_map_rgb(143, 155, 38), 0, 0, 0, menu_texts[dK + u]);
			}

			now->create(dX, dY, 0, SQUARE, bitmap_collide, bitmap_no_collide, bitmap_block, functions[dK + u]);

			menu_knopMass[dK+u] = now;

			now->show = false;

			dX += menu_changes[p][0]; dY += menu_changes[p][1];
		}
		dK += kolKnopMenu[p];
	}

	al_set_target_bitmap(back_image);

	menu_knopMass[KNOPKA_LAST]->y = menu_knopMass[KNOPKA_BEGYN_TIME]->y; 
	menu_knopMass[KNOPKA_LAST]->x = menu_knopMass[KNOPKA_BEGYN_TIME]->x - 154 - menu_knopMass[KNOPKA_LAST]->w/2 - menu_knopMass[KNOPKA_BEGYN_TIME]->w/2;
	menu_knopMass[KNOPKA_NEXT]->y = menu_knopMass[KNOPKA_BEGYN_TIME]->y; 
	menu_knopMass[KNOPKA_NEXT]->x = menu_knopMass[KNOPKA_BEGYN_TIME]->x + 154 + menu_knopMass[KNOPKA_NEXT]->w/2 + menu_knopMass[KNOPKA_BEGYN_TIME]->w/2;

	menu_knopMass[KNOPKA_CONTINUE]->x = menu_knopMass[KNOPKA_PLAY]->x; menu_knopMass[KNOPKA_CONTINUE]->y = menu_knopMass[KNOPKA_PLAY]->y;

	menu_knopMass[KNOPKA_CONTINUE]->iKey = ALLEGRO_KEY_ESCAPE;

	fps = al_create_timer(1/FPS);
	event_queue = al_create_event_queue();

	// registers

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source()); 
	al_register_event_source(event_queue, al_get_keyboard_event_source()); 
	al_register_event_source(event_queue, al_get_timer_event_source(fps)); 

	fon = colors[BLACK];
	cursor.color = al_map_rgb(255,0,0);

	set_playMode(MENU);
	backGroundMusic = random(0,KOLSAMPLESMUSIC-1);
	al_play_sample_instance(instances[backGroundMusic]);
	al_start_timer(fps);

	while(!done)
	{
		al_wait_for_event(event_queue,&ev);
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
        { 
            done = true; 
        }
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				//done = true;
				keys[ALLEGRO_KEY_ESCAPE] = true;
				LastKeyKeyboad[ALLEGRO_KEY_ESCAPE] = true;
			}else{
				keys[ev.keyboard.keycode] = true;
				LastKeyKeyboad[ev.keyboard.keycode] = true;
			}
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			keys[ev.keyboard.keycode] = false;
		}
		if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			cursor.x = ev.mouse.x;
			cursor.y = ev.mouse.y;
			cursor.z += ev.mouse.dz;
		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(ev.mouse.button >= 1){
				cursor.keys[ev.mouse.button-1] = true;
				cursor.lastkey[ev.mouse.button-1] = true;
			}
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			if (ev.mouse.button >= 1){
				cursor.keys[ev.mouse.button-1] = false;
			}
		}

		/*if (ev.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY)
		{
			al_show_mouse_cursor(display);
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{
			al_hide_mouse_cursor(display);
		}*/

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (ev.timer.source == fps)
			{
				draw = true;

				if(knopka_time_fixX == -1000) UpdateMuisc();

				collide_knopki();
				use_knopki();
				update_knopki();
				switch(iRejIg) 
				{
				case PLAY:
					timeFlip++;
					if(ship_respawn > 0)
					{
						if(--ship_respawn <= 0)
						{
							gameBG.x = gameBG.y = gameFG.x = gameFG.y = 0;
							Ship.respawn();
							if(Ship.lives < 0)
							{
								set_playMode(ENDGAME);
								if(score > records[ship_now][9])
								{
									int p;
									for(p = 8; p > -1; p--)
									{
										if(score > records[ship_now][p]){
											records[ship_now][p+1] = records[ship_now][p];
										}else{
											break;
										}
									}
									records[ship_now][p+1] = score;
									records[ship_now][10] = 0;
									for(int k = 0; k < 10; k++)
									{
										records[ship_now][10] += records[ship_now][k];
									}
									records[ship_now][10] *= 17;
									records[ship_now][10] += 274;
									fIn = fopen("Resources/records.txt", "w");
									for(int u = 0; u < KOLSHIPS; u++)
									{
										fprintf(fIn, "%i", records[u][0]);
										for(int i = 1; i < 11; i++)
										{
											fprintf(fIn, "\n%i", records[u][i]);
										}
										if(u < KOLSHIPS-1)
										{
											fprintf(fIn, "\n");
										}
									}
									fclose(fIn);
								}
							}
						}
					}else{
						if(Ship.shield)
						{
							Ship.shield -= 0.25;
							if(Ship.shield <= 0) 
							{
								Ship.shield = 0;
								Ship.obj.r -= 17;
							}
						}
						Ship.power += 100/(MINUTE/2);
						if(Ship.power >= 100)
						{
							Ship.power = 100;
						}
						if(keys[ALLEGRO_KEY_A] || keys[ALLEGRO_KEY_LEFT]) Ship.rotate(-4);
						if(keys[ALLEGRO_KEY_D] || keys[ALLEGRO_KEY_RIGHT]) Ship.rotate(4);
						if(keys[ALLEGRO_KEY_W] || keys[ALLEGRO_KEY_UP])
						{
							tochka ch(cos(Ship.angle/FROMRADTOGRAD) * 0.05, sin(Ship.angle/FROMRADTOGRAD) * 0.05);
							Ship.speed.init(tNull, Ship.speed.t2 + ch);
							if(Ship.speed.len > 5)
							{
								ch = tochka(Ship.speed.t2.x / Ship.speed.len * 5, Ship.speed.t2.y / Ship.speed.len * 5);
								Ship.speed.init(tNull, ch);
							}
						}
						if(keys[ALLEGRO_KEY_S] || keys[ALLEGRO_KEY_DOWN])
						{
							if(Ship.speed.len > 0.05){
								tochka ch(Ship.speed.t2.x / Ship.speed.len * 0.05, Ship.speed.t2.y / Ship.speed.len * 0.05);
								Ship.speed.init(tNull, Ship.speed.t2 - ch);
							}else{
								Ship.speed.init(tNull, tNull);
							}
						}
						Ship.move();
						if(LastKeyKeyboad[ALLEGRO_KEY_SPACE] || cursor.lastkey[MLEFT])
						{
							al_stop_sample_instance(instances[BLAST]);
							al_play_sample_instance(instances[BLAST]);
							Blast* blNow = add_blast();
							if(!blNow) abort_example("Error creating Blast");

							blNow->create(Ship);
							float x1 = al_get_bitmap_width(ship_bitmaps[ship_now])/2.0, y1 = 0;
							ALLEGRO_TRANSFORM transform;
							al_identity_transform(&transform);
							al_rotate_transform(&transform, Ship.angle/FROMRADTOGRAD);
							al_transform_coordinates(&transform, &x1, &y1);
							blNow->obj.pos += tochka(x1, y1);
						}
						if(LastKeyKeyboad[ALLEGRO_KEY_ALT] && Ship.power >= 100)
						{
							al_stop_sample_instance(instances[BLAST]);
							al_play_sample_instance(instances[BLAST]);
							Ship.power = 0;
							for(float angle = 0; angle < 360; angle++)
							{
								Blast* blNow = add_blast();
								if(!blNow) abort_example("Error creating Blast");
								Ship.angle++;
								blNow->create(Ship);
								blNow->obj.damage = 4;
								float x1 = al_get_bitmap_width(ship_bitmaps[ship_now])/2.0, y1 = 0;
								ALLEGRO_TRANSFORM transform;
								al_identity_transform(&transform);
								al_rotate_transform(&transform, Ship.angle/FROMRADTOGRAD);
								al_transform_coordinates(&transform, &x1, &y1);
								blNow->obj.pos += tochka(x1, y1);
							}
						}
					}
					for(Bonus* boNow = Bonuses; boNow != NULL; boNow = boNow->next)
					{
						Bonus bo;
						boNow->update();
						if(boNow->now > (3 + boNow->lvl*3)*SECOND)
						{
							bo.next = boNow->next;
							delete_bonus(boNow);
							boNow = &bo;
						}
						if(Ship.obj.collide(boNow->obj, NULL, 0, 0))
						{
							Bonus_act = *boNow;
							Bonus_act.function();

							bo.next = boNow->next;
							delete_bonus(boNow);
							boNow = &bo;
						}
					}
					for(Explosion* exNow = Explosions; exNow != NULL; exNow = exNow->next)
					{
						Explosion ex;
						exNow->update_flip();
						if(exNow->gone)
						{
							ex.next = exNow->next;
							delete_explosion(exNow);
							exNow = &ex;
						}
					}
					for(Asteroid* asNow = Asteroids; asNow != NULL; asNow = asNow->next)
					{
						asNow->move();
						if(asNow->obj.pos.x < -W + Ship.pos.x){
							while(asNow->obj.pos.x < -W + Ship.pos.x){
								asNow->obj.pos.x += 2*W;
							}
						}else if(asNow->obj.pos.x > W + Ship.pos.x){
							while(asNow->obj.pos.x > W + Ship.pos.x){
								asNow->obj.pos.x -= 2*W;
							}
						}if(asNow->obj.pos.y < -H + Ship.pos.y){
							while(asNow->obj.pos.y < -H + Ship.pos.y){
							asNow->obj.pos.y += 2*H;
							}
						}else if(asNow->obj.pos.y > H + Ship.pos.y){
							while(asNow->obj.pos.y > H + Ship.pos.y){
							asNow->obj.pos.y -= 2*H;
							}
						}
						asNow->update_flip();
					}
					for(Blast* blNow = Blasts; blNow != NULL; blNow = blNow->next)
					{
						blNow->move();
						if(blNow->now > W)
						{
 							Blast bl;
							bl.next = blNow->next;
							delete_blast(blNow);
							blNow = &bl;
						}
					}
					for(Blast* blNow = Blasts; blNow != NULL; blNow = blNow->next)
					{
						Blast bl;
						for(Asteroid* asNow = Asteroids; asNow != NULL; asNow = asNow->next)
						{
							if(!asNow->obj.gone)
							{
								if(blNow->obj.collide(asNow->obj, asNow->col_polygon, 
									(asNow->type != KOLASTEROIDS-1) ? (Asteroid_col_polygon_size[asNow->type][(int)asNow->flip]): (0), asNow->flip))
								{
									asNow->obj.health -= blNow->obj.damage;
									if(asNow->obj.gone = (asNow->obj.health <= 0))
									{
										al_stop_sample_instance(instances[ASTEREX]);
										al_play_sample_instance(instances[ASTEREX]);
									}
									al_stop_sample_instance(instances[BLASTEX]);
									al_play_sample_instance(instances[BLASTEX]);
									bl.next = blNow->next;
									delete_blast(blNow);
									blNow = &bl;
									break;
								}
							}
						}
					}
					if(!ship_respawn)
					{
						if(Ship.immortal_len)
						{
							Ship.immortal_len--;
						}else{
							for(Asteroid* asNow = Asteroids; asNow != NULL; asNow = asNow->next)
							{
								Asteroid as;
								Explosion *ex;
								if(!asNow->obj.gone)
								{
									if(Ship.obj.collide(asNow->obj, asNow->col_polygon, 
										(asNow->type != KOLASTEROIDS-1) ? (Asteroid_col_polygon_size[asNow->type][(int)asNow->flip]): (0), asNow->flip))
									{
										al_stop_sample_instance(instances[ASTEREX]);
										al_play_sample_instance(instances[ASTEREX]);
										if(Ship.shield){
											asNow->obj.gone = true;
											Ship.shield -= asNow->obj.damage * asNow->obj.health;
											if(Ship.shield <= 0) 
											{
												Ship.shield = 0;
												Ship.obj.r -= 17;
											}
										}else{
											Ship.obj.health -= asNow->obj.damage;
											ex = add_explosion();
											if(!ex) abort_example("Error creating Explosion");
											ex->create((asNow->type == KOLASTEROIDS-1) ? (EXASBLUE): (EXASRED), asNow->lvl, asNow->obj.pos, asNow->angle);
											as.next = asNow->next;
											delete_asteroid(asNow);
											asNow = &as;
											if(Ship.obj.health <= 0)
											{
												al_stop_sample_instance(instances[SHIPEX]);
												al_play_sample_instance(instances[SHIPEX]);
												ex = add_explosion();
												if(!ex) abort_example("Error creating Explosion");
												ex->create(EXSHIP, 1, Ship.pos, Ship.angle);
												ship_respawn = SECOND;
												break;
											}
										}
									}
								}
							}
						}
					}
					for(Asteroid* asNow = Asteroids; asNow != NULL; asNow = asNow->next)
					{
						Asteroid as;
						Explosion *ex;
						Bonus *bo;
						if(asNow->obj.gone)
						{
							if(asNow->lvl != 1)
							{
								tochka t1 = tochka(cos((asNow->speed.angle-35)/FROMRADTOGRAD)*asNow->speed.len,
									sin((asNow->speed.angle-35)/FROMRADTOGRAD)*asNow->speed.len);
								Asteroid *asAdd = add_asteroid();
								asAdd->create((asNow->type == KOLASTEROIDS-1) ? (asNow->type): (random(0, KOLASTEROIDS-2)),
									asNow->lvl-1, asNow->obj.pos, t1);
								t1 = tochka(cos((asNow->speed.angle+35)/FROMRADTOGRAD)*asNow->speed.len,
									sin((asNow->speed.angle+35)/FROMRADTOGRAD)*asNow->speed.len);
								asAdd = add_asteroid();
								asAdd->create((asNow->type == KOLASTEROIDS-1) ? (asNow->type): (random(0, KOLASTEROIDS-2)),
									asNow->lvl-1, asNow->obj.pos, t1);
							}
							score += asNow->lvl*50;
							as.next = asNow->next;
							ex = add_explosion();
							if(!ex) abort_example("Error creating Explosion");
							ex->create((asNow->type == KOLASTEROIDS-1) ? (EXASBLUE): (EXASRED), asNow->lvl, asNow->obj.pos, asNow->angle);
							if(asNow->lvl > 1)
							{
								if(random(1, 100) <= 20)
								{
									bo = add_bonus();
									int j = random(0, KOLBONUS-1);
									bo->create(j, random(1, asNow->lvl-1), asNow->obj.pos, Bonus_functions[j]);
								}
							}
							delete_asteroid(asNow);
							asNow = &as;
						}
					}
					ALLEGRO_TRANSFORM trans;
					al_identity_transform(&trans);
					al_use_transform(&trans);
					if((timeFlip % ((int)SECOND) == 0 || kol_Asteroids == 0) && kol_Asteroids < 10+score/500)
					{
						Asteroid* asNow = add_asteroid();
						if(!asNow) abort_example("Error creating asteroid");
						asNow->create();
						asNow->obj.pos += Ship.pos;
					}

					if(!ship_respawn)
					{
						gameFG.dirX = -Ship.speed.t2.x; gameFG.dirY = -Ship.speed.t2.y;
						UpdateBackground(gameFG);
						gameBG.dirX = -Ship.speed.t2.x; gameBG.dirY = -Ship.speed.t2.y;
						UpdateBackground(gameBG);
					}
					break;
				case OPTIONS:
					if(!cursor.keys[MLEFT]) {knopka_music_fixX = -1000; knopka_soundEff_fixX = -1000;}
					if(knopka_music_fixX != -1000){
						gain_music = (cursor.x - (W/2 - 100) - knopka_music_fixX) / 200.0;
						if(gain_music > 1) gain_music = 1;
						if(gain_music < 0) gain_music = 0;
						for(int p = 0; p < KOLSAMPLESMUSIC; p++)
						{
							al_set_sample_instance_gain(instances[p], gain_music);
						}
					}if(knopka_soundEff_fixX != -1000){
						gain_soundEff = (cursor.x - (W/2 - 100) - knopka_soundEff_fixX) / 200.0;
						if(gain_soundEff > 1) gain_soundEff = 1;
						if(gain_soundEff < 0) gain_soundEff = 0;
						for(int p = KOLSAMPLESMUSIC; p < KOLSAMPLES; p++)
						{
							al_set_sample_instance_gain(instances[p], gain_soundEff);
						}
					}if(knopka_time_fixX != -1000){
						if(!cursor.keys[MLEFT]){
							knopka_time_fixX = -1000;
							al_play_sample_instance(instances[backGroundMusic]);
						}else{
							al_stop_sample_instance(instances[backGroundMusic]);

							time_music = (cursor.x - (W/2 - 150) - knopka_time_fixX) / 300.0;
							if(time_music > 1) time_music = 1;
							if(time_music < 0) time_music = 0;

							al_set_sample_instance_position(instances[backGroundMusic],
								al_get_sample_instance_length(instances[backGroundMusic]) * time_music);
							menu_knopMass[KNOPKA_BEGYN_TIME]->x = W/2-150 + time_music * 300;
						}
					}else{ menu_knopMass[KNOPKA_BEGYN_TIME]->x = W/2-150 + al_get_sample_instance_position(instances[backGroundMusic]) /
						(float) al_get_sample_instance_length(instances[backGroundMusic]) * 300;
					}
					menu_knopMass[KNOPKA_BEGYN_MUSIC]->x = W/2-100 + gain_music * 200;
					menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->x = W/2-100 + gain_soundEff * 200;
				case PLAYOPT:
				case RECORDS:
				case ENDGAME:
				case MENU:
				case PAUSE:
					UpdateBackground(BG);
					UpdateBackground(MG);
					UpdateBackground(FG);
					break;
				default:
					break;
				}

				if(LastKeyKeyboad[ALLEGRO_KEY_F1]) F1 = !F1;
				if(LastKeyKeyboad[ALLEGRO_KEY_F2]) F2 = !F2;

				for(int p = 0; p < 5; p++)
				{
					cursor.lastkey[p] = false;
				}
				for(int p = 0; p < ALLEGRO_KEY_MAX; p++)
				{
					LastKeyKeyboad[p] = false;
				}
			}

			/*if (ev.timer.source == )
			{
				
			}*/
		}

		if (draw && al_is_event_queue_empty(event_queue))
		{
			draw = false;
			al_clear_to_color(fon);
			ALLEGRO_COLOR color;
			int retX, retY;
			float y, x;
			int w1, h1, x1, y1, y2;
			tochka t1, t2;

			switch (iRejIg)
			{
			case PLAY:
				ALLEGRO_TRANSFORM transform;
				al_identity_transform(&transform);
				al_translate_transform(&transform, - Ship.speed.t2.x*KOOFOTD * gameBG.valX - 300, - Ship.speed.t2.y*KOOFOTD * gameBG.valY - 300);
				al_use_transform(&transform);

				DrawBackground(gameBG, F2);

				Ship.use_trans();

				color = colors[PURPLE]; color.a = 0.3;
				for(Explosion* exNow = Explosions; exNow != NULL; exNow = exNow->next)
				{
					exNow->draw();
				}
				for(Asteroid* asNow = Asteroids; asNow != NULL; asNow = asNow->next)
				{
					asNow->draw();
					if(F1)
					{
						al_draw_filled_circle(asNow->obj.pos.x, asNow->obj.pos.y, asNow->obj.r, color);
						if(asNow->type != KOLASTEROIDS-1)
						{
							for(int u = 0; u < Asteroid_col_polygon_size[asNow->type][(int)asNow->flip]; u++)
							{
								tochka t1 = asNow->col_polygon[(int)asNow->flip][u] + asNow->obj.pos,
									t2 = asNow->col_polygon[(int)asNow->flip][(u+1) % Asteroid_col_polygon_size[asNow->type][(int)asNow->flip]] + asNow->obj.pos;
								al_draw_line(t1.x, t1.y, t2.x, t2.y, color, 2);
							}
						}
					}
				}
				for(Bonus* boNow = Bonuses; boNow != NULL; boNow = boNow->next)
				{
					if(boNow->now < boNow->lvl*2*SECOND || boNow->now % (int)SECOND < 2*SECOND/3)
						boNow->draw();
					if(F1)
						al_draw_filled_circle(boNow->obj.pos.x, boNow->obj.pos.y, boNow->obj.r, color);
				}
				for(Blast* blNow = Blasts; blNow != NULL; blNow = blNow->next)
				{
					blNow->draw();
					if(F1)
					{
						al_draw_filled_circle(blNow->obj.pos.x, blNow->obj.pos.y, blNow->obj.r, color);
					}
				}

				al_identity_transform(&transform);
				al_use_transform(&transform);

				if(((Ship.immortal_len % (int)SECOND < 3*SECOND/4) || Ship.immortal_len == 0) && !ship_respawn)
				{
					Ship.draw();
				}

				w1 = al_get_bitmap_width(mini_map); h1 = al_get_bitmap_height(mini_map);

				back_image = al_get_target_bitmap();
				al_set_target_bitmap(mini_map);

				al_clear_to_color(colors[BLACK]);
				al_draw_rectangle(1, 1, w1, h1, colors[RED], 1);

				for(Asteroid* asNow = Asteroids; asNow != NULL; asNow = asNow->next)
				{
					t1 = asNow->obj.pos - Ship.pos + tochka(W,H);

					t1.x *= (float)w1 / W * 0.5;
					t1.y *= (float)h1 / H * 0.5;

					al_draw_filled_circle(t1.x, t1.y, asNow->obj.r * (float)w1 / W * 0.5,
						colors[(asNow->type == KOLASTEROIDS-1) ? (BLUE): (ORANGE)]);
					al_draw_circle(t1.x, t1.y, asNow->obj.r * (float)w1 / W * 0.5, colors[GREEN], 1);
				}
				al_draw_filled_circle(w1/2, h1/2, Ship.obj.r * (float)w1 / W * 0.5, colors[PURPLE]);
				al_draw_circle(w1/2, h1/2, Ship.obj.r * (float)w1 / W * 0.5, colors[BLUE], 1);

				al_set_target_bitmap(back_image);
				al_draw_tinted_bitmap(mini_map, al_map_rgba(255, 255, 255, 100), 10, H-h1-10, 0);
				al_draw_filled_rounded_rectangle(5, 5, 5 + 300*Ship.obj.health/(float)Ship.max_health, 30, 6, 7, al_map_rgba(127, 0, 0, 100));
				al_draw_rounded_rectangle(5, 5, 305, 30, 6, 7, 
					al_map_rgba_f(colors[BLASTEROIDES].r, colors[BLASTEROIDES].g, colors[BLASTEROIDES].b, 0.3), 3);
				al_draw_textf(FontEn25, al_map_rgba_f(colors[BLASTEROIDES].r, colors[BLASTEROIDES].g, colors[BLASTEROIDES].b, 0.3),
					155, 3, ALLEGRO_ALIGN_CENTRE, "%i/%i", Ship.obj.health, Ship.max_health);
				if(Ship.lives)
				{
					y2 = 33 + (((al_get_bitmap_height(Ship.bitmap) - al_get_font_line_height(FontEn25))/2.0 > 0) ? 
						((al_get_bitmap_height(Ship.bitmap) - al_get_font_line_height(FontEn25))/2.0): (0));

					draw_textf(FontEn25, FontRu25, al_map_rgba_f(colors[BLASTEROIDES].r, colors[BLASTEROIDES].g, colors[BLASTEROIDES].b, 0.3),
						5, y2, 0, "Жизней %i:", Ship.lives);
					get_textfMax(FontEn25, FontRu25, 5, 0, x1, y1, "Жизней %i:", Ship.lives);
					for(int u = 0; u < Ship.lives; u++)
					{
						if(Ship.bitmap)
							al_draw_tinted_bitmap(Ship.bitmap, al_map_rgba(255, 255, 255, 100), 
								x1 + (al_get_bitmap_width(Ship.bitmap)+5) * u, 33, 0);
					}
				}
				y2 = 35 + ((Ship.lives != 0) ? (al_get_bitmap_height(Ship.bitmap)): (0));
				if(Ship.shield)
				{
					al_draw_tinted_bitmap(Bonus_bitmaps[SHIELD], al_map_rgba_f(1, 1, 1, 0.3), 5, y2, 0);
					al_draw_filled_rounded_rectangle(5 + al_get_bitmap_width(Bonus_bitmaps[SHIELD]), y2, 
						5 + al_get_bitmap_width(Bonus_bitmaps[SHIELD]) + 500*(Ship.shield / 1000.0), y2 + 20, 6, 7, al_map_rgba(0, 25, 187, 100));
					y2 += 25;
				}
				al_draw_filled_rounded_rectangle(5, y2, 
						5 + 300*(Ship.power / 100), y2 + 20, 6, 7, al_map_rgba(0, 128, 0, 100));
				al_draw_rounded_rectangle(5, y2, 
						5 + 300, y2 + 20, 6, 7, al_map_rgba_f(colors[BLASTEROIDES].r, colors[BLASTEROIDES].g, colors[BLASTEROIDES].b, 0.3), 3);
				draw_textf(FontEn30, FontRu30, al_map_rgba_f(colors[BLASTEROIDES].r, colors[BLASTEROIDES].g, colors[BLASTEROIDES].b, 0.3),
					W, H-25, 2, "Счет: %i", score);

				if(F1)
				{
					al_identity_transform(&transform);
					al_use_transform(&transform);
					draw_textf(FontEn25, FontRu25, colors[PURPLE], 0, 0, 0, 
						"Sp:: x: %0.2f, y: %0.2f, len: %0.2f \nPos:: x: %0.2f, y: %0.2f \nKolAst: %i \nKolBlast: %i \nKolEx: %i \nKolBo: %i",
						Ship.speed.t2.x, Ship.speed.t2.y, Ship.speed.len, Ship.pos.x, Ship.pos.y, kol_Asteroids, kol_Blasts, kol_Explosions, kol_Bonuses);
					Ship.use_trans();
					al_draw_filled_circle(Ship.obj.pos.x, Ship.obj.pos.y, Ship.obj.r, color);
				}
				al_identity_transform(&transform);
				al_translate_transform(&transform, - Ship.speed.t2.x*KOOFOTD * gameFG.valX - 300, - Ship.speed.t2.y*KOOFOTD * gameFG.valY - 300);
				al_use_transform(&transform);

				DrawBackground(gameFG, F2);

				al_identity_transform(&transform);
				al_use_transform(&transform);
				break;
			case ENDGAME:
			case RECORDS:
			case PLAYOPT:
			case OPTIONS:
			case MENU:
			case PAUSE:
				DrawBackground(BG, F2);
				DrawBackground(MG, F2);
				DrawBackground(FG, F2);
				color = al_map_rgba(10, 10, 10, 128);
				switch(iRejIg)
				{
				case ENDGAME:
					draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W/2, 70, 1, "Конец Игры");
					draw_textf(FontEn30, FontRu30, colors[BLASTEROIDES], W/2, 120, 1, "Счет: %i", score);
					al_draw_filled_rounded_rectangle(menu_knopMass[KNOPKA_OPT]->x - menu_knopMass[KNOPKA_OPT]->w/2 - 5, 
						menu_knopMass[KNOPKA_PLAY]->y - menu_knopMass[KNOPKA_PLAY]->h/2 - 5, 
						menu_knopMass[KNOPKA_OPT]->x + menu_knopMass[KNOPKA_OPT]->w/2 + 5,
						menu_knopMass[KNOPKA_QUIT]->y + menu_knopMass[KNOPKA_QUIT]->h/2 + 5, 10, 10, color);
					break;
				case MENU:
					draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W/2, 70, 1, "Главное Меню");
					al_draw_filled_rounded_rectangle(menu_knopMass[KNOPKA_OPT]->x - menu_knopMass[KNOPKA_OPT]->w/2 - 5, 
						menu_knopMass[KNOPKA_PLAY]->y - menu_knopMass[KNOPKA_PLAY]->h/2 - 5, 
						menu_knopMass[KNOPKA_OPT]->x + menu_knopMass[KNOPKA_OPT]->w/2 + 5,
						menu_knopMass[KNOPKA_QUIT]->y + menu_knopMass[KNOPKA_QUIT]->h/2 + 5, 10, 10, color);
					break;
				case PAUSE:
					draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W/2, 70, 1, "Пауза");
					al_draw_filled_rounded_rectangle(menu_knopMass[KNOPKA_CONTINUE]->x - menu_knopMass[KNOPKA_CONTINUE]->w/2 - 5, 
						menu_knopMass[KNOPKA_CONTINUE]->y - menu_knopMass[KNOPKA_CONTINUE]->h/2 - 5, 
						menu_knopMass[KNOPKA_CONTINUE]->x + menu_knopMass[KNOPKA_CONTINUE]->w/2 + 5,
						menu_knopMass[KNOPKA_QUIT]->y + menu_knopMass[KNOPKA_QUIT]->h/2 + 5, 10, 10, color);
					break;
				case OPTIONS:
					get_textMax(FontEn25, FontRu25, "Гром. Зв. Эфф.", 0, 0, retX, retY);
					al_draw_filled_rounded_rectangle(W/2 - 109 - menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->w/2 - retX, 
						menu_knopMass[KNOPKA_BEGYN_TIME]->y - menu_knopMass[KNOPKA_BEGYN_TIME]->h/2 - 40, 
						menu_knopMass[KNOPKA_NEXT]->x + menu_knopMass[KNOPKA_NEXT]->w/2 + 5,
						menu_knopMass[KNOPKA_BACK]->y + menu_knopMass[KNOPKA_BACK]->h/2 + 5, 10, 10, color);
					draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W/2, 70, 1, "Настройки");
					draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W/2,
						menu_knopMass[KNOPKA_BEGYN_TIME]->y - menu_knopMass[KNOPKA_BEGYN_TIME]->h/2 - 35, 1, NameMusic[backGroundMusic]);
					color = al_map_rgba(0, 255, 0, 128);
					al_draw_line(W/2 - 100, menu_knopMass[KNOPKA_BEGYN_MUSIC]->y, 
								W/2 + 100, menu_knopMass[KNOPKA_BEGYN_MUSIC]->y, color, 3);
					draw_text(FontEn25, FontRu25, colors[BLASTEROIDES], W/2 - 104 - menu_knopMass[KNOPKA_BEGYN_MUSIC]->w/2, 
						menu_knopMass[KNOPKA_BEGYN_MUSIC]->y - menu_knopMass[KNOPKA_BEGYN_MUSIC]->h/2, 2, "Гром. Музыки");
					al_draw_line(W/2 - 100, menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->y, 
								W/2 + 100, menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->y, color, 3);
					draw_text(FontEn25, FontRu25, colors[BLASTEROIDES], W/2 - 104 - menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->w/2, 
						menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->y - menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->h/2, 2, "Гром. Зв. Эфф.");
					al_draw_line(W/2 - 150, menu_knopMass[KNOPKA_BEGYN_TIME]->y, 
								W/2 + 150, menu_knopMass[KNOPKA_BEGYN_TIME]->y, color, 3);
					break;
				case PLAYOPT:
					y = H;
					y = MIN(y, (menu_knopMass[KNOPKA_LAST_SHIP]->y - menu_knopMass[KNOPKA_LAST_SHIP]->h/2));
					y = MIN(y, (menu_knopMass[KNOPKA_NEXT_SHIP]->y - menu_knopMass[KNOPKA_NEXT_SHIP]->h/2));
					y = MIN(y, (menu_knopMass[KNOPKA_LAST_SHIP]->y - al_get_bitmap_height((ship_bitmaps[ship_now]) ? 
						(ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0));

					draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W/2, 70, 1, "Настройки Игры");

					al_draw_filled_rounded_rectangle(menu_knopMass[KNOPKA_LAST_SHIP]->x - menu_knopMass[KNOPKA_LAST_SHIP]->w/2 - 5, 
						y - 5, 
						menu_knopMass[KNOPKA_NEXT_SHIP]->x + menu_knopMass[KNOPKA_NEXT_SHIP]->w/2 + 5,
						menu_knopMass[KNOPKA_BACK]->y + menu_knopMass[KNOPKA_BACK]->h/2 + 5, 10, 10, color);

					if(ship_bitmaps[ship_now]) al_draw_bitmap(ship_bitmaps[ship_now], W/2 - al_get_bitmap_width(ship_bitmaps[ship_now])/2.0,
						menu_knopMass[KNOPKA_LAST_SHIP]->y - al_get_bitmap_height(ship_bitmaps[ship_now])/2.0, 0);
					else al_draw_tinted_bitmap(ship_bitmaps[0], colors[BLACK],  W/2 - al_get_bitmap_width(ship_bitmaps[0])/2.0,
						menu_knopMass[KNOPKA_LAST_SHIP]->y - al_get_bitmap_height(ship_bitmaps[0])/2.0, 0);
					break;
				case RECORDS:
					y = H;
					y = MIN(y, (menu_knopMass[KNOPKA_LAST_SHIP_REC]->y - menu_knopMass[KNOPKA_LAST_SHIP_REC]->h/2));
					y = MIN(y, (menu_knopMass[KNOPKA_NEXT_SHIP_REC]->y - menu_knopMass[KNOPKA_NEXT_SHIP_REC]->h/2));
					y = MIN(y, (menu_knopMass[KNOPKA_LAST_SHIP_REC]->y - al_get_bitmap_height((ship_bitmaps[ship_now]) ? 
						(ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0));
					x = W;
					get_textfMax(FontEn25, FontRu25, 0, 0, x1, y1, 
						"1: %i\n2: %i\n3: %i\n4: %i\n5: %i\n6: %i\n7: %i\n8: %i\n9: %i\n10: %i",
						records[ship_rec][0], records[ship_rec][1], records[ship_rec][2], 
						records[ship_rec][3], records[ship_rec][4], records[ship_rec][5], 
						records[ship_rec][6], records[ship_rec][7], records[ship_rec][8], 
						records[ship_rec][9]);
					x = MIN(x, (W-x1)/2.0);
					x = MIN(x, (menu_knopMass[KNOPKA_LAST_SHIP_REC]->x - menu_knopMass[KNOPKA_LAST_SHIP_REC]->w/2));
					draw_text(FontEn30, FontRu30, colors[BLASTEROIDES], W/2, 70, 1, "Рекорды");

					al_draw_filled_rounded_rectangle(x - 5, y - 5, W - x + 5,
						menu_knopMass[KNOPKA_BACK]->y + menu_knopMass[KNOPKA_BACK]->h/2 + 5, 10, 10, color);

					if(ship_bitmaps[ship_rec]) al_draw_bitmap(ship_bitmaps[ship_rec], W/2 - al_get_bitmap_width(ship_bitmaps[ship_rec])/2.0,
						menu_knopMass[KNOPKA_LAST_SHIP_REC]->y - al_get_bitmap_height(ship_bitmaps[ship_rec])/2.0, 0);
					else al_draw_tinted_bitmap(ship_bitmaps[0], colors[BLACK],  W/2 - al_get_bitmap_width(ship_bitmaps[0])/2.0,
						menu_knopMass[KNOPKA_LAST_SHIP_REC]->y - al_get_bitmap_height(ship_bitmaps[0])/2.0, 0);
					draw_textf(FontEn25, FontRu25, colors[BLASTEROIDES], W/2, 
						menu_knopMass[KNOPKA_BACK]->y - menu_knopMass[KNOPKA_BACK]->h/2 - y1-2, ALLEGRO_ALIGN_CENTER, 
						"1: %i\n2: %i\n3: %i\n4: %i\n5: %i\n6: %i\n7: %i\n8: %i\n9: %i\n10: %i",
						records[ship_rec][0], records[ship_rec][1], records[ship_rec][2], 
						records[ship_rec][3], records[ship_rec][4], records[ship_rec][5], 
						records[ship_rec][6], records[ship_rec][7], records[ship_rec][8], 
						records[ship_rec][9]);
					break;
				default:
					break;
				}
				
				draw_text(FontEn25, FontRu25, colors[BLASTEROIDES], W-2, H-27, 2, "Бластеройды v2.0");
				break;
			default:
				break;
			}
			view_knopki();

			if(iRejIg == OPTIONS)
			{
				draw_textf(FontEn30, FontRu30, colors[BLACK], menu_knopMass[KNOPKA_BEGYN_MUSIC]->x, 
					menu_knopMass[KNOPKA_BEGYN_MUSIC]->y-15, ALLEGRO_ALIGN_CENTRE, "%.0f", gain_music * 100);
				draw_textf(FontEn30, FontRu30, colors[BLACK], menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->x, 
					menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->y-15, ALLEGRO_ALIGN_CENTRE, "%.0f", gain_soundEff * 100);
			}

			al_flip_display();
		}

		/*if (ev.type == )
		{
			
		}*/
	}

	// destroy all
	delete_knopki();

	for(int g = 0; g < KOLASTEROIDS-1; g++)
	{
		for(int u = 0; u < Asteroid_flips_in_bitmap[g][2]; u++)
		{
			delete Asteroid_col_polygon[g][u];
		}
		delete Asteroid_col_polygon[g];
		delete Asteroid_col_polygon_size[g];
	}
	for(int u = 0; u < KOLASTEROIDS; u++)
	{
		al_destroy_bitmap(Asteroid_bitmaps[u]);
	}
	for(int u = 0; u < KOLBONUS; u++)
	{
		al_destroy_bitmap(Bonus_bitmaps[u]);
	}
	for(int u = 0; u < KOLEXPLOSION; u++)
	{
		al_destroy_bitmap(Explosion_bitmaps[u]);
	}
	for(int u = 0; u < KOLGALAXY; u++)
	{
		al_destroy_bitmap(galaxy_bitmaps[u]);
	}
	for(int u = 0; u < KOLPLANET; u++)
	{
		al_destroy_bitmap(planet_bitmaps[u]);
	}
	
	delete_asteroids();
	delete_bonuses();
	delete_explosions();
	delete_blasts();
	al_destroy_timer(fps);
	al_destroy_event_queue(event_queue);

	al_destroy_font(FontEn12);
	al_destroy_font(FontEn25);
	al_destroy_font(FontRu25);
	al_destroy_font(FontEn30);
	al_destroy_font(FontRu30);
	
	for(int p = 0; p < KOLSHIPS; p++)
	{
		al_destroy_bitmap(ship_bitmaps[p]);
	}
	al_destroy_bitmap(bgimage);
	al_destroy_bitmap(mgimage);
	al_destroy_bitmap(fgimage);
	al_destroy_bitmap(gameBgImage);
	al_destroy_bitmap(mini_map);

	for(int x = 0; x < KOLSAMPLES; x++)
	{
		al_destroy_sample(samples[x]);
		al_destroy_sample_instance(instances[x]);
	}

	al_destroy_display(display);

	return 0;
};

void exit_game()
{
	done = true;
}
void play()
{
	Asteroid *as;
	int kolGalaxies, kolGalaxiesDone = 0;
	int randCoorX, randCoorY, randNum;
	float bitmapW, bitmapH, Sootn;
	bool bMass[KOLGALAXY];
	bool bMatr[6][4];
	switch(iRejIg)
	{
	case PLAYOPT:
		timeFlip = 0;
		score = 0;
		Ship.destroy();
		Ship.create(ship_bitmaps[ship_now]);
		delete_blasts();
		delete_asteroids();
		delete_bonuses();
		delete_explosions();
		as = add_asteroid();
		if(!as) abort_example("Error creating asteroid");
		as->create(0, 4);
		as->obj.pos += Ship.pos;
		
		back_image = al_get_target_bitmap();
		al_set_target_bitmap(gameBgImage);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		kolGalaxies = random(3, 6);
		for(int u = 0; u < 6; u++)
		{
			for(int k = 0; k < 4; k++)
			{
				bMatr[u][k] = false;
			}
		}
		for(int u = 0; u < KOLGALAXY; u++)
		{
			bMass[u] = false;
		}
		
		while(kolGalaxiesDone < kolGalaxies)
		{
			randCoorX = random(0, 5); randCoorY = random(0, 3);
			if(!bMatr[randCoorX][randCoorY])
			{
				randNum = random(0, KOLGALAXY-1);
				while(bMass[randNum])
				{
					randNum = random(0, KOLGALAXY-1);
				}
				bitmapW = al_get_bitmap_width(galaxy_bitmaps[randNum]); bitmapH = al_get_bitmap_height(galaxy_bitmaps[randNum]);
				Sootn = MAX(bitmapW / (al_get_bitmap_width(gameBgImage) / 4.0), bitmapH / (al_get_bitmap_height(gameBgImage) / 4.0));
				if(Sootn > 1)
				{
					bitmapW /= Sootn; bitmapH /= Sootn;
				}
				al_draw_scaled_bitmap(galaxy_bitmaps[randNum], 0, 0, 
					al_get_bitmap_width(galaxy_bitmaps[randNum]), al_get_bitmap_height(galaxy_bitmaps[randNum]),
					randCoorX*(al_get_bitmap_width(gameBgImage) / 4.0), randCoorY*(al_get_bitmap_height(gameBgImage) / 4.0),
					bitmapW, bitmapH, 0);
				bMass[randNum] = true;
				bMatr[randCoorX][randCoorY] = true;
				kolGalaxiesDone++;
			}
		}
		randCoorX = random(0, 5); randCoorY = random(0, 3);
		while(bMatr[randCoorX][randCoorY])
		{
			randCoorX = random(0, 5); randCoorY = random(0, 3);
		}
		randNum = random(0, KOLPLANET-1);
		bitmapW = al_get_bitmap_width(planet_bitmaps[randNum]); bitmapH = al_get_bitmap_height(planet_bitmaps[randNum]);
		Sootn = MAX(bitmapW / (al_get_bitmap_width(gameBgImage) / 4.0), bitmapH / (al_get_bitmap_height(gameBgImage) / 4.0));
		if(Sootn > 1)
		{
			bitmapW /= Sootn; bitmapH /= Sootn;
		}
		al_draw_scaled_bitmap(planet_bitmaps[randNum], 0, 0, 
			al_get_bitmap_width(planet_bitmaps[randNum]), al_get_bitmap_height(planet_bitmaps[randNum]),
			randCoorX*(al_get_bitmap_width(gameBgImage) / 4.0), randCoorY*(al_get_bitmap_height(gameBgImage) / 4.0),
			bitmapW, bitmapH, 0);


		al_set_target_bitmap(back_image);
		InitBackground(gameBG, 0, 0, 0.5, 0.5, 0, 0, al_get_bitmap_width(gameBgImage), al_get_bitmap_height(gameBgImage), gameBgImage);
	case MENU:
		if(LastKeyKeyboad[ALLEGRO_KEY_ESCAPE])
			break;
	case PAUSE:
		set_playMode(PLAY);
		break;
	case PLAY:
		set_playMode(PAUSE);
		break;
	default:
		break;
	}
}
void options()
{
	lastPlayMode = iRejIg;
	set_playMode(OPTIONS);
}
void recordes()
{
	lastPlayMode = iRejIg;
	set_playMode(RECORDS);
}
void choice_gameMode()
{
	lastPlayMode = iRejIg;
	set_playMode(PLAYOPT);
}
void back()
{
	set_playMode(lastPlayMode);
}
void music_fix()
{
	knopka_music_fixX = cursor.x - menu_knopMass[KNOPKA_BEGYN_MUSIC]->x;
}
void soundEff_fix()
{
	knopka_soundEff_fixX = cursor.x - menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->x;
}
void time_fixX()
{
	knopka_time_fixX = cursor.x - menu_knopMass[KNOPKA_BEGYN_TIME]->x;
}
void music_next()
{
	al_stop_sample_instance(instances[backGroundMusic]);
	if(++backGroundMusic >= KOLSAMPLESMUSIC)
		backGroundMusic %= KOLSAMPLESMUSIC;
	al_play_sample_instance(instances[backGroundMusic]);
}
void music_last()
{
	al_stop_sample_instance(instances[backGroundMusic]);
	if(--backGroundMusic < 0){
		backGroundMusic %= KOLSAMPLESMUSIC;
		if(backGroundMusic < 0)backGroundMusic += KOLSAMPLESMUSIC;
	}
	al_play_sample_instance(instances[backGroundMusic]);
}
void ship_next()
{
	if(++ship_now >= KOLSHIPS) ship_now = 0;
	menu_knopMass[KNOPKA_LAST_SHIP]->x = W/2 - menu_knopMass[KNOPKA_LAST_SHIP]->w/2 - 5 - 
		al_get_bitmap_width((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_NEXT_SHIP]->x = W/2 + menu_knopMass[KNOPKA_NEXT_SHIP]->w/2 + 5 +
		al_get_bitmap_width((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_LAST_SHIP]->y = menu_knopMass[KNOPKA_START]->y - menu_knopMass[KNOPKA_START]->h/2 - 
		MAX(menu_knopMass[KNOPKA_LAST_SHIP]->h/2, al_get_bitmap_height((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0);
	menu_knopMass[KNOPKA_NEXT_SHIP]->y = menu_knopMass[KNOPKA_LAST_SHIP]->y;
	if(!ship_bitmaps[ship_now]) menu_knopMass[KNOPKA_START]->block = true;
	else menu_knopMass[KNOPKA_START]->block = false;
}
void ship_last()
{
	if(--ship_now < 0) ship_now = KOLSHIPS-1;
	menu_knopMass[KNOPKA_LAST_SHIP]->x = W/2 - menu_knopMass[KNOPKA_LAST_SHIP]->w/2 - 5 - 
		al_get_bitmap_width((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_NEXT_SHIP]->x = W/2 + menu_knopMass[KNOPKA_NEXT_SHIP]->w/2 + 5 +
		al_get_bitmap_width((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_LAST_SHIP]->y = menu_knopMass[KNOPKA_START]->y - menu_knopMass[KNOPKA_START]->h/2 - 
		MAX(menu_knopMass[KNOPKA_LAST_SHIP]->h/2, al_get_bitmap_height((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0);
	menu_knopMass[KNOPKA_NEXT_SHIP]->y = menu_knopMass[KNOPKA_LAST_SHIP]->y;
	if(!ship_bitmaps[ship_now]) menu_knopMass[KNOPKA_START]->block = true;
	else menu_knopMass[KNOPKA_START]->block = false;
}
void ship_next_rec()
{
	if(++ship_rec >= KOLSHIPS) ship_rec = 0;
	menu_knopMass[KNOPKA_LAST_SHIP_REC]->x = W/2 - menu_knopMass[KNOPKA_LAST_SHIP_REC]->w/2 - 5 - 
		al_get_bitmap_width((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_NEXT_SHIP_REC]->x = W/2 + menu_knopMass[KNOPKA_NEXT_SHIP_REC]->w/2 + 5 +
		al_get_bitmap_width((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_LAST_SHIP_REC]->y = menu_knopMass[KNOPKA_BACK]->y - menu_knopMass[KNOPKA_BACK]->h/2 - 
		MAX(menu_knopMass[KNOPKA_LAST_SHIP_REC]->h/2, 
			al_get_bitmap_height((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0) - 295;
	menu_knopMass[KNOPKA_NEXT_SHIP_REC]->y = menu_knopMass[KNOPKA_LAST_SHIP_REC]->y;
}
void ship_last_rec()
{
	if(--ship_rec < 0) ship_rec = KOLSHIPS-1;
	menu_knopMass[KNOPKA_LAST_SHIP_REC]->x = W/2 - menu_knopMass[KNOPKA_LAST_SHIP_REC]->w/2 - 5 - 
		al_get_bitmap_width((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_NEXT_SHIP_REC]->x = W/2 + menu_knopMass[KNOPKA_NEXT_SHIP_REC]->w/2 + 5 +
		al_get_bitmap_width((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0;
	menu_knopMass[KNOPKA_LAST_SHIP_REC]->y = menu_knopMass[KNOPKA_BACK]->y - menu_knopMass[KNOPKA_BACK]->h/2 - 
		MAX(menu_knopMass[KNOPKA_LAST_SHIP_REC]->h/2, 
			al_get_bitmap_height((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0) - 295;
	menu_knopMass[KNOPKA_NEXT_SHIP_REC]->y = menu_knopMass[KNOPKA_LAST_SHIP_REC]->y;
}

void set_playMode(int m)
{
	switch(iRejIg)
	{
	case PLAY:
		break;
	case PAUSE:
		menu_knopMass[KNOPKA_CONTINUE]->show = false;
	case ENDGAME:
	case MENU:
		if(iRejIg != PAUSE) menu_knopMass[KNOPKA_PLAY]->show = false;
		menu_knopMass[KNOPKA_OPT]->show = false;
		menu_knopMass[KNOPKA_REC]->show = false;
		menu_knopMass[KNOPKA_QUIT]->show = false;
		break;
	case OPTIONS:
		menu_knopMass[KNOPKA_NEXT]->show = false;
		menu_knopMass[KNOPKA_LAST]->show = false;
		menu_knopMass[KNOPKA_BEGYN_TIME]->show = false;
		menu_knopMass[KNOPKA_BEGYN_MUSIC]->show = false;
		menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->show = false;
		menu_knopMass[KNOPKA_BACK]->show = false;
		break;
	case RECORDS:
		menu_knopMass[KNOPKA_BACK]->show = false;
		menu_knopMass[KNOPKA_BACK]->x = back_coord.x;
		menu_knopMass[KNOPKA_BACK]->y = back_coord.y;
		menu_knopMass[KNOPKA_LAST_SHIP_REC]->show = false;
		menu_knopMass[KNOPKA_NEXT_SHIP_REC]->show = false;
		break;
	case PLAYOPT:
		menu_knopMass[KNOPKA_LAST_SHIP]->show = false;
		menu_knopMass[KNOPKA_NEXT_SHIP]->show = false;
		menu_knopMass[KNOPKA_START]->show = false;
		menu_knopMass[KNOPKA_BACK]->show = false;
		break;
	default:
		break;
	}

	iRejIg = m;
	switch(iRejIg)
	{
	case PLAY:
		InitBackgroundSize(W + 600, H + 600);
		break;
	case PAUSE:
	case ENDGAME:
	case MENU:
		switch(iRejIg)
		{
		case PAUSE:
			menu_knopMass[KNOPKA_CONTINUE]->show = true;
			break;
		case ENDGAME:
			delete_asteroids();
			delete_bonuses();
			delete_explosions();
			delete_blasts();
			Ship.destroy();
		case MENU:
			menu_knopMass[KNOPKA_PLAY]->show = true;
			break;
		}
		InitBackgroundSize(W, H);
		menu_knopMass[KNOPKA_OPT]->show = true;
		menu_knopMass[KNOPKA_REC]->show = true;
		menu_knopMass[KNOPKA_QUIT]->show = true;
		break;
	case OPTIONS:
		menu_knopMass[KNOPKA_NEXT]->show = true;
		menu_knopMass[KNOPKA_LAST]->show = true;
		menu_knopMass[KNOPKA_BEGYN_TIME]->show = true;
		menu_knopMass[KNOPKA_BEGYN_MUSIC]->show = true;
		menu_knopMass[KNOPKA_BEGYN_SOUND_EFF]->show = true;
		menu_knopMass[KNOPKA_BACK]->show = true;
		break;
	case RECORDS:
		menu_knopMass[KNOPKA_BACK]->show = true;
		back_coord = tochka(menu_knopMass[KNOPKA_BACK]->x, menu_knopMass[KNOPKA_BACK]->y);
		menu_knopMass[KNOPKA_BACK]->y += 150 ;
		menu_knopMass[KNOPKA_LAST_SHIP_REC]->show = true;
		menu_knopMass[KNOPKA_NEXT_SHIP_REC]->show = true;
		menu_knopMass[KNOPKA_LAST_SHIP_REC]->x = W/2 - menu_knopMass[KNOPKA_LAST_SHIP_REC]->w/2 - 5 - 
			al_get_bitmap_width((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0;
		menu_knopMass[KNOPKA_NEXT_SHIP_REC]->x = W/2 + menu_knopMass[KNOPKA_NEXT_SHIP_REC]->w/2 + 5 +
			al_get_bitmap_width((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0;
		menu_knopMass[KNOPKA_LAST_SHIP_REC]->y = menu_knopMass[KNOPKA_BACK]->y - menu_knopMass[KNOPKA_BACK]->h/2 - 
			MAX(menu_knopMass[KNOPKA_LAST_SHIP_REC]->h/2, 
				al_get_bitmap_height((ship_bitmaps[ship_rec]) ? (ship_bitmaps[ship_rec]) : (ship_bitmaps[0]))/2.0) - 295;
		menu_knopMass[KNOPKA_NEXT_SHIP_REC]->y = menu_knopMass[KNOPKA_LAST_SHIP_REC]->y;
		break;
	case PLAYOPT:
		menu_knopMass[KNOPKA_LAST_SHIP]->show = true;
		menu_knopMass[KNOPKA_LAST_SHIP]->x = W/2 - menu_knopMass[KNOPKA_LAST_SHIP]->w/2 - 5 - 
			al_get_bitmap_width((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0;
		menu_knopMass[KNOPKA_LAST_SHIP]->y = menu_knopMass[KNOPKA_START]->y - menu_knopMass[KNOPKA_START]->h/2 - 
			MAX(menu_knopMass[KNOPKA_LAST_SHIP]->h/2, al_get_bitmap_height((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0);
		menu_knopMass[KNOPKA_NEXT_SHIP]->show = true;
		menu_knopMass[KNOPKA_NEXT_SHIP]->x = W/2 + menu_knopMass[KNOPKA_NEXT_SHIP]->w/2 + 5 +
			al_get_bitmap_width((ship_bitmaps[ship_now]) ? (ship_bitmaps[ship_now]) : (ship_bitmaps[0]))/2.0;
		menu_knopMass[KNOPKA_NEXT_SHIP]->y = menu_knopMass[KNOPKA_LAST_SHIP]->y;
		menu_knopMass[KNOPKA_START]->show = true;
		menu_knopMass[KNOPKA_BACK]->show = true;
		break;
	default:
		break;
	}
}

void load_flip_reset(int kol)
{
	kolMax = kol; kolNow = -1;
}
void load_flip()
{
	draw_textf(FontEn30, FontRu30, colors[BLASTEROIDES], W, H-30, ALLEGRO_ALIGN_RIGHT, "%0.0f%%", ++kolNow / kolMax * 100);
	al_flip_display();
	al_draw_filled_rectangle(0, H-30, W, H, colors[BLACK]);
}

void UpdateMuisc()
{
	if(!al_get_sample_instance_playing(instances[backGroundMusic]))
	{
		al_stop_sample_instance(instances[backGroundMusic]);
		if(++backGroundMusic >= KOLSAMPLESMUSIC)
			backGroundMusic %= KOLSAMPLESMUSIC;
		al_play_sample_instance(instances[backGroundMusic]);
	}
}

void Bonus_Heal()
{
	switch(Bonus_act.lvl)
	{
	case 1:
		Ship.obj.health += 25;
		break;
	case 2:
		Ship.obj.health += 50;
		break;
	case 3:
		if(Ship.obj.health == Ship.max_health) Ship.lives++;
		else Ship.obj.health += 100;
		break;
	default:
		break;
	}
	if(Ship.obj.health > Ship.max_health)
	{
		Ship.obj.health = Ship.max_health;
	}
}
void Bonus_Shield()
{
	if(!Ship.shield) Ship.obj.r += 17;
	switch(Bonus_act.lvl)
	{
	case 1:
		Ship.shield += 100;
		break;
	case 2:
		Ship.shield += 250;
		break;
	case 3:
		Ship.shield += 500;
		break;
	default:
		break;
	}
	if(Ship.shield > 1000)
	{
		Ship.shield = 1000;
	}
}
void Bonus_Money()
{
	switch(Bonus_act.lvl)
	{
	case 1:
		score += 1000;
		break;
	case 2:
		score += 2000;
		break;
	case 3:
		score += 5000;
		break;
	default:
		break;
	}
}
void Bonus_Power()
{
	switch(Bonus_act.lvl)
	{
	case 1:
		Ship.power += 12.5;
		break;
	case 2:
		Ship.power += 25;
		break;
	case 3:
		Ship.power += 50;
		break;
	default:
		break;
	}
	if(Ship.power >= 100)
	{
		Ship.power = 100;
	}
}
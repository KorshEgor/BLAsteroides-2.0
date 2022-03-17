#include "Headers.h"
#include "functions.h"
#include "MyGeometry.h"
#include "EgorColor.h"

#ifndef _SPACE_LIB  // флаг подключения
#define _SPACE_LIB

#define KOLSHIPS 10
#define KOLGALAXY 17
#define KOLPLANET 5
#define KOLASTEROIDS 4
#define KOLEXPLOSION 3
#define KOLBONUS 4
#define KOOFOTD 20

class Spaceship;
class Asteroid;
class Explosion;
class Bonus;
class Blast;

Asteroid* add_asteroid();
int delete_asteroid(Asteroid *as);
void delete_asteroids();

Explosion* add_explosion();
int delete_explosion(Explosion *ex);
void delete_explosions();

Blast* add_blast();
int delete_blast(Blast *bl);
void delete_blasts();

Bonus* add_bonus();
int delete_bonus(Bonus *bl);
void delete_bonuses();

class Object{
public:
	tochka pos, size;
	int health, damage;
	float r;
	int Rejim;
	int kolToch;
	tochka *toch;
	bool gone;
	void create(tochka position, tochka siZe, float R, int rejim, int Health, int Damage, int koltoch = 0, tochka *t = NULL)
	{
		if(koltoch != 0)
		{
			toch = new tochka[koltoch];
			if(!toch)
			{
				abort_example("Error creating object");
			}
			for(int p = 0; p < koltoch; p++)
			{
				toch[p] = t[p];
			}
		}else toch = NULL;
		kolToch = koltoch; pos = position; size = siZe; r = R; Rejim = rejim; health = Health; damage = Damage; gone = false;
	}
	int collide(Object obj, tochka **pol, int sizePol, int flip)
	{
		line lGran, perp;
		float len;
		if(lenFrom1to2(pos.x, pos.y, obj.pos.x, obj.pos.y) < r + obj.r)
		{
			if(pol)
			{
				bool collide = false;
				int kol = 0;
				tochka ret;
				for(int u = 0; u < sizePol; u++)
				{
					lGran.init(pol[flip][u] + obj.pos, pol[flip][(u+1) % sizePol] + obj.pos);
					perp = lGran.get_perp(pos);
					if(lGran.vhojd_tochki(perp.t2))
					{
						if(perp.len < r)
						{
							collide = true;
							break;
						}
					}else{
						len = MIN(lenFrom1to2(lGran.t1.x, lGran.t1.y, pos.x,  pos.y), 
							lenFrom1to2(lGran.t2.x, lGran.t2.y, pos.x,  pos.y));
						if(len < r)
						{
							collide = true;
							break;
						}
					}
					perp.init(pos, tochka(pos.x+1000, pos.y+1000));
					if(lGran.check_collide(perp, ret))
					{
						kol++;
					}
				}
				if(!collide)
				{
					collide = kol % 2;
				}
				return collide;
			}else 
				return 1;
		}else{
			return 0;
		}
	}
	void destroy()
	{
		delete toch;
	}
};

ALLEGRO_BITMAP *ship_bitmaps[KOLSHIPS];
int ship_max_health[KOLSHIPS] = {100, 75, 0, 0, 0, 0, 0, 0, 0, 0,};
int ship_max_lives[KOLSHIPS] = {3, 2, 0, 0, 0, 0, 0, 0, 0, 0,};

ALLEGRO_BITMAP *galaxy_bitmaps[KOLGALAXY];
ALLEGRO_BITMAP *planet_bitmaps[KOLPLANET];

int ship_now = 0, ship_rec = 0;

ALLEGRO_FONT *FontEn12;
enum BONUSES{HEAL, SHIELD, MONEY};
ALLEGRO_BITMAP *Bonus_bitmaps[KOLBONUS];

enum EXPLOSIONS{EXASRED, EXASBLUE, EXSHIP};
ALLEGRO_BITMAP *Explosion_bitmaps[KOLEXPLOSION];
char *Explosion_bitmaps_names[KOLEXPLOSION] = {
	"explosion_red.png",
	"explosion_blue.png",
	"explosion_ship.png",
};
int Explosion_flips_in_bitmap[KOLEXPLOSION][3] = {
	{3, 4, 10},
	{3, 4, 11},
	{11, 1, 10},
};
float Explosion_size[KOLEXPLOSION][4] = {
	{120, 120, 100, 100}, 
	{120, 120, 100, 100}, 
	{130, 130, 100, 100}, 
};

ALLEGRO_BITMAP *Asteroid_bitmaps[KOLASTEROIDS];
int Asteroid_flips_in_bitmap[KOLASTEROIDS][3] = {
	{20, 1, 20},
	{25, 1, 25},
	{20, 1, 20},
	{15, 2, 30},
};
float Asteroid_rad[KOLASTEROIDS][4] = {
	{25, 50, 75, 100},
	{25, 50, 75, 100},
	{25, 50, 75, 100},
	{25, 50, 75, 100},
};
float Asteroid_heal[KOLASTEROIDS][4] = {
	{1, 1, 3, 4},
	{1, 1, 3, 4},
	{1, 1, 3, 4},
	{2, 2, 6, 8},
};
float Asteroid_size[KOLASTEROIDS][4] = {
	{200, 200, 100, 100}, 
	{320, 240, 133, 100}, 
	{200, 200, 100, 100}, 
	{104, 108, 104, 108}, 
};
tochka **Asteroid_col_polygon[KOLASTEROIDS];
int *Asteroid_col_polygon_size[KOLASTEROIDS];

class Spaceship{
public:
	line track1[60];
	line track2[60];
	Object obj;
	int kolTrackes;
	int max_health, lives, max_lives, immortal_len;
	ALLEGRO_BITMAP *bitmap;
	tochka pos;
	line speed;
	float angle;
	float shield;
	long float power;
	tochka coordTrackStart[2];
	tochka coordTrack[2];
	
	void create(ALLEGRO_BITMAP* image = NULL, tochka position = tNull, float Angle360 = 0, tochka sp = tNull)
	{
		bitmap = image; pos = position; angle = Angle360; max_health = ship_max_health[ship_now]; lives = max_lives = ship_max_lives[ship_now];
		speed.init(tNull, sp);
		power = shield = kolTrackes = immortal_len = 0;

		coordTrack[0] = coordTrackStart[0] = tochka(-21, -30);
		coordTrack[1] = coordTrackStart[1] = tochka(-21, 30);

		float x1 = coordTrack[0].x, y1 = coordTrack[0].y, x2 = coordTrack[1].x, y2 = coordTrack[1].y;
		ALLEGRO_TRANSFORM transform;

		obj.create(pos, (bitmap) ? (tochka(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap))) : (tNull),
			(bitmap) ? (al_get_bitmap_width(bitmap)/2): (30), CIRCLE, ship_max_health[ship_now], 0);

		al_identity_transform(&transform);
		al_rotate_transform(&transform, angle/FROMRADTOGRAD);
		al_transform_coordinates(&transform, &x1, &y1);
		al_transform_coordinates(&transform, &x2, &y2);

		coordTrack[0] = tochka(x1, y1); coordTrack[1] = tochka(x2, y2);

		for(int p = 0; p < 60; p++)
		{
			track1[p].init(pos + coordTrack[0], pos + coordTrack[0]);
			track2[p].init(pos + coordTrack[1], pos + coordTrack[1]);
		}
	}
	void respawn(tochka position = tNull, float Angle360 = 0, tochka sp = tNull)
	{
		pos = position; angle = Angle360; speed.init(tNull, sp); shield = kolTrackes = 0;
		coordTrack[0] = coordTrackStart[0] = tochka(-21, -30);
		coordTrack[1] = coordTrackStart[1] = tochka(-21, 30);

		float x1 = coordTrack[0].x, y1 = coordTrack[0].y, x2 = coordTrack[1].x, y2 = coordTrack[1].y;
		ALLEGRO_TRANSFORM transform;

		obj.pos = position; obj.health = ship_max_health[ship_now];
		al_identity_transform(&transform);
		al_rotate_transform(&transform, angle/FROMRADTOGRAD);
		al_transform_coordinates(&transform, &x1, &y1);
		al_transform_coordinates(&transform, &x2, &y2);

		coordTrack[0] = tochka(x1, y1); coordTrack[1] = tochka(x2, y2);

		for(int p = 0; p < 60; p++)
		{
			track1[p].init(pos + coordTrack[0], pos + coordTrack[0]);
			track2[p].init(pos + coordTrack[1], pos + coordTrack[1]);
		}
		immortal_len = 4*SECOND;
		lives--;
	}
	void destroy()
	{
		obj.destroy();
	}
	void draw(ALLEGRO_COLOR color = colors[WHITE])
	{
		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_translate_transform(&transform, W/2 - speed.t2.x*KOOFOTD, H/2 - speed.t2.y*KOOFOTD);
		al_use_transform(&transform);

		if(kolTrackes != 0)
		{
			for(int p = 0; p < 60; p++)
			{
				if(track1[p].t1 != track1[p].t2)
				{
					int r = 255 + 12750 / (p-110.0), g, b, a;

					a = g = b = r;

					track1[p].t1 -= pos; track1[p].t2 -= pos;
					track1[p].draw(al_map_rgba(r, g, b, a), (60-p)/20.0);
					track1[p].t1 += pos; track1[p].t2 += pos;
				}
				if(track2[p].t1 != track2[p].t2)
				{
					int r = 255 + 12750 / (p-110.0), g, b, a;

					a = g = b = r;

					track2[p].t1 -= pos; track2[p].t2 -= pos;
					track2[p].draw(al_map_rgba(r, g, b, a), (60-p)/20.0);
					track2[p].t1 += pos; track2[p].t2 += pos;
				}
			}
		}

		if(bitmap != NULL) al_draw_tinted_rotated_bitmap(bitmap, color, al_get_bitmap_width(bitmap)/2.0, 
																		al_get_bitmap_height(bitmap)/2.0, 0, 0,angle/FROMRADTOGRAD, 0);
		else al_draw_filled_rectangle(-20, -20, 20, 20, colors[RED]);
		if (shield)
		{
			al_draw_filled_circle(0, 0, obj.r-12, al_map_rgba(0, 85, 155, 33));
			al_draw_circle(0, 0, obj.r-9, al_map_rgba(0, 65, 117, 80), 6);
			al_draw_circle(0, 0, obj.r-3, al_map_rgba(0, 55, 100, 127), 6);
		}

		al_identity_transform(&transform);
		al_use_transform(&transform);

	}
	void move(){
		if(track1[59].t1 != track1[59].t2) kolTrackes--;
		for(int p = 59; p > 0; p--)
		{
			track1[p] = track1[p-1];
			track2[p] = track2[p-1];
		}
		pos += speed.t2;
		obj.pos = pos;
		tochka t[2][2] = {
			{track1[1].t2, pos+coordTrack[0]},
			{track2[1].t2, pos+coordTrack[1]},
		};
		if(t[0][0] != t[0][1]) kolTrackes++;
		track1[0].init(t[0][0], t[0][1]);
		track2[0].init(t[1][0], t[1][1]);
		/*
		tochka t1 = pos, t2;
		t2 = pos;
		if(t1 != t2) kolTrackes++;
		track[0].init(t1, t2);*/
	}
	void use_trans()
	{
		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_translate_transform(&transform, pos.x*(-1) + W/2 - speed.t2.x*KOOFOTD, pos.y*(-1) + H/2 - speed.t2.y*KOOFOTD);
		al_use_transform(&transform);
	}
	void rotate(float angle360)
	{
		angle += angle360;
		if(angle > 360){
			int c = angle;
			c = c/360;
			angle -= 360*c;
		}else if(angle < 0){
			int c = angle;
			c = c/360 - 1;
			angle -= 360*c;
		}
		coordTrack[0] = coordTrackStart[0];
		coordTrack[1] = coordTrackStart[1];

		float x1 = coordTrack[0].x, y1 = coordTrack[0].y, x2 = coordTrack[1].x, y2 = coordTrack[1].y;

		ALLEGRO_TRANSFORM transform;

		al_identity_transform(&transform);
		al_rotate_transform(&transform, angle/FROMRADTOGRAD);
		al_transform_coordinates(&transform, &x1, &y1);
		al_transform_coordinates(&transform, &x2, &y2);

		coordTrack[0] = tochka(x1, y1); coordTrack[1] = tochka(x2, y2);
	}
};
class Asteroid{
public:
	Object obj;
	ALLEGRO_BITMAP *bitmap;
	line speed;
	tochka **col_polygon;
	float angle;
	int type;
	int lvl;
	float flip;
	float flip_speed;
	Asteroid *next, *last;
	
	void create(int Type = random(0, KOLASTEROIDS-1), int Lvl = random(1, 4), tochka pos = tNull, tochka sp = tNull)
	{
		ALLEGRO_TRANSFORM transform;
		type = Type; lvl = Lvl;

		float fSp, Angle = random(1, 360);
		if(pos == tNull)
		{
			if(random(0,1)){
				pos.x = random(-W, W); pos.y = random(0,1)*(H*2)-H;
			}else{
				pos.x = random(0,1)*(W*2)-W; pos.y = random(-H, H);
			}
		}

		if(sp == tNull)
		{
			fSp = random(20, 40) / 10.0;
			sp.x = fSp * cos(Angle / FROMRADTOGRAD); sp.y = fSp * sin(Angle / FROMRADTOGRAD);
		}
		speed.init(tNull, sp);

		bitmap = Asteroid_bitmaps[type]; angle = random(1, 360); flip = 0; flip_speed = Asteroid_flips_in_bitmap[type][2] / SECOND;

		obj.create(pos, tochka(al_get_bitmap_width(Asteroid_bitmaps[type]) / Asteroid_flips_in_bitmap[type][0], 
			al_get_bitmap_height(Asteroid_bitmaps[type]) / Asteroid_flips_in_bitmap[type][1]), Asteroid_rad[type][lvl-1], CIRCLE, 
			Asteroid_heal[type][lvl-1], lvl*25);

		if(type != KOLASTEROIDS-1)
		{
			col_polygon = new tochka*[Asteroid_flips_in_bitmap[type][2]];
			if(!col_polygon) abort_example("Error new memory");
			for(int u = 0; u < Asteroid_flips_in_bitmap[type][2]; u++)
			{
				col_polygon[u] = new tochka[Asteroid_col_polygon_size[type][u]];
				if(!col_polygon[u]) abort_example("Error new memory");
				for(int p = 0; p < Asteroid_col_polygon_size[type][u]; p++)
				{
					float x1 = Asteroid_col_polygon[type][u][p].x, y1 =  Asteroid_col_polygon[type][u][p].y;
					al_identity_transform(&transform);
					al_rotate_transform(&transform, angle/FROMRADTOGRAD);
					al_scale_transform(&transform, (Asteroid_size[type][2] / Asteroid_size[type][0])*lvl/2.0, 
													(Asteroid_size[type][3] / Asteroid_size[type][1])*lvl/2.0);
					al_transform_coordinates(&transform, &x1,  &y1);
					col_polygon[u][p] = tochka(x1, y1);
				}
			}
		}else{
			col_polygon = NULL;
		}
	}
	void destroy()
	{
		if(type != KOLASTEROIDS-1)
		{
			if(col_polygon)
			{
				for(int u = 0; u < Asteroid_flips_in_bitmap[type][2]; u++)
				{
					if(col_polygon[u])
						delete col_polygon[u];
				}
				delete col_polygon;
			}
		}
	}
	void draw(ALLEGRO_COLOR color = colors[WHITE])
	{
		al_draw_tinted_scaled_rotated_bitmap_region(bitmap, (int)flip % Asteroid_flips_in_bitmap[type][0] * obj.size.x, 
															(int)flip / Asteroid_flips_in_bitmap[type][0] * obj.size.y, 
															obj.size.x, obj.size.y, colors[WHITE], obj.size.x/2, obj.size.y/2,
															obj.pos.x, obj.pos.y, (Asteroid_size[type][2] / Asteroid_size[type][0])*lvl/2.0, 
																					(Asteroid_size[type][3] / Asteroid_size[type][1])*lvl/2.0, angle/FROMRADTOGRAD, 0);
	}
	void move()
	{
		obj.pos += speed.t2;
	}
	void update_flip()
	{
		flip += flip_speed;
		if(flip >= Asteroid_flips_in_bitmap[type][2])
		{
			flip -= Asteroid_flips_in_bitmap[type][2];
		}
	}
};
class Bonus{
public:
	Object obj;
	ALLEGRO_BITMAP *bitmap;
	int type, lvl, now;
	Bonus *next, *last;
	void (*function)();
	
	void create(int Type, int Lvl, tochka pos, void (*Function)())
	{
		type = Type; lvl = Lvl; now = 0; bitmap = Bonus_bitmaps[Type]; now = 0; function = Function;
		obj.create(pos, tochka(al_get_bitmap_width(Bonus_bitmaps[type]), 
			al_get_bitmap_height(Bonus_bitmaps[type])), 12, CIRCLE, 1, 0);
	}
	void destroy()
	{
		return;
	}
	void draw(ALLEGRO_COLOR color = colors[WHITE])
	{
		al_draw_rotated_bitmap(bitmap, al_get_bitmap_width(bitmap)/2.0, al_get_bitmap_height(bitmap)/2.0,
			 obj.pos.x, obj.pos.y, 0, 0);
		al_draw_textf(FontEn12, colors[BLACK], obj.pos.x, obj.pos.y-10, ALLEGRO_ALIGN_CENTRE, "%i", lvl);
	}
	void update()
	{
		now++;
	}
};
class Explosion{
public:
	ALLEGRO_BITMAP *bitmap;
	tochka pos;
	tochka size;
	float angle;
	int type;
	int lvl;
	float flip;
	float flip_speed;
	Explosion *next, *last;
	bool gone;
	
	void create(int Type, int Lvl, tochka position, float Angle)
	{
		type = Type; lvl = Lvl; angle = Angle; flip = 0; pos = position; gone = false;
		bitmap = Explosion_bitmaps[type]; flip_speed = Explosion_flips_in_bitmap[type][2] / (SECOND*0.8);
		size = tochka(al_get_bitmap_width(bitmap) / Explosion_flips_in_bitmap[type][0], 
			al_get_bitmap_height(bitmap) / Explosion_flips_in_bitmap[type][1]);
	}
	void destroy()
	{
		return;
	}
	void draw(ALLEGRO_COLOR color = colors[WHITE])
	{
		al_draw_tinted_scaled_rotated_bitmap_region(bitmap, (int)flip % Explosion_flips_in_bitmap[type][0] * size.x, 
															(int)flip / Explosion_flips_in_bitmap[type][0] * size.y, 
															size.x, size.y, colors[WHITE], size.x/2, size.y/2,
															pos.x, pos.y, (size.x / Explosion_size[type][0])*lvl/2.0, 
																					(size.y / Explosion_size[type][1])*lvl/2.0, angle/FROMRADTOGRAD, 0);
	}
	void update_flip()
	{
		flip += flip_speed;
		if(flip >= Explosion_flips_in_bitmap[type][2])
		{
			gone = true;
		}
	}
};
class Blast{
public:
	Object obj;
	line speed;
	float now, angle;
	Blast *next, *last;
	void create(Spaceship s)
	{
		speed.init(tNull, tochka(cos(s.angle / FROMRADTOGRAD)*5.5, sin(s.angle / FROMRADTOGRAD)*5.5) + s.speed.t2);
		obj.create(s.pos, tochka(1, 1), 2, CIRCLE, 1, 1);
		now = 0; angle = s.angle;
	}
	void move()
	{
		obj.pos += speed.t2;
		now += 5.5;
	}
	void draw()
	{
		float x1 = 5.5/2 * cos(angle/FROMRADTOGRAD), y1 = 5.5/2 * sin(angle/FROMRADTOGRAD);
		al_draw_line(obj.pos.x - x1, obj.pos.y - y1, obj.pos.x + x1,  obj.pos.y + y1, colors[GREEN], 3);
	}
	void destroy()
	{
		return;
	}
};

Asteroid *Asteroids = NULL, *Asteroid_last = NULL, *Asteroid_added = NULL;
int kol_Asteroids = 0;
Explosion *Explosions = NULL, *Explosion_last = NULL, *Explosion_added = NULL;
int kol_Explosions = 0;
Bonus *Bonuses = NULL, *Bonus_last = NULL, *Bonus_added = NULL;
int kol_Bonuses = 0;
Blast *Blasts = NULL, *Blast_last = NULL, *Blast_added = NULL;
int kol_Blasts = 0;
Spaceship Ship;


Asteroid* add_asteroid()
{
	if(kol_Asteroids == 0){
		Asteroids = new Asteroid;
		if(!Asteroids) return NULL;
		Asteroid_last = Asteroids;
		Asteroids->last = Asteroids->next = NULL;
	}else{
		Asteroid_added = new Asteroid;
		if(!Asteroid_added) return NULL;
		Asteroid_last->next = Asteroid_added;
		Asteroid_added->last = Asteroid_last;
		Asteroid_last = Asteroid_added;
		Asteroid_last->next = NULL;
		Asteroid_added = NULL;
	}
	kol_Asteroids++;
	return Asteroid_last;
}
int delete_asteroid(Asteroid *as)
{
	if(kol_Asteroids == 0) return -1;
	as->destroy();
	if(as->last == NULL){
		Asteroids = as->next;
	}else{
		as->last->next = as->next;
	}if(as->next == NULL){
		Asteroid_last = as->last;
	}else{
		as->next->last = as->last;
	}
	delete as;
	kol_Asteroids--;
	return 0;
}
void delete_asteroids()
{
	while(Asteroids != NULL){
		delete_asteroid(Asteroids);
	}
}

Explosion* add_explosion()
{
	if(kol_Explosions == 0){
		Explosions = new Explosion;
		if(!Explosions) return NULL;
		Explosion_last = Explosions;
		Explosions->last = Explosions->next = NULL;
	}else{
		Explosion_added = new Explosion;
		if(!Explosion_added) return NULL;
		Explosion_last->next = Explosion_added;
		Explosion_added->last = Explosion_last;
		Explosion_last = Explosion_added;
		Explosion_last->next = NULL;
		Explosion_added = NULL;
	}
	kol_Explosions++;
	return Explosion_last;
}
int delete_explosion(Explosion *ex)
{
	if(kol_Explosions == 0) return -1;
	ex->destroy();
	if(ex->last == NULL){
		Explosions = ex->next;
	}else{
		ex->last->next = ex->next;
	}if(ex->next == NULL){
		Explosion_last = ex->last;
	}else{
		ex->next->last = ex->last;
	}
	delete ex;
	kol_Explosions--;
	return 0;
}
void delete_explosions()
{
	while(Explosions != NULL){
		delete_explosion(Explosions);
	}
}

Blast* add_blast()
{
	if(kol_Blasts == 0){
		Blasts = new Blast;
		if(!Blasts) return NULL;
		Blast_last = Blasts;
		Blasts->last = Blasts->next = NULL;
	}else{
		Blast_added = new Blast;
		if(!Blast_added) return NULL;
		Blast_last->next = Blast_added;
		Blast_added->last = Blast_last;
		Blast_last = Blast_added;
		Blast_last->next = NULL;
		Blast_added = NULL;
	}
	kol_Blasts++;
	return Blast_last;
}
int delete_blast(Blast *bl)
{
	if(kol_Blasts == 0) return -1;
	bl->destroy();
	if(bl->last == NULL){
		Blasts = bl->next;
	}else{
		bl->last->next = bl->next;
	}if(bl->next == NULL){
		Blast_last = bl->last;
	}else{
		bl->next->last = bl->last;
	}
	delete bl;
	kol_Blasts--;
	return 0;
}
void delete_blasts()
{
	while(Blasts != NULL){
		delete_blast(Blasts);
	}
}

Bonus* add_bonus()
{
	if(kol_Bonuses == 0){
		Bonuses = new Bonus;
		if(!Bonuses) return NULL;
		Bonus_last = Bonuses;
		Bonuses->last = Bonuses->next = NULL;
	}else{
		Bonus_added = new Bonus;
		if(!Bonus_added) return NULL;
		Bonus_last->next = Bonus_added;
		Bonus_added->last = Bonus_last;
		Bonus_last = Bonus_added;
		Bonus_last->next = NULL;
		Bonus_added = NULL;
	}
	kol_Bonuses++;
	return Bonus_last;
}
int delete_bonus(Bonus *bl)
{
	if(kol_Bonuses == 0) return -1;
	bl->destroy();
	if(bl->last == NULL){
		Bonuses = bl->next;
	}else{
		bl->last->next = bl->next;
	}if(bl->next == NULL){
		Bonus_last = bl->last;
	}else{
		bl->next->last = bl->last;
	}
	delete bl;
	kol_Bonuses--;
	return 0;
}
void delete_bonuses()
{
	while(Bonuses != NULL){
		delete_bonus(Bonuses);
	}
}
#endif
#include "Headers.h"
#include "functions.h"

#ifndef _KNOPKI_LIB  // флаг подключения
#define _KNOPKI_LIB
class knopka;

void nathing();
knopka *knopki = NULL, *last = NULL, *Rez = NULL;
void collide_knopki();
void use_knopki();
void update_knopki();
void view_knopki();
void delete_knopki();
int delete_knopka(knopka *kn);

class knopka
{
public:
	ALLEGRO_BITMAP *bitmap_view;
	ALLEGRO_BITMAP *bitmap_block;
	ALLEGRO_BITMAP *bitmap_collide;
	ALLEGRO_BITMAP *bitmap_no_collide;
	int x, y, w, h, r, iRejim, iKey;
	bool show, block, collide;
	void (*function)();
	knopka *last, *next;

	void view()
	{
		if(show)
		{
			al_draw_bitmap(bitmap_view, x - w/2, y - h/2, 0);
		}
	}
	void update()
	{
		if(block) {bitmap_view = bitmap_block;}
		else if(collide) {bitmap_view = bitmap_collide;}
		else bitmap_view = bitmap_no_collide;
		w = al_get_bitmap_width(bitmap_view);
		h = al_get_bitmap_height(bitmap_view);
		return;
	}
	void create(int x1, int y1, int r1, int Rejim, ALLEGRO_BITMAP *bitmap_collide2, ALLEGRO_BITMAP *bitmap_no_collide2, ALLEGRO_BITMAP *bitmap_block2, void (*function2)() = nathing)
	{
		bitmap_collide = bitmap_collide2;
		bitmap_no_collide = bitmap_no_collide2;
		bitmap_block = bitmap_block2;
		w = al_get_bitmap_width(bitmap_no_collide); h = al_get_bitmap_height(bitmap_no_collide);
		x = x1; y = y1; r = r1; iRejim = Rejim; show = true;
		/*al_convert_mask_to_alpha(bitmap_collide, al_map_rgb(255, 255, 255));
		al_convert_mask_to_alpha(bitmap_no_collide, al_map_rgb(255, 255, 255));
		al_convert_mask_to_alpha(bitmap_block, al_map_rgb(255, 255, 255));*/
		bitmap_view = bitmap_no_collide;
		function = function2;
		iKey = -1;
		block = false;
		collide = false;
	}
	void destroy()
	{
		al_destroy_bitmap(bitmap_collide);
		al_destroy_bitmap(bitmap_no_collide);
		al_destroy_bitmap(bitmap_block);
	}
};

int iKolKnop = 0;

knopka* add_knopka()
{
	if(iKolKnop == 0){
		knopki = new knopka;
		if(!knopki) return NULL;
		last = knopki;
		knopki->last = knopki->next = NULL;
	}else{
		Rez = new knopka;
		if(!Rez) return NULL;
		last->next = Rez;
		Rez->last = last;
		last = Rez;
		last->next = NULL;
		Rez = NULL;
	}
	iKolKnop++;
	return last;
}

int delete_knopka(knopka *kn)
{
	if(iKolKnop == 0) return -1;
	kn->destroy();
	if(kn->last == NULL){
		knopki = kn->next;
	}else{
		kn->last->next = kn->next;
	}if(kn->next == NULL){
		last = kn->last;
	}else{
		kn->next->last = kn->last;
	}
	delete kn;
	iKolKnop--;
	return 0;
}

void delete_knopki()
{
	while(knopki != NULL){
		delete_knopka(knopki);
	}
}

void nathing()
{
	return;
}
void collide_knopki()
{
	bool collide = false;
	knopka *cur = last;
	while(cur != NULL)
	{
		if(cur->iKey != -1 && LastKeyKeyboad[cur->iKey])
		{
			cur->collide = 1;
			cur = cur->last;
			continue;
		}
		if(!collide)
		{
			if(cur->show && !cur->block)
			{
				if(cur->iRejim == CIRCLE){
					cur->collide = Collide_detect(cur->x - cur->w/2, cur->y - cur->h/2, cur->w, cur->h, cur->r, cursor.x, cursor.y, 1, 1, 0);
				}else if(cur->iRejim == SQUARE){
					cur->collide = Collide_detect(cur->x - cur->w/2, cur->y - cur->h/2, cur->w, cur->h, cursor.x, cursor.y, 1, 1);
				}
			}else cur->collide = false;
			if(cur->collide) collide = true;
		}
		else
			cur->collide = false;
		cur = cur->last;
	}
	cur = NULL;
}
void use_knopki()
{
	knopka *cur = last;
	while(cur != NULL)
	{
		if(!cur->block && cur->collide)
		{
			if((cur->show && cursor.lastkey[MLEFT]) || (cur->iKey != -1 && LastKeyKeyboad[cur->iKey]))
			{
				cur->function();
				break;
			}
		}
		cur = cur->last;
	}
}
void view_knopki()
{
	knopka *cur = knopki;
	while(cur != NULL)
	{
		if(cur->show)
		{
			cur->view();
		}
		cur = cur->next;
	}
}
void update_knopki()
{
	knopka *cur = knopki;
	while(cur != NULL)
	{
		if(cur->show)
		{
			cur->update();
		}
		cur = cur->next;
	}
}
#endif
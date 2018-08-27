/*
 * This file is part of the lui_text.c
 *
 *  Copyright (c) : 2018Äê8ÔÂ19ÈÕ linghaibin
 *      Author: a6735
 */

#include "lui_text.h"
#include "lui_color.h"
#include "lui_draw.h"

static void lui_text_design (struct _lui_obj_t * obj);
static void lui_text_event(lui_touch_val_t *val);

lui_obj_t * lui_create_text(int x,int y) {
	lui_text * text = lui_malloc(sizeof(lui_button));
	text->tex = "";
	text->color = lui_color565(red);
	lui_obj_t * obj = lui_create_obj(x,y,100,16,text,lui_text_design);
	lui_obj_set_event(obj,lui_text_event);
	return obj;
}

void lui_text_set_text(lui_obj_t * obj, char * tex) {
	lui_text * text = obj->val;
	text->tex = tex;
}

static void lui_text_design (struct _lui_obj_t * obj) {
	lui_text * text = obj->val;
	//lui_draw_text(point->x,point->y,text->color,1,text->tex);
	int ax = obj->layout.point.x;
	int ay = obj->layout.point.y;
	char * tex = text->tex;
	while(*tex) {
		if(ax > (obj->layout.point.x+obj->layout.size.width)) {
			ax = obj->layout.point.x;
			ay += 16;
			if(ay > (obj->layout.point.x+obj->layout.size.length)) {
				break;
			}
		}
		lui_draw_char(ax,ay,*tex++,text->color,2,2);
		ax += 14;
	}
}

static void lui_text_event(lui_touch_val_t *val) {

}


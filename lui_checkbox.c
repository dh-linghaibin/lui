/*
 * This file is part of the lui_checkbox.c
 *
 *  Copyright (c) : 2018Äê8ÔÂ20ÈÕ linghaibin
 *      Author: a6735
 */

#include "lui_checkbox.h"
#include "lui_color.h"
#include "lui_draw.h"

static void lui_checkbox_design (struct _lui_obj_t * obj);
static void lui_checkbox_event(lui_touch_val_t *val);

lui_obj_t * lui_create_checkbox(int x,int y) {
	lui_checkbox * cheb = lui_malloc(sizeof(lui_button));
	cheb->flag = 0;
	cheb->color = lui_color565(white);
	cheb->on_click = NULL;
	lui_obj_t * obj = lui_create_obj(x,y,15,15,cheb,lui_checkbox_design);
	lui_obj_set_event(obj,lui_checkbox_event);
	return obj;
}

void lui_checkbox_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj)) {
	lui_checkbox * cheb = obj->val;
	cheb->on_click = on_click;
}

static void lui_checkbox_design (struct _lui_obj_t * obj) {
	lui_checkbox * cheb = obj->val;
	lui_draw_empty_frame(obj->layout.point.x,
			obj->layout.point.y,
			obj->layout.point.x+obj->layout.size.width,
			obj->layout.point.y+obj->layout.size.length,
					   cheb->color);
	if(cheb->flag) {
		lui_draw_frame(obj->layout.point.x+2,
				obj->layout.point.y+2,
					   obj->layout.size.width-3,
					   obj->layout.size.length-3,
					   0,cheb->color);
	}
}

static void lui_checkbox_event(lui_touch_val_t *val) {
	lui_checkbox * cheb = val->obj->val;
	if(val->falg == 2) {

	}
	if(val->falg == 0) {
		if(cheb->flag) {
			cheb->flag = 0;
		} else {
			cheb->flag = 1;
		}
		if(cheb->on_click != NULL) {
			cheb->on_click(val->obj);
		}
	}
}



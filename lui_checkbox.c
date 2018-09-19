/*
 * This file is part of the lui_checkbox.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_checkbox.h"
#include "lui_draw.h"

static void lui_checkbox_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_checkbox_event(lui_touch_val_t *val);

lui_obj_t * lui_create_checkbox(int x,int y) {
    lui_checkbox_t * cheb = lui_malloc(sizeof(lui_checkbox_t));
    cheb->flag = 0;
    cheb->color.color.rgb565 = lui_color_888_to_565(lui_color_white);
    cheb->color.alpha = LUI_COLOR_ALPHA_NULL;
    cheb->on_click = NULL;
    lui_obj_t * obj = lui_create_obj(x,y,15,15,cheb,lui_checkbox_design);
    lui_obj_set_event(obj,lui_checkbox_event);
    return obj;
}

void lui_checkbox_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj)) {
    lui_checkbox_t * cheb = obj->val;
    cheb->on_click = on_click;
}

static void lui_checkbox_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_checkbox_t * cheb = obj->val;
    lui_draw_empty_frame(point->x,
                       point->y,
                       point->x+obj->layout.size.width,
                       point->y+obj->layout.size.length,
                       cheb->color.color.rgb565);
    if(cheb->flag) {
        lui_draw_frame(point->x+2,
                       point->y+2,
                       obj->layout.size.width-3,
                       obj->layout.size.length-3,
                       cheb->color);
    }
}

static void lui_checkbox_event(lui_touch_val_t *val) {
    lui_checkbox_t * cheb = val->obj->val;
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



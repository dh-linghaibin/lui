/*
 * This file is part of the lui_bar.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_bar.h"
#include "lui_color.h"

static void lui_bar_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_bar_event(lui_touch_val_t *val);

lui_obj_t * lui_create_bar(int x, int y) {
    lui_bar * bar = lui_malloc(sizeof(lui_bar));
    bar->val = 1;
    bar->b_color = lui_color565(white);
    bar->t_color = lui_color565(white);
    bar->on_event = NULL;
    lui_obj_t * obj = lui_create_obj(x,y,150,10,bar,lui_bar_design);
    lui_obj_set_event(obj,lui_bar_event);
    return obj;
}

void lui_bar_set_val(lui_obj_t * obj, uint8_t val) {
    lui_bar * bar = obj->val;
    bar->val = val;
}

void lui_bar_set_event(lui_obj_t * obj, void (*on_event)(lui_obj_t * obj)) {
    lui_bar * bar = obj->val;
    bar->on_event = on_event;
}

static void lui_bar_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_bar * bar = obj->val;
//    lui_draw_frame(point->x,
//                   point->y,
//                   obj->layout.size.width,
//                   obj->layout.size.length,
//                   100,bar->b_color);
//    lui_draw_frame(point->x+(bar->val*(obj->layout.size.width*0.01)),
//                   point->y,
//                   15,
//                   obj->layout.size.length,
//                   100,bar->t_color);
    lui_draw_frame(point->x,
                   point->y,
                   obj->layout.size.width,
                   obj->layout.size.length,
                   150,bar->b_color);
    lui_draw_frame(point->x,
                   point->y,
                   bar->val*(obj->layout.size.width*0.01),
                   obj->layout.size.length,
                   0,bar->t_color);
}

static void lui_bar_event(lui_touch_val_t *val) {
    lui_bar * bar = val->obj->val;
    switch(val->falg) {
        case 0: {

        } break;
        case 1: {
            if(val->abs_x > val->obj->layout.size.width) {
                val->abs_x = val->obj->layout.size.width;
            }
            if(val->abs_x < 0) {
                val->abs_x = 0;
            }
            bar->val = val->abs_x/(val->obj->layout.size.width*0.01);
            if(bar->on_event != NULL) {
                bar->on_event(val->obj);
            }
        } break;
        case 2: {
            if(val->abs_x > val->obj->layout.size.width) {
                val->abs_x = val->obj->layout.size.width;
            }
            if(val->abs_x < 0) {
                val->abs_x = 0;
            }
            bar->val = val->abs_x/(val->obj->layout.size.width*0.01);
            if(bar->on_event != NULL) {
                bar->on_event(val->obj);
            }
        } break;
    }
}


/*
 * This file is part of the lui_view_group.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_view_group.h"
#include "lui_color.h"
#include "lui_draw.h"

static void lui_view_group_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_view_group_event(lui_touch_val_t *val);

lui_obj_t * lui_view_group_create(int x,int y) {
    lui_view_group * view_group = lui_malloc(sizeof(lui_view_group));
    view_group->back_color = lui_color565(white);
    lui_obj_t * obj = lui_create_obj(x,y,0,0,view_group,lui_view_group_design);
    lui_obj_set_event(obj,lui_view_group_event);
    return obj;
}

void lui_view_group_set_size(lui_obj_t * obj, int width, int length) {
    lui_obj_set_width(obj,width);
    lui_obj_set_length(obj,length);
}

static void lui_view_group_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_view_group * list = obj->val;
    lui_draw_frame(point->x,
                    point->y,
                    obj->layout.size.width,
                    obj->layout.size.length,
                    0,list->back_color);
}
int i = 0;
static void lui_view_group_event(lui_touch_val_t *val) {
    lui_view_group * list = val->obj->val;
    if(val->falg == 2) {
        i = val->abs_y;
    }
    if(val->falg == 1) {
       val->obj->layout.point.x = val->abs_x;
       val->obj->layout.point.y = val->abs_y;
    }
}



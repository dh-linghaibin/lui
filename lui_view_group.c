/*
* This file is part of the
*
* Copyright (c) 2016-2018 linghaibin
*
*/

#include "lui_view_group.h"
#include "lui_color.h"
#include "lui_draw.h"

static void lui_view_group_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_view_group_event(lui_touch_val_t *val);

lui_obj_t * lui_view_group_create(int x,int y) {
    lui_view_group_t * view_group = lui_malloc(sizeof(lui_view_group_t));
    view_group->back_color.color.rgb565 = lui_color_888_to_565(lui_color_white);
    view_group->back_color.alpha = LUI_COLOR_ALPHA_NULL;
    view_group->horizontal_move = lui_true;
    view_group->vertical_move   = lui_true;
    lui_obj_t * obj             = lui_create_obj(x,y,0,0,view_group,lui_view_group_design);
    lui_obj_set_event(obj,lui_view_group_event);
    return obj;
}

void lui_view_group_horizontal_move(lui_obj_t * obj, lui_bool val) {
    lui_view_group_t * view_group = obj->val;
    view_group->horizontal_move = val;
}

void lui_view_group_vertical_move(lui_obj_t * obj, lui_bool val) {
    lui_view_group_t * view_group = obj->val;
    view_group->vertical_move = val;
}

void lui_view_group_set_size(lui_obj_t * obj, int width, int length) {
    lui_obj_set_width(obj,width);
    lui_obj_set_length(obj,length);
}

static void lui_view_group_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_view_group_t * view_group = obj->val;
    lui_draw_frame(point->x,
                    point->y,
                    obj->layout.size.width,
                    obj->layout.size.length,
                    view_group->back_color);
}
int c_x = 0;
int c_y = 0;
static void lui_view_group_event(lui_touch_val_t *val) {
    lui_view_group_t * view_group = val->obj->val;
    if(val->falg == 2) {
        c_x = val->abs_x;
        c_y = val->abs_y;
    }
    if(val->falg == 1) {
        if(view_group->horizontal_move == lui_true) {
            val->obj->layout.point.x = val->rel_x-c_x;
        }
        if(view_group->vertical_move == lui_true) {
            val->obj->layout.point.y = val->rel_y-c_y;
        }
    }
}



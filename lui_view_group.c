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

lui_obj_t * lui_create_list(int x,int y) {
    lui_view_group * list = lui_malloc(sizeof(lui_view_group));
    list->move_val = 0;
    list->abs_val = 0;
    list->back_color = lui_color565(white);
    lui_obj_t * obj = lui_create_obj(x,y,100,200,list,lui_view_group_design);
    lui_obj_set_event(obj,lui_view_group_event);
    return obj;
}

void lui_view_group_set_backcolor(lui_obj_t * obj, uint16_t color) {
    if(obj != NULL) {
        lui_view_group * list = obj->val;
        list->back_color = color;
    }
}

void lui_view_group_set_size(lui_obj_t * list, int width, int length) {
    lui_obj_set_width(list,width);
    lui_obj_set_length(list,length);
}

void lui_view_group_add_item(lui_obj_t * list, lui_obj_t * item) {
    lui_obj_t * child;
    child = list->child;
    uint32_t i = 0;
    while(child != NULL) {
        i+=child->layout.size.length + 5;
        child = child->brother;
    }
    item->layout.point.x = 0;
    item->layout.point.y = i;
    lui_obj_add_child(list,item);
}

void lui_view_group_set_move(lui_obj_t * obj, int val) {
    lui_view_group * list = obj->val;
    list->move_val = val;
}

void lui_view_group_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj)) {
    lui_view_group * but = obj->val;
    but->on_click = on_click;
}

static void lui_view_group_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_view_group * list = obj->val;
    lui_draw_frame(point->x,
                    point->y,
                    obj->layout.size.width,
                    obj->layout.size.length,
                    0,list->back_color);
}

static void lui_view_group_event(lui_touch_val_t *val) {
    lui_view_group * list = val->obj->val;
    if(val->falg == 2) {
        list->move_val = val->abs_y;
    }
    if(val->falg == 1) {
        lui_obj_t * child;
        child = val->obj->child;
        uint32_t i = 0;
        while(child != NULL) {
            child->layout.point.y = (val->abs_y - list->move_val)+list->abs_val+i;
            i+= child->layout.size.length + 5;
            child = child->brother;
        }
    }
    if(val->falg == 0) {
        list->abs_val += val->abs_y - list->move_val;
        lui_obj_t * child;
        child = val->obj->child;
        uint32_t i = 0;
        while(child != NULL) {
            child->layout.point.y = list->abs_val+i;
            i+= child->layout.size.length + 5;
            child = child->brother;
        }
        if(list->on_click != NULL) {
            list->on_click(val->obj);
        }
    }
}



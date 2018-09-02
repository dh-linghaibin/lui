/*
 * This file is part of the lui_icon.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_icon.h"

static void lui_icon_design (struct _lui_obj_t * obj, lui_point_t *point);

lui_obj_t *lui_create_icon(int x, int y, int width, int length, const unsigned int * material) {
    licon * icon = lui_malloc(sizeof(licon));
    icon->material = material;
    lui_obj_t * obj = lui_create_obj(x,y,width,length,icon,lui_icon_design);
    return obj;
}


static void lui_icon_design (struct _lui_obj_t * obj, lui_point_t *point) {
    licon * icon = obj->val;
    for(int i = 0; i < LCD_LENGTH; i += obj->layout.size.length) {
        for(int j = 0; j < LCD_WIDTH; j += obj->layout.size.width) {
            lui_draw_icon(point->x+j,point->y+i,obj->layout.size.width,obj->layout.size.length,icon->material);
        }
    }
//    lui_draw_icon(point->x,point->y,obj->layout.size.width,obj->layout.size.length,icon->material);
}

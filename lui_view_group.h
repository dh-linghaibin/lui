/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_VIEW_GROUP_H_
#define _LUI_VIEW_GROUP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"
#include "lui_color.h"

typedef struct _lui_view_group_t {
    lui_bool horizontal_move;
    lui_bool vertical_move;
    lui_color5658_t back_color;
    void (*on_click)(lui_obj_t * obj);
} lui_view_group_t;

lui_obj_t * lui_view_group_create(int x,int y);
void lui_view_group_set_size(lui_obj_t * obj, int width, int length);
void lui_view_group_back_color_set(lui_obj_t * obj, lui_color5658_t color);

#ifdef __cplusplus
}
#endif

#endif

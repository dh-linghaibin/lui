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

typedef struct _lui_list {
    uint16_t back_color;
    void (*on_click)(lui_obj_t * obj);
} lui_view_group;

lui_obj_t * lui_view_group_create(int x,int y);
void lui_view_group_set_size(lui_obj_t * obj, int width, int length);

#ifdef __cplusplus
}
#endif

#endif

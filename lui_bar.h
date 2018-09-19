/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_BAR_H_
#define _LUI_BAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"
#include "lui_color.h"

typedef struct _lui_bar_t {
    uint8_t val;
    lui_color5658_t b_color;
    lui_color5658_t t_color;
    void (*on_event)(lui_obj_t * obj);
} lui_bar_t;

lui_obj_t * lui_create_bar(int x, int y);
void lui_bar_set_event(lui_obj_t * obj, void (*on_event)(lui_obj_t * obj));

#ifdef __cplusplus
}
#endif

#endif

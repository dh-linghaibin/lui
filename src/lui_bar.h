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

typedef struct _lui_bar {
    uint8_t val;
    uint16_t b_color;
    uint16_t t_color;
    void (*on_event)(lui_obj_t * obj);
} lui_bar;

lui_obj_t * lui_create_bar(int x, int y);
void lui_bar_set_event(lui_obj_t * obj, void (*on_event)(lui_obj_t * obj));

#ifdef __cplusplus
}
#endif

#endif

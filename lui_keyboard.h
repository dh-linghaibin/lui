/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_KEYBOARD_H_
#define _LUI_KEYBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"

typedef struct _lui_keyboard_t {
    int x;
    int y;
    lui_obj_t * tips;
    void (*on_click)(lui_obj_t * obj, char val);
} lui_keyboard_t;

lui_obj_t * lui_create_keyboard(int x,int y);

#ifdef __cplusplus
}
#endif

#endif



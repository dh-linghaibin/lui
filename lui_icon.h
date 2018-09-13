/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_ICON_H_
#define _LUI_ICON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"
#include "lui_tick.h"

typedef enum {
    LIT_JPG = 1,
    LIT_PNG = 2,
    LIT_GIF = 3,
} lui_icon_type;

typedef struct _lui_icon {
    uint8_t       mesh : 4;
    lui_icon_type type : 4;
    uint8_t frames;
    uint8_t frames_now;
    lui_tick_t * tic;
    char * path;
} lui_icon;

lui_obj_t *lui_create_icon(int x, int y);
void lui_icon_set_path(lui_obj_t * obj, char * path);

#ifdef __cplusplus
}
#endif

#endif



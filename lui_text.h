/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_TEXT_H_
#define _LUI_TEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"
#include "lui_font.h"

typedef struct _lui_text {
    char * tex;
    uint16_t color;
    lui_font_type type;
} lui_text;

lui_obj_t * lui_create_text(int x,int y);
void lui_text_set_text(lui_obj_t * obj, char * tex);
void lui_text_set_color(lui_obj_t * obj, uint16_t color);

#ifdef __cplusplus
}
#endif

#endif

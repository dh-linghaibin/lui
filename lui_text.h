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

typedef enum {
    LTP_INTERNAL,
    LTP_EXTERNAL,
} lui_text_path;

typedef enum {
    LTT_STR,
    LTP_CHINA,
} lui_text_type;

typedef struct _lui_text {
    char * tex;
    uint16_t color;
    lui_text_type type : 4;
    lui_text_path path : 4;
    char * path_name;
} lui_text;

lui_obj_t * lui_create_text(int x,int y);
void lui_text_set_font(lui_obj_t * obj, lui_text_path path, char * path_name);
void lui_text_set_text(lui_obj_t * obj, char * tex);
void lui_text_set_color(lui_obj_t * obj, uint16_t color);
char * lui_text_get_text(lui_obj_t * obj);
int lui_text_utf8_to_unicode(uint16_t * unicode, char *utf8);

#ifdef __cplusplus
}
#endif

#endif

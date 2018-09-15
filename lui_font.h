/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_FONT_H_
#define _LUI_FONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

typedef enum {
    LFT_Y_CONSOLA_10,
    LFT_Y_CONSOLA_30,
} lui_font_type;

typedef enum {
    LFP_INTERNAL,
    LFP_EXTERNAL,
} lui_font_path;

typedef struct _lui_font {
    uint8_t       wight;
    uint8_t       length;
    uint8_t       type : 4;
    lui_font_path path : 4;
    uint8_t *     font;
} lui_font;

lui_font lui_font_get(lui_font_type type, char *utf8);
int lui_font_utf8_to_unicode(uint16_t * unicode, char *utf8);

#ifdef __cplusplus
}
#endif

#endif



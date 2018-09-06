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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

typedef enum {
    LFT_Y_CONSOLA_10,
} lui_font_type;

typedef struct _lui_font {
    uint8_t wight;
    uint8_t length;
    const uint8_t * font;
} lui_font;

lui_font lui_font_get(lui_font_type type);
int lui_font_utf8_to_unicode(uint32_t * unicode, char *utf8);

#ifdef __cplusplus
}
#endif

#endif



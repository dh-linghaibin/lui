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

typedef enum {
    LIT_BMP,
    LIT_PNG,
} lui_icon_type;

typedef struct _licon {
    lui_icon_type type;
    const unsigned int * material;
} licon;

lui_obj_t *lui_create_icon(int x, int y, int width, int length, const unsigned int * material);

#ifdef __cplusplus
}
#endif

#endif



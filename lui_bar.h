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

#include "lui.h"

typedef struct _lui_bar {
    uint8_t val;
    uint16_t b_color;
    uint16_t t_color;
} lui_bar;

//lui_obj_t * lui_create_bar(int x, int y);

#ifdef __cplusplus
}
#endif

#endif

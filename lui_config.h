/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_CONFIG_H_
#define _LUI_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sram.h"

#define LCD_LENGTH 240
#define LCD_WIDTH  320

#define CACHE_LENGTH 5
#define CACHE_WIDTH  5
#define CACHE_SIZE   CACHE_LENGTH * CACHE_WIDTH

#define BEST_CACHE_LENGTH_BLACK LCD_LENGTH/CACHE_LENGTH
#define BEST_CACHE_WIDTH_BLACK  LCD_WIDTH/CACHE_WIDTH

#define lui_malloc sram_mallc
#define lui_free   sram_free

#ifdef __cplusplus
}
#endif

#endif

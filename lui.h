/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_H_
#define _LUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "sram.h"

#include "lui_obj.h"
#include "lui_draw.h"

#include "lui_color.h"
#include "lui_button.h"
#include "lui_icon.h"
#include "lui_bar.h"


void lui_init(void);
void lui_loop(void);

#ifdef __cplusplus
}
#endif

#endif

/*
* This file is part of the
*
* Copyright (c) 2016-2018 linghaibin
*
*/
#ifndef _LUI_TICK_H_
#define _LUI_TICK_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "lui_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "lui_obj.h"

typedef struct _lui_tick_t {
    lui_obj_t * obj;
    void (*event)(lui_obj_t * obj);
    struct _lui_tick_t * nxet;
} lui_tick_t;

void lui_tick_inc(uint32_t tick_period);
lui_tick_t * lui_tick_create(lui_obj_t * obj ,void (*event)(lui_obj_t * obj));
void lui_tick_delet(lui_tick_t * tick);

#ifdef __cplusplus
}
#endif
#endif
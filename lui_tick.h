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
    uint32_t over_time;
    uint32_t repeat;
    lui_obj_t * obj;
    void (*event)(struct _lui_tick_t * tick);
    struct _lui_tick_t * nxet;
} lui_tick_t;

void lui_tick_inc(uint32_t tick_period);
lui_tick_t * lui_tick_create(lui_obj_t * obj, void (*event)(lui_tick_t * tick), uint32_t time);
void lui_tick_delet(lui_tick_t * tick);
void lui_tick_loop(void);
void lui_tick_time_set(lui_tick_t * tick, uint32_t time);

#ifdef __cplusplus
}
#endif
#endif
/*
* This file is part of the
*
* Copyright (c) 2016-2018 linghaibin
*
*/
#include "lui_tick.h"
#include "sram.h"

static uint32_t sys_time    = 0;
static lui_tick_t tick_head = {
    0,
    0,
    NULL,
    NULL,
    NULL,
};

void lui_tick_inc(uint32_t tick_period) {
    sys_time += tick_period;
}

uint32_t lui_tick_get(void) {
    return sys_time;
}

uint32_t lui_tick_elaps(uint32_t prev_tick) {
    uint32_t act_time = lui_tick_get();
    /*If there is no overflow in sys_time simple subtract*/
    if(act_time >= prev_tick) {
        prev_tick = act_time - prev_tick;
    } else {
        prev_tick  = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }
    return prev_tick;
}

lui_tick_t * lui_tick_create(lui_obj_t * obj, void (*event)(lui_tick_t * tick), uint32_t time) {
    if(obj == NULL) return NULL;
    lui_tick_t * node;
    node = &tick_head;
    while(node->nxet != NULL) {
        node = node->nxet;
    }
    lui_tick_t * new_node = lui_malloc(sizeof(lui_tick_t));
    if(new_node == NULL) {
        return NULL;
    }
    node->nxet      = new_node;
    
    new_node->nxet  = NULL;
    new_node->obj   = obj;
    new_node->event = event;
    new_node->over_time = time;
    new_node->repeat    = lui_tick_get();
    return new_node;
}

void lui_tick_time_set(lui_tick_t * tick, uint32_t time) {
    if(tick == NULL) return;
    tick->over_time = time;
}

void lui_tick_delet(lui_tick_t * tick) {
    if(tick == NULL) return;
    lui_tick_t * node;
    node = &tick_head;
    while(node != NULL) {
        if(node->nxet == tick) {
            node->nxet = node->nxet->nxet;
            lui_free(tick);
            return;
        }
        node = node->nxet;
    }
}

void lui_tick_loop(void) {
    lui_tick_t * node; /* 遍历时间链表 */
    node = tick_head.nxet;
    while(node != NULL) {
        if(node->event != NULL) {
            if(lui_tick_elaps(node->repeat) >= node->over_time) {
                node->repeat = lui_tick_get();
                node->event(node);
            }
        }
        node = node->nxet;
    }
}




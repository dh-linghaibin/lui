/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_OBJ_H_
#define _LUI_OBJ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

typedef struct _lui_point_t {
    int x;
    int y;
} lui_point_t;

typedef struct _lui_size_t {
    uint16_t length;
    uint16_t width;
} lui_size_t;

typedef struct _lui_layout_t {
    lui_point_t point;
    lui_size_t  size;
} lui_layout_t;

typedef struct _lui_touch_val_t {
    struct _lui_obj_t *obj;
    int abs_x;
    int abs_y;
    int rel_x;
    int rel_y;
    uint8_t falg;
    void (*event) (struct _lui_touch_val_t *val);
} lui_touch_val_t;

typedef struct _lui_obj_t {
    lui_layout_t      layout;
    struct _lui_obj_t *father;
    struct _lui_obj_t *brother;
    struct _lui_obj_t *child;
    void * val;
    void (*design) (struct _lui_obj_t * obj, lui_point_t *point);
    void (*event) (lui_touch_val_t *val);
    uint8_t event_flag;
} lui_obj_t;


lui_obj_t *lui_get_root(void);
lui_obj_t *lui_create_obj( int x, int y, int width, int length, void * val,
        void (*design) (struct _lui_obj_t * obj, lui_point_t *point) );

void lui_obj_set_x(lui_obj_t * obj, int x);
void lui_obj_set_y(lui_obj_t * obj, int y);
void lui_obj_set_width(lui_obj_t * obj, int width);
void lui_obj_set_length(lui_obj_t * obj, int length);

void lui_obj_set_event(lui_obj_t * obj, void (*event) (lui_touch_val_t *val));

void lui_obj_add_brother(lui_obj_t * obj, lui_obj_t * brother);
void lui_obj_add_child(lui_obj_t * obj, lui_obj_t * child);
void lui_obj_distroy(lui_obj_t ** obj);

void lui_obj_traverse(lui_obj_t * obj);
void lui_obj_coupoint(lui_obj_t * obj, lui_point_t * point);

void lui_obj_even(int x, int y, uint8_t flag);

#ifdef __cplusplus
}
#endif

#endif

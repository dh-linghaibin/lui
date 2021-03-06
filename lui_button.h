/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_BUTTON_H_
#define _LUI_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"
#include "lui_color.h"

typedef struct _lui_button_t {
    lui_color5658_t color;
    void (*on_click)(lui_obj_t * obj);
    void (*on_click_down)(lui_obj_t * obj);
} lui_button_t;

lui_obj_t * lui_create_button(int x,int y);
void lui_button_set_text(lui_obj_t * obj, char * tex);
void lui_button_set_size(lui_obj_t * obj, int width,int length);
void lui_button_set_color(lui_obj_t * obj, lui_color5658_t color);
void lui_button_text_set_color(lui_obj_t * obj, uint16_t color);
void lui_button_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj));
void lui_button_setonclicklistener_down(lui_obj_t * obj, void (*on_click_down)(lui_obj_t * obj));

#ifdef __cplusplus
}
#endif

#endif

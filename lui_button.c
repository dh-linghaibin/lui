/*
 * This file is part of the lui_button.c
 *
 *  Copyright (c) : 2018  linghaibin
 *      Author: a6735
 */

#include "lui_button.h"
#include "lui_color.h"
#include "lui_draw.h"
#include "lui_text.h"

static void lui_button_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_button_event(lui_touch_val_t *val);

lui_obj_t * lui_create_button(int x,int y) {
    lui_button * but = lui_malloc(sizeof(lui_button));
    but->color = lui_color565(white);
    but->alpha = 0;
    but->bold = 1;
    but->on_click = NULL;
    but->on_click_down = NULL;
    lui_obj_t * obj = lui_create_obj(x,y,50,25,but,lui_button_design);
    lui_obj_set_event(obj,lui_button_event);

    lui_obj_t * text = lui_create_text(0,0);
    lui_obj_add_child(obj,text);
    return obj;
}

void lui_button_set_text(lui_obj_t * obj, char * tex) {
    lui_button * but = obj->val;
    lui_text_set_text(obj->child,tex);
    int tex_size = 0;
    lui_text * _text = obj->child->val;
    uint16_t adr = 0;
    uint16_t wight = 0;
    uint16_t length = 0;
    if(_text->path == LTP_EXTERNAL) {
        FILE *file;
        file = fopen(_text->path_name, "rb");
        unsigned char size[2]= {0,0};
        fread(size, 1, 2, file);
        uint16_t wight = 7;//size[0];
        uint16_t length = 9;//size[1];
        fclose(file);
    } else {
        wight  = (uint16_t)( ( _text->path_name[2]<<8) | _text->path_name[1] );
        length = (uint16_t)( ( _text->path_name[4]<<8) | _text->path_name[3] );
    }
    while(*tex) {
        uint8_t type = lui_text_utf8_to_unicode(&adr,tex);
        if(type == 0) {
            tex++;
        } else {
            tex+=3;
        }
        tex_size++;
    }
    int _x = (obj->layout.size.width/2) - ((wight*tex_size)/2) - 1;
    int _y = (obj->layout.size.length/2) - (length/2);
    lui_obj_set_x(obj->child,_x);
    lui_obj_set_y(obj->child,_y);
}

void lui_button_set_color(lui_obj_t * obj, uint16_t color) {
    lui_button * but = obj->val;
    but->color = color;
}

void lui_button_text_set_color(lui_obj_t * obj, uint16_t color) {
    lui_button * but = obj->val;
    lui_text_set_color(obj->child,color);
}

void lui_button_set_size(lui_obj_t * obj, int width,int length) {
    lui_obj_set_width(obj,width);
    lui_obj_set_length(obj,length);
}

void lui_button_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj)) {
    lui_button * but = obj->val;
    but->on_click = on_click;
}

void lui_button_setonclicklistener_down(lui_obj_t * obj, void (*on_click_down)(lui_obj_t * obj)) {
    lui_button * but = obj->val;
    but->on_click_down = on_click_down;
}

static void lui_button_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_button * but = obj->val;
    lui_draw_frame(point->x,
                    point->y,
                    obj->layout.size.width,
                    obj->layout.size.length,
                    but->alpha,but->color);
    // int x = point->x + (obj->layout.size.width/2) - (but->tex_size*(but->bold*4.5)/2);
    // int y = point->y + (obj->layout.size.length/2) - but->bold*3.5;
    // lui_draw_text(x,y,but->tex_color, LFT_Y_CONSOLA_10,but->tex);
}

static void lui_button_event(lui_touch_val_t *val) {
    lui_button * but = val->obj->val;
    if(val->falg == 2) {
        but->alpha = 200;
        if(but->on_click_down != NULL) {
            but->on_click_down(val->obj);
        }
    }
    if(val->falg == 0) {
        but->alpha = 0;
        if(but->on_click != NULL) {
            but->on_click(val->obj);
        }
    }
}


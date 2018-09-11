/*
 * This file is part of the lui_text.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_text.h"
#include "lui_color.h"
#include "lui_draw.h"
#include "lui_font.h"

static void lui_text_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_text_event(lui_touch_val_t *val);

lui_obj_t * lui_create_text(int x,int y) {
    lui_text * text = lui_malloc(sizeof(lui_button));
    text->tex = "";
    text->type = LFT_Y_CONSOLA_10;
    text->color = lui_color565(0x00);
    lui_obj_t * obj = lui_create_obj(x,y,150,20,text,lui_text_design);
    lui_obj_set_event(obj,NULL);
    return obj;
}

void lui_text_set_text(lui_obj_t * obj, char * tex) {
    lui_text * text = obj->val;
    text->tex = tex;
}

void lui_text_set_color(lui_obj_t * obj, uint16_t color) {
    lui_text * text = obj->val;
    text->color = color;
}

char * lui_text_get_text(lui_obj_t * obj) {
    lui_text * text = obj->val;
    return text->tex;
}

static void lui_text_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_text * text = obj->val;
    int ax = point->x;
    int ay = point->y;
    char * tex = text->tex;
    lui_font font;
    while(*tex) {
        
        font = lui_font_get(text->type,tex);

        if(ax > (point->x+obj->layout.size.width)) {
            ax = point->x;
            ay += font.length;
            if(ay > (point->y+obj->layout.size.length)) {
                break;
            }
        }
        lui_draw_font(ax, ay,font.wight, font.length,text->color,font.font);
        lui_free(font.font);
        if(font.type == 0) {
            tex++;
        } else {
            tex+=3;
        }
        ax += font.wight;
    }
}

static void lui_text_event(lui_touch_val_t *val) {

}

/*
 * This file is part of the lui_keyboard.c
 *
 *  Copyright (c) : 2018��8��22�� linghaibin
 *      Author: a6735
 */

#include "lui_keyboard.h"
#include "lui_color.h"
#include "lui_draw.h"

typedef struct _key_code {
    char *code;
} key_code;
static const key_code mkey_code[30] = { {"Q"},{"W"},{"E"},{"R"},{"T"},{"Y"},{"U"},{"I"},{"O"},{"P"},
                                        {"A"},{"S"},{"D"},{"F"},{"G"},{"H"},{"J"},{"K"},{"L"},{" "},
                                        {"Z"},{"X"},{"C"},{"V"},{"B"},{"N"},{"M"},{"."},{" "},{" "},
                                      };

static void lui_keyboard_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_keyboard_event(lui_touch_val_t *val);
static void but_on_click(lui_obj_t * obj);
static void but_on_click_down(lui_obj_t * obj);

lui_obj_t * lui_create_keyboard(int x,int y) {
    lui_keyboard * kb = lui_malloc(sizeof(lui_keyboard));
    kb->on_click = NULL;
    lui_obj_t * obj = lui_create_obj(x,y,258,83,kb,lui_keyboard_design);
    lui_obj_set_event(obj,lui_keyboard_event);
    for(int i = 0; i < 3;i++) {
        for(int j = 0; j < 10;j++) {
            lui_obj_t * but_key = lui_create_button(5+j*25,5+i*25);
            lui_button_set_size(but_key,22,22);
            lui_button_setonclicklistener(but_key,but_on_click);
            lui_button_setonclicklistener_down(but_key,but_on_click_down);
            lui_button_set_text(but_key,mkey_code[i*10+j].code);
            lui_obj_add_child(obj,but_key);
        }
    }
    kb->tips = NULL;
    return obj;
}

void lui_keyboard_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj, char val)) {
    lui_keyboard * but = obj->val;
    but->on_click = on_click;
}

static void lui_keyboard_design (struct _lui_obj_t * obj, lui_point_t *point) {
//    lui_keyboard * list = obj->val;
//    if(list->y != 0) {
//        lui_draw_frame(list->x,
//                       list->y,
//                       40,
//                       40,
//                       0,0x34ff);
//    }
}

static void lui_keyboard_event(lui_touch_val_t *val) {

}

static void but_on_click_down(lui_obj_t * obj) {
    lui_button * but = obj->val;
    lui_keyboard * kb = obj->father->val;

    lui_point_t point;
    point.x = obj->layout.point.x;
    point.y = obj->layout.point.y;
    lui_obj_coupoint(obj,&point);

    if(kb->tips == NULL) {
        kb->tips = lui_create_button(-10,-10);
        lui_obj_add_child(lui_get_root(),kb->tips);
        lui_button_set_size(kb->tips,30,30);
        lui_button_set_color(kb->tips,lui_color565(blue));
    }
    lui_button_set_text(kb->tips,but->tex);
    kb->tips->layout.point.y = point.y-40;
    kb->tips->layout.point.x = point.x-4;
}

static void but_on_click(lui_obj_t * obj) {
    lui_button * but = obj->val;
    lui_keyboard * kb = obj->father->val;

    kb->tips->layout.point.y = -40;
    kb->tips->layout.point.x = -40;
    if(kb->on_click != NULL) {
        kb->on_click(obj->father,*but->tex);
    }
}




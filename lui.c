
#include "lui.h"
#include "lui_checkbox.h"
#include "lui_keyboard.h"
#include "lui_text.h"
#include "lui_icon.h"
#include "lui_view_group.h"
#include "lui_tick.h"

lui_obj_t *xshow;

uint8_t c_r, c_g, c_b;

void bar_r_event(lui_obj_t * obj) {
    lui_bar * bar = obj->val;
    c_r = bar->val*2.55;
    lui_text_set_color(xshow,lui_color_rgb( c_r,c_g,c_b));
}

void bar_g_event(lui_obj_t * obj) {
    lui_bar * bar = obj->val;
    c_g = bar->val*2.55;
    lui_text_set_color(xshow,lui_color_rgb( c_r,c_g,c_b));
}

void bar_b_event(lui_obj_t * obj) {
    lui_bar * bar = obj->val;
    c_b = bar->val*2.55;
    lui_text_set_color(xshow,lui_color_rgb( c_r,c_g,c_b));
}

int i = 0;
void tic_event(lui_tick_t * tick) {
    printf("time over %d\n",i);
    i++;
    if(i == 5) {
        lui_tick_delet(tick);
    }
}

void lui_init(void) {
    lui_obj_t * icon = lui_create_icon(0,0);
    lui_icon_set_path(icon,LIP_EXTERNAL,"tool/desk.bin");
    lui_obj_add_child(lui_get_root(),icon);

    // lui_obj_t * icon2 = lui_create_icon(0,0);
    // lui_icon_set_path(icon2,LIP_EXTERNAL,"tool/test.bin");
    // lui_obj_add_child(lui_get_root(),icon2);

    // lui_obj_t * icon3 = lui_create_icon(40,10);
    // lui_icon_set_path(icon3,LIP_EXTERNAL,"tool/button4.png.bin");
    // lui_obj_add_child(lui_get_root(),icon3);

    // lui_obj_t * icon4 = lui_create_icon(60,10);
    // lui_icon_set_path(icon4,LIP_EXTERNAL,"tool/button4_blue.png.bin");
    // lui_obj_add_child(lui_get_root(),icon4);

    // lui_obj_t * icon5 = lui_create_icon(80,0);
    // lui_icon_set_path(icon5,LIP_EXTERNAL,"tool/switch_open.png.bin");
    // lui_obj_add_child(lui_get_root(),icon5);
    
    // lui_obj_t * icon6 = lui_create_icon(120,0);
    // lui_icon_set_path(icon6,LIP_EXTERNAL,"tool/switch_close.png.bin");
    // lui_obj_add_child(lui_get_root(),icon6);

    // lui_obj_t * group = lui_view_group_create(50,50);
    // lui_view_group_set_size(group,100,100);
    // lui_obj_add_child(lui_get_root(),group);

    // lui_obj_t * icon_ba = lui_create_icon(40,0);
    // lui_icon_set_path(icon_ba,LIP_EXTERNAL,"tool/battery.png.bin");
    // lui_obj_add_child(group,icon_ba);
    
    // lui_obj_t * icon7 = lui_create_icon(0,50);
    // lui_icon_set_path(icon7,LIP_EXTERNAL,"tool/dog.gif.bin");
    // lui_obj_add_child(lui_get_root(),icon7);

    // lui_obj_t * icon8 = lui_create_icon(0,0);
    // lui_icon_set_path(icon8,LIP_EXTERNAL,"tool/cat.gif.bin");
    // lui_obj_add_child(group,icon8);

    // // lui_obj_t * icon9 = lui_create_icon(50,0);
    // // lui_icon_set_path(icon9,"tool/monthing.gif.bin");
    // // lui_obj_add_child(lui_get_root(),icon9);

    // lui_obj_t * keyboard = lui_create_keyboard(10,150);
    // lui_obj_add_child(lui_get_root(),keyboard);

    // lui_obj_t * but = lui_create_button(100,120);
    // lui_button_set_text(but,"Enter");
    // lui_obj_add_child(lui_get_root(),but);
    // lui_text_set_color(but->child,lui_color565(red));
}

void lui_loop(void) {
    for(int i = 0; i < LCD_LENGTH; i += CACHE_LENGTH) {
        for(int j = 0; j < LCD_WIDTH; j += CACHE_WIDTH) {
            lui_set_cache_size(j,i,CACHE_WIDTH,CACHE_LENGTH);
            lui_obj_traverse(lui_get_root());
            lui_cachedev(j,i,CACHE_WIDTH,CACHE_LENGTH);
        }
    }
}





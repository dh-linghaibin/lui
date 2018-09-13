
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
    lui_icon_set_path(icon,"tool/desk.bin");
    lui_obj_add_child(lui_get_root(),icon);

    lui_obj_t * icon2 = lui_create_icon(0,0);
    lui_icon_set_path(icon2,"tool/test.bin");
    lui_obj_add_child(lui_get_root(),icon2);

    lui_obj_t * icon3 = lui_create_icon(40,10);
    lui_icon_set_path(icon3,"tool/button4.png.bin");
    lui_obj_add_child(lui_get_root(),icon3);

    lui_obj_t * icon4 = lui_create_icon(60,10);
    lui_icon_set_path(icon4,"tool/button4_blue.png.bin");
    lui_obj_add_child(lui_get_root(),icon4);

    lui_obj_t * icon5 = lui_create_icon(80,0);
    lui_icon_set_path(icon5,"tool/switch_open.png.bin");
    lui_obj_add_child(lui_get_root(),icon5);
    
    lui_obj_t * icon6 = lui_create_icon(120,0);
    lui_icon_set_path(icon6,"tool/switch_close.png.bin");
    lui_obj_add_child(lui_get_root(),icon6);

    lui_obj_t * group = lui_view_group_create(300,50);
    lui_view_group_set_size(group,100,100);
    lui_obj_add_child(lui_get_root(),group);

    lui_obj_t * icon_ba = lui_create_icon(40,0);
    lui_icon_set_path(icon_ba,"tool/battery.png.bin");
    lui_obj_add_child(group,icon_ba);
    
    lui_obj_t * icon7 = lui_create_icon(0,0);
    lui_icon_set_path(icon7,"tool/close.png.bin");
    lui_obj_add_child(group,icon7);

    // lui_obj_t * list1 = lui_create_list(0,0);
    // lui_obj_set_width(list1,100);
    // lui_obj_set_length(list1,55);
    // lui_list_set_backcolor(list1,lui_color565(deepskyblue));
    // lui_list_add_item(list1,lui_create_bar(100,20));

    // lui_obj_t * list2 = lui_create_list(0,0);
    // lui_obj_set_width(list2,120);
    // lui_obj_set_length(list2,60);
    // lui_list_add_item(list2,list1);
    // lui_list_set_backcolor(list2,lui_color565(red));

    // lui_obj_t * list3 = lui_create_list(0,0);
    // lui_obj_set_width(list3,140);
    // lui_obj_set_length(list3,70);
    // lui_list_add_item(list3,list2);
    // lui_list_set_backcolor(list3,lui_color565(yellow));

    // lui_obj_t * list5 = lui_create_list(50,50);
    // lui_obj_set_width(list5,180);
    // lui_obj_set_length(list5,90);
    // lui_list_add_item(list5,list3);
    // lui_obj_add_child(lui_get_root(),list5);
    // lui_list_set_backcolor(list5,lui_color565(pink));

    // lui_obj_t * xl1 = lui_create_list(0,0);
    // lui_obj_set_width(xl1,20);
    // lui_obj_set_length(xl1,20);
    // lui_list_set_backcolor(xl1,lui_color565(red));

    // lui_obj_t * xl2 = lui_create_list(200,0);
    // lui_obj_set_width(xl2,40);
    // lui_obj_set_length(xl2,40);
    // lui_list_add_item(xl2,xl1);
    // lui_obj_add_child(lui_get_root(),xl2);
    // lui_list_set_backcolor(xl2,lui_color565(gainsboro));

    lui_obj_t * keyboard = lui_create_keyboard(10,150);
    lui_obj_add_child(lui_get_root(),keyboard);

    lui_obj_t * but = lui_create_button(100,120);
    lui_button_set_text(but,"Enter");
    lui_obj_add_child(lui_get_root(),but);
    lui_text_set_color(but->child,lui_color565(red));

    lui_tick_t * tic = lui_tick_create(keyboard,tic_event,1000);

    // lui_tick_delet(tic);

    // xshow = lui_create_text(0,0);
    // lui_obj_add_child(lui_get_root(),xshow);
    // lui_text_set_text(xshow,"123");
    // lui_obj_set_width(xshow,32);
    // lui_obj_set_length(xshow,32);
    // lui_text_set_color(xshow,lui_color565(deepskyblue));

    // lui_obj_t* bar_r =  lui_create_bar(10,50);
    // lui_bar_set_event(bar_r,bar_r_event);
    // lui_obj_add_child(lui_get_root(),bar_r);

    // lui_obj_t* bar_g =  lui_create_bar(10,80);
    // lui_bar_set_event(bar_g,bar_g_event);
    // lui_obj_add_child(lui_get_root(),bar_g);

    // lui_obj_t* bar_b =  lui_create_bar(10,110);
    // lui_bar_set_event(bar_b,bar_b_event);
    // lui_obj_add_child(lui_get_root(),bar_b);
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





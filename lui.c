
#include "lui.h"
#include "material/i_window.h"
#include "material/desk.h"
#include "material/img_test.h"
#include "lui_checkbox.h"
#include "lui_keyboard.h"
#include "lui_text.h"
#include "material/fruit.h"
#include "lui_icon.h"
#include "lui_list.h"

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

void lui_init(void) {
    // lui_obj_t * icon = lui_create_icon(0,0,320,240,desk);
    // lui_obj_add_child(lui_get_root(),icon);

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

    // lui_obj_t * keyboard = lui_create_keyboard(10,150);
    // lui_obj_add_child(lui_get_root(),keyboard);

    xshow = lui_create_text(0,0);
    lui_obj_add_child(lui_get_root(),xshow);
    char * name = lui_malloc(sizeof(char));
    name = "12121SDFLSAJFLK三月，醉一场青春的流年。慢步在三月的春光里，走走停停，看花开嫣然，看春雨绵绵，感受春风拂面，春天，就是青春的流年。青春，是人生中最美的风景。青春，是一场花开的遇见；青春，是一场痛并快乐着的旅行；青春，是一场轰轰烈烈的比赛；青春，是一场鲜衣奴马的争荣岁月；青春，是一场风花雪月的光阴青春往事，多么甜蜜；青春岁月，多么靓丽；青春流年，如火如荼。青春里，我们向着梦想前进。跌倒过，伤心过，快乐过，痛苦过，孤独过，彷徨过，迷茫过。总是，在悠悠的岁月中徘徊；总是，在如春的生命中成长；总是，在季节的交替中感悟人生四季的美妙与韵律；总是，在多愁善感中体悟青春的美好与无奈。";
    lui_text_set_text(xshow,name);
    lui_obj_set_width(xshow,320);
    lui_obj_set_length(xshow,240);
    lui_text_set_color(xshow,lui_color565(deepskyblue));

    lui_obj_t* bar_r =  lui_create_bar(10,50);
    lui_bar_set_event(bar_r,bar_r_event);
    lui_obj_add_child(lui_get_root(),bar_r);

    lui_obj_t* bar_g =  lui_create_bar(10,80);
    lui_bar_set_event(bar_g,bar_g_event);
    lui_obj_add_child(lui_get_root(),bar_g);

    lui_obj_t* bar_b =  lui_create_bar(10,110);
    lui_bar_set_event(bar_b,bar_b_event);
    lui_obj_add_child(lui_get_root(),bar_b);
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





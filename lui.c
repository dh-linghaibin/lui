
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

void lui_init(void) {
    lui_obj_t * icon = lui_create_icon(0,0,320,240,desk);
    lui_obj_add_child(lui_get_root(),icon);

    lui_obj_t *text = lui_create_text(10,10);
    // lui_obj_add_child(lui_get_root(),text);
    char * name = lui_malloc(sizeof(char)*17);
    name = "Container Qiantao";
    lui_text_set_text(text,name);

//    lui_obj_t * but[5];
//    for(int i = 0; i < 5;i++) {
//        but[i] = lui_create_button(20,20+i*35);
//        lui_obj_add_child(lui_get_root(),but[i]);
//        lui_button_set_text(but[i],"Enter");
//        lui_button_text_set_color(but[i],lui_color565(blue));
//    }

    // lui_list_add_item(list,lui_create_button(100,20));
    // lui_list_add_item(list,lui_create_bar(100,20));
    // lui_list_add_item(list,lui_create_button(100,20));
    // lui_list_add_item(list,lui_create_bar(100,20));
    // lui_list_add_item(list,lui_create_button(100,20));
    // lui_list_add_item(list,lui_create_bar(100,20));
    // lui_list_add_item(list,lui_create_button(100,20));
    // lui_list_add_item(list,lui_create_bar(100,20));

    lui_obj_t * list1 = lui_create_list(0,0);
    lui_obj_set_width(list1,100);
    lui_obj_set_length(list1,50);
    lui_obj_add_child(list1,text);
    lui_list_set_backcolor(list1,lui_color565(white));

    lui_obj_t * list2 = lui_create_list(0,0);
    lui_obj_set_width(list2,120);
    lui_obj_set_length(list2,60);
    lui_list_add_item(list2,list1);
    lui_list_set_backcolor(list2,lui_color565(red));

    lui_obj_t * list3 = lui_create_list(0,0);
    lui_obj_set_width(list3,140);
    lui_obj_set_length(list3,70);
    lui_list_add_item(list3,list2);
    lui_list_set_backcolor(list3,lui_color565(yellow));
    // lui_list_set_backcolor(list3,lui_color565(green));
    // lui_list_add_item(list3,lui_create_button(100,20));
    // lui_list_add_item(list3,lui_create_bar(100,20));
    // lui_list_add_item(list3,lui_create_button(100,20));
    // lui_list_add_item(list3,lui_create_bar(100,20));

    // lui_obj_t * list4 = lui_create_list(50,50);
    // lui_obj_set_width(list4,160);
    // lui_obj_set_length(list4,80);
    // lui_list_add_item(list4,list3);
    // lui_list_set_backcolor(list4,lui_color565(yellow));

    lui_obj_t * list5 = lui_create_list(50,50);
    lui_obj_set_width(list5,180);
    lui_obj_set_length(list5,90);
    lui_list_add_item(list5,list3);
    lui_obj_add_child(lui_get_root(),list5);
    lui_list_set_backcolor(list5,lui_color565(pink));

    lui_obj_t * keyboard = lui_create_keyboard(10,150);
    lui_obj_add_child(lui_get_root(),keyboard);

    lui_obj_t * xl1 = lui_create_list(0,0);
    lui_obj_set_width(xl1,20);
    lui_obj_set_length(xl1,20);
    lui_list_set_backcolor(xl1,lui_color565(red));

    lui_obj_t * xl2 = lui_create_list(200,0);
    lui_obj_set_width(xl2,40);
    lui_obj_set_length(xl2,40);
    lui_list_add_item(xl2,xl1);
    lui_obj_add_child(lui_get_root(),xl2);
    lui_list_set_backcolor(xl2,lui_color565(gainsboro));

    // lui_obj_t * keyboard2 = lui_create_keyboard(0,0);
    // lui_obj_add_child(lui_get_root(),keyboard2 );
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





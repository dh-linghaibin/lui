
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

//    lui_obj_t * but[5];
//    for(int i = 0; i < 5;i++) {
//        but[i] = lui_ create_button(20,20+i*35);
//        lui_obj_add_child(lui_get_root(),but[i]);
//        lui_button_set_text(but[i],"B");
//    }

    lui_obj_t * list = lui_create_list(0,0);
    lui_obj_set_width(list,100);
    lui_obj_set_length(list,200);
    lui_obj_t * bar2 = lui_create_bar(100,20);
    lui_list_add_item(list,bar2);
    lui_list_add_item(list,lui_create_button(100,20));
    lui_list_add_item(list,lui_create_bar(100,20));
    lui_list_add_item(list,lui_create_button(100,20));
    lui_list_add_item(list,lui_create_bar(100,20));
    lui_list_add_item(list,lui_create_button(100,20));
    lui_list_add_item(list,lui_create_bar(100,20));
    lui_list_add_item(list,lui_create_button(100,20));
    lui_list_add_item(list,lui_create_bar(100,20));

    lui_obj_t * list2 = lui_create_list(0,0);
    lui_obj_set_length(list2,220);
    lui_obj_set_width(list2,120);
    lui_list_add_item(list2,list);

    lui_obj_t * list3 = lui_create_list(0,0);
    lui_obj_set_length(list3,240);
    lui_obj_set_width(list3,140);
    lui_list_add_item(list3,list2);
    lui_obj_add_child(lui_get_root(),list3);

//    lui_obj_t *text = lui_create_text(300,10);
//    lui_obj_add_child(lui_get_root(),text);
//    char * name = lui_malloc(sizeof(char));
//    name[0] = "T";
//    lui_text_set_text(text,name);

//    lui_obj_t * keyboard = lui_create_keyboard(300,220);
//    lui_obj_add_child(lui_get_root(),keyboard);
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





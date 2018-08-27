
#include "lui.h"
#include "material/i_window.h"
#include "material/desk.h"
#include "material/img_test.h"
#include "lui_checkbox.h"
#include "lui_keyboard.h"
#include "lui_text.h"
#include "material/fruit.h"

//lui_obj_t * bar;
//lui_obj_t * keyboard;
//lui_obj_t * text;
//
//void but_on_click(lui_obj_t * obj) {
//	lui_bar_set_val(bar,33);
//	if(keyboard->layout.point.y == 200) {
//		keyboard->layout.point.y = -100;
//	} else {
//		keyboard->layout.point.y = 200;
//	}
//}
//
//void list_on_click(lui_obj_t * obj, char val) {
//	lui_text * ltext = text->val;
//	ltext->tex[0] = val;
//}

void lui_init(void) {
//	lui_obj_t * icon = lui_create_icon(0,0,320,240,desk);
//	lui_obj_add_child(lui_get_root(),icon);
//
//	lui_obj_t * but[5];
//	for(int i = 0; i < 5;i++) {
//		but[i] = lui_create_button(20,20+i*35);
//		lui_obj_add_child(lui_get_root(),but[i]);
//		lui_button_set_text(but[i],"B");
//	}
//
//	bar = lui_create_bar(100,20);
	//lui_obj_add_child(lui_get_root(),bar);
	//lui_button_setonclicklistener(but[4],but_on_click);

//	lui_obj_t * cheb = lui_create_checkbox(100,60);
//	lui_obj_add_child(lui_get_root(),cheb);

	lui_obj_t * list = lui_create_list(99,120);
	lui_obj_add_child(lui_get_root(),list);
	//lui_list_add_item(list,bar);
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
//	lui_draw_png(0,0,24,24,emoji_pixel_map);

//	lui_obj_t * list2 = lui_create_list(299,120);
//	lui_obj_set_width(list2,300);
//	lui_list_add_item(list2,list);
//	lui_obj_add_child(lui_get_root(),list2);
//
//	text = lui_create_text(300,10);
//	lui_obj_add_child(lui_get_root(),text);
//	char * name = lui_malloc(sizeof(char));
//	name[0] = "T";
//	lui_text_set_text(text,name);

//	keyboard = lui_create_keyboard(205,200);
//	lui_obj_add_child(lui_get_root(),keyboard);
//	lui_keyboard_setonclicklistener(keyboard,list_on_click);

//	lui_obj_distroy(&list->child->brother);
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





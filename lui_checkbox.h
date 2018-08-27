/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_CHECKBOX_H_
#define _LUI_CHECKBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"


typedef struct _lui_checkbox {
	uint8_t flag;
	uint16_t color;
	void (*on_click)(lui_obj_t * obj);
} lui_checkbox;

lui_obj_t * lui_create_checkbox(int x,int y);
void lui_checkbox_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj));


#ifdef __cplusplus
}
#endif

#endif

/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_LIST_H_
#define _LUI_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_obj.h"


typedef struct _lui_list {
	int move_val;
	int abs_val;
	void (*on_click)(lui_obj_t * obj);
} lui_list;

lui_obj_t * lui_create_list(int x,int y);
void lui_list_add_item(lui_obj_t * list, lui_obj_t * item);

#ifdef __cplusplus
}
#endif

#endif
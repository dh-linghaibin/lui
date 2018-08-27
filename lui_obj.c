/*
 * This file is part of the lui_obj.c
 *
 *  Copyright (c) : 2018年8月18日 linghaibin
 *      Author: a6735
 */

#include "lui_obj.h"

static lui_obj_t lui_root = {
    .layout.point.x = 0,
    .layout.point.y = 0,
    .layout.size.width = LCD_LENGTH,
    .layout.size.length = LCD_WIDTH,
    .father  = NULL,
    .brother = NULL,
    .child   = NULL,
	.design  = NULL,
};

lui_obj_t *lui_get_root(void) {
	return &lui_root;
}

lui_obj_t *lui_create_obj( int x, int y, int width, int length, void * val,
		void (*design) (struct _lui_obj_t * obj,lui_point_t *point) ) {
	lui_obj_t * obj;
    obj = (lui_obj_t *)lui_malloc(sizeof(lui_obj_t));
    if( obj == NULL ) {
        return NULL;
    }
    obj->layout.point.x = x;
    obj->layout.point.y = y;
    obj->layout.size.width = width;
    obj->layout.size.length = length;
    obj->brother = NULL;
    obj->child   = NULL;
    obj->father  = NULL;
    obj->design  = design;
    obj->val     = val;
    obj->event   = NULL;
    obj->event_flag = 0;
    return obj;
}

void lui_obj_set_event(lui_obj_t * obj, void (*event) (lui_touch_val_t *val)) {
	obj->event = event;
}

void lui_obj_set_x(lui_obj_t * obj, int x) {
	obj->layout.point.x = x;
}

int lui_obj_get_x(lui_obj_t * obj) {
	return obj->layout.point.x;
}


void lui_obj_set_y(lui_obj_t * obj, int y) {
	obj->layout.point.y = y;
}

void lui_obj_set_width(lui_obj_t * obj, int width) {
	obj->layout.size.width = width;
}

void lui_obj_set_length(lui_obj_t * obj, int length) {
	obj->layout.size.length = length;
}

static lui_obj_t * _lui_get_last_brother(lui_obj_t * obj) {
    if (obj->brother == NULL) {
        return obj;
    } else {
        return _lui_get_last_brother(obj->brother);
    }
}

static lui_obj_t * _lui_get_last_child(lui_obj_t * obj) {
    if (obj->child == NULL) {
        return obj;
    } else {
        return _lui_get_last_brother(obj->child);
    }
}

void lui_obj_add_brother(lui_obj_t * obj, lui_obj_t * brother) {
	lui_obj_t * t;
    t = _lui_get_last_brother(obj);
    brother->father = obj->father;
    t->brother = brother;
}

void lui_obj_add_child(lui_obj_t * obj, lui_obj_t * child) {
	lui_obj_t * t;
    t = _lui_get_last_child(obj);
    child->father = obj;
    if(obj->child == NULL) {
        t->child = child;
    } else {
        t->brother = child;
    }
}


void lui_obj_find_up(lui_obj_t * root, lui_obj_t * obj) {
	if (root == NULL) {
		return;
	} else {
		if(root->child == obj) {
			root->child = obj->brother;
			return;
		} else if(root->brother == obj) {
			root->brother = obj->brother;
			return;
		}
		lui_obj_find_up(root->child,obj);
		lui_obj_find_up(root->brother,obj);
	}
}

static void _lui_obj_distroy(lui_obj_t ** obj) {
	lui_obj_t *pl, *pr;
    if((*obj) == NULL) {
        return ;
    } else {
        pl = (*obj)->child;  //保存左孩子的地址
        pr = (*obj)->brother;  //保存右孩子的地址
        (*obj)->child = NULL;
        (*obj)->brother = NULL;
        lui_free((*obj)->val);
        lui_free(*obj);   //释放根节点
        (*obj) = NULL;
        _lui_obj_distroy(&pl);   //递归销毁
        _lui_obj_distroy(&pr);
    }
}

void lui_obj_distroy(lui_obj_t ** obj) {
	if((*obj) != NULL) {
		lui_obj_find_up(&lui_root,(*obj));
		_lui_obj_distroy(&((*obj)->child));
	}
}

static void lui_obj_get_father_layout(lui_obj_t * obj, lui_layout_t * layout) {
	layout->point.x = 0;
	layout->point.y = 0;
	layout->size.width = LCD_WIDTH;
	layout->size.length = LCD_LENGTH;
	lui_obj_t * f_obj = obj->father;

	if(f_obj != NULL) {
		layout->point.x = f_obj->layout.point.x;
		layout->point.y = f_obj->layout.point.y;
		layout->size.width = f_obj->layout.size.width;
		layout->size.length = f_obj->layout.size.length;
	}
}

extern lui_layout_t f_layout;

void lui_obj_traverse(lui_obj_t * obj) {
    if (obj == NULL) {
        return;
    } else {
    	if(obj->design != NULL) {
    		lui_point_t point;
    		point.x = 0;
    		point.y = 0;
			lui_obj_coupoint(obj,&point);
			point.x += obj->layout.point.x;
			point.y += obj->layout.point.y;

			lui_obj_get_father_layout(obj,&f_layout);

			obj->design(obj,&point);
    	}
        lui_obj_traverse(obj->child);
        lui_obj_traverse(obj->brother);
    }
}

void lui_obj_coupoint(lui_obj_t * obj, lui_point_t * point) {
	if(obj->father == NULL) {
		return;
	} else {
		point->x += obj->father->layout.point.x;
		point->y += obj->father->layout.point.y;
		lui_obj_coupoint(obj->father, point);
	}
}

static lui_touch_val_t touch_val = {
	.obj = NULL,
	.abs_x = 0,
	.abs_y = 0,
	.rel_x = 0,
	.rel_y = 0,
	.falg  = 0,
	.event = NULL,
};

void _lui_obj_even(lui_obj_t * obj, int x, int y, uint8_t flag) {
    if (obj == NULL) {
        return;
    } else {
    	if(obj->design != NULL) {
    		lui_point_t point;
			point.x = obj->layout.point.x;
			point.y = obj->layout.point.y;
			lui_obj_coupoint(obj,&point);
			if( ( ( x >= point.x && x <= (point.x + obj->layout.size.width) )
					&& ( y >= point.y && y <= (point.y + obj->layout.size.length) ) )
					|| obj->event_flag == 1 ) {
					touch_val.event = obj->event;
					touch_val.abs_x = x - point.x;
					touch_val.abs_y = y - point.y;
					touch_val.rel_x = x;
					touch_val.rel_y = y;
					touch_val.falg  = flag;
					touch_val.obj   = obj;
			}
    	}
    	if(obj->event_flag == 0) {
    		_lui_obj_even(obj->child,x,y,flag);
    		_lui_obj_even(obj->brother,x,y,flag);
    	}
    }
}

void lui_obj_even(int x, int y, uint8_t flag) {
	touch_val.obj = NULL;
	touch_val.event = NULL;
	_lui_obj_even(&lui_root,x,y,flag);
	if(touch_val.event != NULL) {
		if(touch_val.falg == 2) {
			touch_val.obj->event_flag = 1;
		}
		if(touch_val.falg == 0) {
			touch_val.obj->event_flag = 0;
		}
		touch_val.event(&touch_val);
	}
}

/*
 * This file is part of the lui_obj.c
 *
 *  Copyright (c) : 2018��8��18�� linghaibin
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
        pl = (*obj)->child;  
        pr = (*obj)->brother;
        (*obj)->child = NULL;
        (*obj)->brother = NULL;
        lui_free((*obj)->val);
        lui_free(*obj);
        (*obj) = NULL;
        _lui_obj_distroy(&pl);
        _lui_obj_distroy(&pr);
    }
}

void lui_obj_distroy(lui_obj_t ** obj) {
	if((*obj) != NULL) {
		lui_obj_find_up(&lui_root,(*obj));
		_lui_obj_distroy(&((*obj)->child));
	}
}

extern lui_layout_t f_layout;
static lui_obj_t * _f_layout_obj = NULL;

void _lui_obj_father_size(lui_obj_t * obj) {
//	lui_obj_t * f_obj = obj->father;
//	while(f_obj != NULL) {
//		printf("%d, %d, %d, %d \n",f_obj->layout.point.x,f_obj->layout.point.y,
//				f_obj->layout.size.width,f_obj->layout.size.length);
//		f_obj = f_obj->father;
//	}
	f_layout.point.x = 0;
	f_layout.point.y = 0;
	f_layout.size.width = LCD_WIDTH;
	f_layout.size.length = LCD_LENGTH;
}

static void lui_obj_get_father_layout(lui_point_t * pos, lui_obj_t * obj, lui_layout_t * layout) {
	lui_obj_t * f_obj = obj->father;
	layout->point.x = 0;
	layout->point.y = 0;
	layout->size.width = LCD_WIDTH;
	layout->size.length = LCD_LENGTH;
	if(f_obj == NULL) {
		layout->point.x = 0;
		layout->point.y = 0;
		layout->size.width = LCD_WIDTH;
		layout->size.length = LCD_LENGTH;
	} else {
		if(_f_layout_obj == f_obj) {

		} else {

		}
	}
}

lui_obj_t * last_stack = NULL;
lui_point_t stack_point;

int tree_layer = 0;
int tree_layer2 = 0;

void lui_obj_traverse(lui_obj_t * obj) {
    if (obj == NULL) {
    	tree_layer --;
        return;
    } else {
    	tree_layer++;
//    	printf("%d \n",tree_layer);
    	if(tree_layer2 > tree_layer) {
    		int i = tree_layer2-tree_layer;
    		while(i > 0) {
    			i--;
    			if(last_stack != NULL) {
					stack_point.x -= last_stack->layout.point.x;
					stack_point.y -= last_stack->layout.point.y;

					last_stack = last_stack->father;
				}
    		}
    	}
    	tree_layer2 = tree_layer;
    	if(obj->child != NULL) {
    		if(obj->father == NULL) {
    			printf("start---");
    			tree_layer = 0;
    			stack_point.x = 0;
    			stack_point.y = 0;
    			f_layout.point.x = 0;
    			f_layout.point.y = 0;
    			f_layout.size.width = LCD_WIDTH;
    			f_layout.size.length = LCD_LENGTH;
    		} else {
    			last_stack = obj;

				int w1 = stack_point.x + f_layout.size.width;
				int h1 = stack_point.y + f_layout.size.length;

    			stack_point.x += last_stack->layout.point.x;
				stack_point.y += last_stack->layout.point.y;

				int w2 = stack_point.x + last_stack->layout.size.width;
				int h2 = stack_point.y + last_stack->layout.size.length;

				if(h2 < h1) {
					f_layout.size.length = last_stack->layout.size.length;
					if(f_layout.point.y > stack_point.y) {
						f_layout.size.length -= f_layout.point.y - stack_point.y;
						// printf("%d %d---",f_layout.point.y , stack_point.y);
					}
				} else if(h2 > h1) {
					if((h2 - h1) > f_layout.size.length) {
						f_layout.size.length = 0;
						tree_layer --;
						return ;
					} else {
						f_layout.size.length -= (h2 - h1);
					}
				}

				if(f_layout.point.x < stack_point.x) {
					f_layout.point.x = stack_point.x;
				}

				if(f_layout.point.y < stack_point.y) {
					f_layout.point.y = stack_point.y;
				}

				printf("^ %d %d---%d^", h1,h2,f_layout.point.x+f_layout.size.length);
    		}
			printf("father-\n");
		}
//		printf("-- %d, %d, %d, %d \n",obj->layout.point.x,obj->layout.point.y,
//				obj->layout.size.width,obj->layout.size.length);

		if(obj->design != NULL) {
			lui_point_t point;
			if(last_stack == obj) {
				point.x = stack_point.x;
				point.y = stack_point.y;
			} else {
				point.x = stack_point.x + obj->layout.point.x;
				point.y = stack_point.y + obj->layout.point.y;
			}
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

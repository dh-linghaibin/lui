/*
 * This file is part of the lui_icon.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_icon.h"
#include "lui_draw.h"

static void lui_icon_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_icon_event(lui_touch_val_t *val);

lui_obj_t *lui_create_icon(int x, int y) {
    lui_icon * icon = lui_malloc(sizeof(lui_icon));
    icon->path = NULL;
    icon->mesh = 0;
    lui_obj_t * obj = lui_create_obj(x,y,0,0,icon,lui_icon_design);
    lui_obj_set_event(obj,lui_icon_event);
    return obj;
}

void lui_icon_set_path(lui_obj_t * obj, char * path) {
    lui_icon * icon = obj->val;
    if(path != NULL) {
        icon->path = path;
        FILE *file;
        file = fopen(icon->path, "rb");
        if (!file) {
            icon->path = NULL;
            return;
        }
        unsigned char size[5]= {0,0,};
        fread(size, 1, 5, file);
        icon->type = (lui_icon_type)size[0];
        lui_obj_set_width(obj,(uint16_t)( size[2]<<8)+size[1]);
        lui_obj_set_length(obj,(uint16_t)( size[4]<<8)+size[3]);
        fclose(file);
    }
}

static void lui_icon_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_icon * icon = obj->val;
    if(icon->path != NULL) {
        FILE *file;
        file = fopen(icon->path, "rb");
        if (!file) {
            icon->path = NULL;
            return;
        }
        unsigned long fileLen;
        fseek(file, 5, SEEK_SET);
        fileLen = obj->layout.size.width*obj->layout.size.length;
        if(icon->type == LIT_JPG) {
            fileLen *= 2;
        } else {
            fileLen *= 3;
        }
        uint8_t * material = (uint8_t *)malloc(fileLen);
        fread(material, 1, fileLen, file);
        fclose(file);
        if(icon->type == LIT_JPG) {
            lui_draw_jpg(point->x,point->y,obj->layout.size.width,obj->layout.size.length,material);
        } else {
            lui_draw_png(point->x,point->y,obj->layout.size.width,obj->layout.size.length,material);
        }
        if(icon->mesh == 1) {
            lui_draw_frame(point->x,point->y,obj->layout.size.width,obj->layout.size.length,200,0x34ff);
        }
        free(material);
    }
}

static void lui_icon_event(lui_touch_val_t *val) {
    lui_icon * icon = val->obj->val;
    if(val->falg == 2) {
        icon->mesh = 1;
    }
    if(val->falg == 0) {
        icon->mesh = 0;
    }
}

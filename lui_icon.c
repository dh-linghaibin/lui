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
static void icon_tic_event(lui_tick_t * tick);

lui_obj_t *lui_create_icon(int x, int y) {
    lui_icon * icon = lui_malloc(sizeof(lui_icon));
    icon->path_name = NULL;
    icon->mesh = 0;
    icon->frames = 0;
    icon->frames_now = 0;
    icon->path = LIP_EXTERNAL;
    icon->tic = NULL;
    lui_obj_t * obj = lui_create_obj(x,y,0,0,icon,lui_icon_design);
    lui_obj_set_event(obj,lui_icon_event);
    return obj;
}

static void icon_tic_event(lui_tick_t * tick) {
    lui_icon * icon = tick->obj->val;
    if(icon->frames_now+1 < icon->frames) {
        icon->frames_now++;
    } else {
        icon->frames_now = 0;
    }
}

void lui_icon_set_path(lui_obj_t * obj, lui_icon_path path, char * path_name) {
    if(obj == NULL) return;
    if(path_name == NULL) return;
    lui_icon * icon = obj->val;
    icon->path = path;
    icon->path_name = path_name;
    if(path == LIP_EXTERNAL) {
        FILE *file;
        file = fopen(icon->path_name, "rb");
        if (!file) {
            icon->path_name = NULL;
            return;
        }
        unsigned char size[6]= {0,0,};
        fread(size, 1, 6, file);
        icon->type = (lui_icon_type)size[0];
        lui_obj_set_width(obj,(uint16_t)( size[2]<<8)+size[1]);
        lui_obj_set_length(obj,(uint16_t)( size[4]<<8)+size[3]);
        if(icon->type == LIT_GIF) {
            icon->frames = size[5];
            icon->tic = lui_tick_create(obj,icon_tic_event,100);
        }
        fclose(file);
    } else {
        icon->type = path_name[0];
        lui_obj_set_width(obj,(uint16_t)( path_name[2]<<8)+path_name[1]);
        lui_obj_set_length(obj,(uint16_t)( path_name[4]<<8)+path_name[3]);
        if(icon->type == LIT_GIF) {
            icon->frames = path_name[5];
            icon->tic = lui_tick_create(obj,icon_tic_event,100);
        }
    }
}

static void lui_icon_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_icon * icon = obj->val;
    if(icon->path_name != NULL) {
        uint8_t * material = NULL;
        if(icon->path ==LIP_EXTERNAL) {
            FILE *file;
            file = fopen(icon->path_name, "rb");
            if (!file) {
                icon->path_name = NULL;
                return;
            }
            unsigned long fileLen;
            fileLen = obj->layout.size.width*obj->layout.size.length;
            if(icon->type == LIT_JPG) {
                fseek(file, 5, SEEK_SET);
                fileLen *= 2;
            } else if(icon->type == LIT_PNG) {
                fseek(file, 5, SEEK_SET);
                fileLen *= 3;
            } else if(icon->type == LIT_GIF) {
                fileLen *= 3;
                fseek(file, 6+fileLen*icon->frames_now, SEEK_SET);
            }
            material = (uint8_t *)malloc(fileLen);
            fread(material, 1, fileLen, file);
            fclose(file);
        } else {
            if(icon->type == LIT_JPG) {
                material = (uint8_t *) icon->path_name + 3;
            } else if(icon->type == LIT_PNG) {
                material = (uint8_t *) icon->path_name + 3;
            } else if(icon->type == LIT_GIF) {
                material = (uint8_t *) icon->path_name + 3 + \
                (obj->layout.size.width*obj->layout.size.length*icon->frames_now);
            }
        }
        if(icon->type == LIT_JPG) {
            lui_draw_jpg(point->x,point->y,obj->layout.size.width,obj->layout.size.length,material);
        } else {
            lui_draw_png(point->x,point->y,obj->layout.size.width,obj->layout.size.length,material);
        }
        if(icon->mesh == 1) {
            lui_draw_frame(point->x,point->y,obj->layout.size.width,obj->layout.size.length,200,0x34ff);
        }
        if(icon->path ==LIP_EXTERNAL) {
            free(material);
        }
        // cg_antialiased_line(0,0,80,100,0x00);
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

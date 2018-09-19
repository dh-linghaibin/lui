/*
 * This file is part of the lui_text.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */
#include "lui_text.h"
#include "lui_color.h"
#include "lui_draw.h"
#include "tool/consola_font_10.h"

// static lui_text default_font = {
    
// };

static void lui_text_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_text_event(lui_touch_val_t *val);

lui_obj_t * lui_create_text(int x,int y) {
    lui_text_t * text = lui_malloc(sizeof(lui_text_t));
    text->tex = "";
    text->color = 0x00;
    text->path = LTP_INTERNAL;
    text->path_name = (char *)consola_font_10;
    lui_obj_t * obj = lui_create_obj(x,y,50,50,text,lui_text_design);
    lui_obj_set_event(obj,NULL);
    return obj;
}

void lui_text_set_font(lui_obj_t * obj, lui_text_path path, char * path_name) {
    lui_text_t * text = obj->val;
    text->path = path;
    text->path_name = path_name;
}

void lui_text_set_text(lui_obj_t * obj, char * tex) {
    lui_text_t * text = obj->val;
    text->tex = tex;
}

void lui_text_set_color(lui_obj_t * obj, uint16_t color) {
    lui_text_t * text = obj->val;
    text->color = color;
}

char * lui_text_get_text(lui_obj_t * obj) {
    lui_text_t * text = obj->val;
    return text->tex;
}

static void lui_text_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_text_t * text = obj->val;
    int ax = point->x;
    int ay = point->y;
    char * tex = text->tex;
    while(*tex) {
        uint16_t adr = 0;
        char * font = NULL;
        uint16_t wight = 0;
        uint16_t length = 0;
        uint8_t type = lui_text_utf8_to_unicode(&adr, tex);
        if(text->path == LTP_EXTERNAL) {
            FILE *file = NULL;
            unsigned long fileLen;
            file = fopen(text->path_name, "rb");
            if(file == NULL) {
                return;
            }
            unsigned char r_size[5]= {0,0};
            fread(r_size, 1, 5, file);
            wight = (uint16_t)( ( r_size[2]<<8) | r_size[1] );
            length = (uint16_t)( ( r_size[4]<<8) | r_size[3] );
            fileLen= wight * length;
            if(type == 1) {
                adr += 95;
            }
            fseek(file, 5+fileLen*adr, SEEK_SET);
            font=(uint8_t *)lui_malloc(fileLen);
            if (font == NULL) {
                fclose(file);
                return;
            }
            fread(font, 1, fileLen, file);
            fclose(file);
        } else {
            wight  = (uint16_t)( ( text->path_name[2]<<8) | text->path_name[1] );
            length = (uint16_t)( ( text->path_name[4]<<8) | text->path_name[3] );
            font   = (uint8_t *) ( text->path_name + 5 + (wight*length*adr) );
        }
        if(ax > (point->x+obj->layout.size.width)) {
            ax = point->x;
            ay += length;
            if(ay > (point->y+obj->layout.size.length)) {
                break;
            }
        }
        lui_draw_font(ax, ay,wight, length,text->color, font);
        if(text->path == LTP_EXTERNAL) {
            lui_free(font);
        }
        if(type == 0) {
            tex++;
        } else {
            tex+=3;
        }
        ax += wight;
    }
}

static void lui_text_event(lui_touch_val_t *val) {

}

int lui_text_utf8_to_unicode(uint16_t * unicode, char *utf8) {
    char *pchar = utf8;
    int nBytes = 0;
    if (0 == (*utf8 & 0x80)) {
        /*
         * single-byte char
         */
        nBytes = 0;
        *unicode = *utf8 - ' ';
    } else {
        /*
         * 3-byte char (chinese char)
         */
        if ( (*utf8 & 0xf0) == 0xe0 ) {
            nBytes  = 3;
            unsigned char code[2];
            code[0] = ((utf8[0] & 0x0f) <<4) + ((utf8[1] & 0x3c) >>2);
            code[1] = ((utf8[1] & 0x03) <<6) + (utf8[2] & 0x3f);
            *unicode =  ((code[0] << 8) | code[1]) - 19968; //- 19968;
            return 1;
        } else {
            /*
             * ? not chaina
             */
            nBytes = 0;
            return 0;
        }
    }
    return 0;
}



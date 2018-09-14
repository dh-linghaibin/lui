/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "lui_font.h"
#include "lui_font_mate.h"

// TODO:
// ! 忘记了啊
// * 是这样的
// ? 颜色
lui_font lui_font_get(lui_font_type type, char *utf8) {
    switch(type) {
        case LFT_Y_CONSOLA_10 : {
            lui_font font_consola_10;
            uint16_t adr = 0;
            font_consola_10.path = LFP_EXTERNAL;
            font_consola_10.type = lui_font_utf8_to_unicode(&adr, utf8);
            {
                FILE *file;
                unsigned long fileLen;
                if(font_consola_10.type == 0) {
                    file = fopen("file/img_.bin", "rb");
                } else {
                    file = fopen("file/img_.bin", "rb");
                }
                if (!file) {
                    exit(1);
                }
                unsigned char size[2]= {0,0};
                fread(size, 1, 2, file);
                font_consola_10.wight = 7;//size[0];
                font_consola_10.length = 9;//size[1];
                fileLen=font_consola_10.wight*font_consola_10.length;
                fseek(file, fileLen*adr, SEEK_SET);
                font_consola_10.font=(uint8_t *)lui_malloc(fileLen+1);
                if (!font_consola_10.font) {
                    fclose(file);
                    exit(1);
                }
                fread(font_consola_10.font, 1, fileLen+1, file);
                fclose(file);
            }
            return font_consola_10;
        } break;
        case LFT_Y_CONSOLA_30 : {
            lui_font font_consola_30;
            uint16_t adr           = 0;
            font_consola_30.path   = LFP_INTERNAL;
            font_consola_30.type   = lui_font_utf8_to_unicode(&adr, utf8);
            font_consola_30.wight  = china_tft_32[1];
            font_consola_30.length = china_tft_32[2];
            font_consola_30.font   = (uint8_t *) 3+(font_consola_30.wight*font_consola_30.length*adr);
            return font_consola_30;
        } break;
    }
}

int lui_font_utf8_to_unicode(uint16_t * unicode, char *utf8) {
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



/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "lui_font.h"
#include "lui_font_mate.h"

//TODO: 凌海滨
//FIXME: 测试
lui_font lui_font_get(lui_font_type type) {
    switch(type) {
        case LFT_Y_CONSOLA_10 : {
            // lui_font font_consola_10;
            // font_consola_10.wight = 7;
            // font_consola_10.length = 9;
            // font_consola_10.font = lui_mate_font_consola_10;

            lui_font font_consola_10;
            font_consola_10.wight = 11;
            font_consola_10.length = 11;
            font_consola_10.font = lui_mate_font_consola_10;
            return font_consola_10;
        } break;
    }
}

int lui_font_utf8_to_unicode(uint32_t * unicode, char *utf8)
{
    char *pchar = utf8;
    int nBytes = 0;
     
    if (0 == (*utf8 & 0x80)) 
    {
        /*
         * single-byte char
         */
        nBytes = 0;
        *unicode = *utf8 - ' ';
    }
    else
    {
        /*
         * 3-byte char (chinese char)
         */
        int i;
 
        if ( (*utf8 & 0xf0) == 0xe0 ) 
        {
            nBytes  = 3;
            char code[2];
            code[0] = ((utf8[0] & 0x0f) <<4) + ((utf8[1] & 0x3c) >>2);
            code[1] = ((utf8[1] & 0x03) <<6) + (utf8[2] & 0x3f);
            *unicode =  ( (code[0] << 8) | code[1] ) - 19968;
            return 1;
        }
        else
        {
            nBytes = 0;
            return 0;
        }
    }
    return 0;
}

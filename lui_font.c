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
            lui_font font_consola_10;
            font_consola_10.wight = 7;
            font_consola_10.length = 9;
            font_consola_10.font = lui_mate_font_consola_10;
            return font_consola_10;
        } break;
    }
}

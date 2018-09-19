/*
 * This file is part of the lui_color.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_color.h"

uint16_t lui_color_888_to_565(uint32_t rgb) {
    register uint8_t  r, g, b;
    b = ( rgb >> (0+3) ) & 0x1f;
    g = ( rgb >> (8+2) ) & 0x3f;
    r = ( rgb >> (16+3)) & 0x1f;
    return( (r<<11) + (g<<5) + (b<<0) );
}

uint16_t lui_color_rgb_to_565(uint8_t r, uint8_t g, uint8_t b) {
    return (uint16_t)( (( r >> 3 ) << 11 ) | (( g >> 2 ) << 5  ) | ( b  >> 3 ));
}

uint16_t lui_color_alpha_blend(uint16_t fr_c,uint16_t bk_c,uint8_t alpha) {
    register lui_color565_u color, color_fr_c,color_bk_c ;
    color_fr_c.rgb565 = fr_c;
    color_bk_c.rgb565 = bk_c;
    color.rgb.r = ( color_fr_c.rgb.r * alpha + color_bk_c.rgb.r * (0xff-alpha) ) >> 8;
    color.rgb.g = ( color_fr_c.rgb.g * alpha + color_bk_c.rgb.g * (0xff-alpha) ) >> 8;
    color.rgb.b = ( color_fr_c.rgb.b * alpha + color_bk_c.rgb.b * (0xff-alpha) ) >> 8;
    return color.rgb565;
}

// uint16_t lui_alpha_blend565(uint16_t src,uint16_t dst,uint8_t alpha) {
//     register  uint32_t src2;
//     register uint32_t dst2;
//     //Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
//     src2=((src<<16)|src)&0x07E0F81F;
//     dst2=((dst<<16)|dst)&0x07E0F81F;
//     //Perform blending R:G:B with alpha in range 0..32
//     //Note that the reason that alpha may not exceed 32 is that there are only
//     //5bits of space between each R:G:B value, any higher value will overflow
//     //into the next component and deliver ugly result.
//     dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
//     return (dst2>>16)|dst2;
// }

// uint16_t lui_blend565(uint16_t src,uint16_t dst,uint8_t alpha) {
//     uint32_t src2;
//     uint32_t dst2;
//     src2=((src<<16)|src)&0x07E0F81F;
//     dst2=((dst<<16)|dst)&0x07E0F81F;
//     dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
//     return ((dst2>>16)|dst2);
// }



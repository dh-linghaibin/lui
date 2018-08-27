/*
 * This file is part of the lui_color.c
 *
 *  Copyright (c) : 2018年8月18日 linghaibin
 *      Author: a6735
 */

#include "lui_color.h"

uint16_t lui_color565(uint32_t rgb) {
	uint8_t  r, g, b;
	b = ( rgb >> (0+3) ) & 0x1f;// 取B色的高5位
	g = ( rgb >> (8+2) ) & 0x3f;// 取G色的高6位
	r = ( rgb >> (16+3)) & 0x1f;// 取R色的高5位
	return( (r<<11) + (g<<5) + (b<<0) );
}

uint16_t lui_alpha_blend(uint16_t fr_c,uint16_t bk_c,uint8_t alpha) {
    color565 color, color_fr_c,color_bk_c ;
    color_fr_c.rgb565 = fr_c;
    color_bk_c.rgb565 = bk_c;
    color.rgb.r = ( color_fr_c.rgb.r * alpha + color_bk_c.rgb.r * (0xff-alpha) ) >> 8;
    color.rgb.g = ( color_fr_c.rgb.g * alpha + color_bk_c.rgb.g * (0xff-alpha) ) >> 8;
    color.rgb.b = ( color_fr_c.rgb.b * alpha + color_bk_c.rgb.b * (0xff-alpha) ) >> 8;
    return color.rgb565;
}

//快速ALPHA BLENDING算法.
//src:源颜色
//dst:目标颜色
//alpha:透明程度(0~32)
//返回值:混合后的颜色.
uint16_t lui_alpha_blend565(uint16_t src,uint16_t dst,uint8_t alpha) {
	uint32_t src2;
	uint32_t dst2;
	//Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
	src2=((src<<16)|src)&0x07E0F81F;
	dst2=((dst<<16)|dst)&0x07E0F81F;
	//Perform blending R:G:B with alpha in range 0..32
	//Note that the reason that alpha may not exceed 32 is that there are only
	//5bits of space between each R:G:B value, any higher value will overflow
	//into the next component and deliver ugly result.
	dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
	return (dst2>>16)|dst2;
}

uint16_t lui_blend565(uint16_t src,uint16_t dst,uint8_t alpha) {
	uint32_t src2;
	uint32_t dst2;
    src2=((src<<16)|src)&0x07E0F81F;
    dst2=((dst<<16)|dst)&0x07E0F81F;
    dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
    return ((dst2>>16)|dst2);
}



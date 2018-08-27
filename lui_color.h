/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_COLOR_H_
#define _LUI_COLOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui.h"

#define white     0xffffff
#define ivory     0xfffff0
#define lightyellow     0xffffe0
#define yellow     0xffff00
#define snow     0xfffafa
#define floralwhite     0xfffaf0
#define lemonchiffon     0xfffacd
#define cornsilk     0xfff8dc
#define seaShell     0xfff5ee
#define lavenderblush     0xfff0f5
#define papayawhip     0xffefd5
#define blanchedalmond     0xffebcd
#define mistyrose     0xffe4e1
#define bisque     0xffe4c4
#define moccasin     0xffe4b5
#define navajowhite     0xffdead
#define peachpuff     0xffdab9
#define gold     0xffd700
#define pink     0xffc0cb
#define lightpink     0xffb6c1
#define orange     0xffa500
#define lightsalmon     0xffa07a
#define darkorange     0xff8c00
#define coral     0xff7f50
#define hotpink     0xff69b4
#define tomato     0xff6347
#define orangered     0xff4500
#define deeppink     0xff1493
#define fuchsia     0xff00ff
#define magenta     0xff00ff
#define red     0xff0000
#define oldlace     0xfdf5e6
#define lightgoldenrodyellow     0xfafad2
#define linen     0xfaf0e6
#define antiquewhite     0xfaebd7
#define salmon     0xfa8072
#define ghostwhite     0xf8f8ff
#define mintcream     0xf5fffa
#define whitesmoke     0xf5f5f5
#define beige     0xf5f5dc
#define wheat     0xf5deb3
#define sandybrown     0xf4a460
#define azure     0xf0ffff
#define honeydew     0xf0fff0
#define aliceblue     0xf0f8ff
#define khaki     0xf0e68c
#define lightcoral     0xf08080
#define palegoldenrod     0xeee8aa
#define violet     0xee82ee
#define darksalmon     0xe9967a
#define lavender     0xe6e6fa
#define lightcyan     0xe0ffff
#define burlywood     0xdeb887
#define plum     0xdda0dd
#define gainsboro     0xdcdcdc
#define crimson     0xdc143c
#define palevioletred     0xdb7093
#define goldenrod     0xdaa520
#define orchid     0xda70d6
#define thistle     0xd8bfd8
#define lightgray     0xd3d3d3
#define lightgrey     0xd3d3d3
#define tan     0xd2b48c
#define chocolate     0xd2691e
#define peru     0xcd853f
#define indianred     0xcd5c5c
#define mediumvioletred     0xc71585
#define silver     0xc0c0c0
#define darkkhaki     0xbdb76b
#define rosybrown     0xbc8f8f
#define mediumorchid     0xba55d3
#define darkgoldenrod     0xb8860b
#define firebrick     0xb22222
#define powderblue     0xb0e0e6
#define lightsteelblue     0xb0c4de
#define paleturquoise     0xafeeee
#define greenyellow     0xadff2f
#define lightblue     0xadd8e6
#define darkgray     0xa9a9a9
#define darkgrey     0xa9a9a9
#define brown     0xa52a2a
#define sienna     0xa0522d
#define darkorchid     0x9932cc
#define palegreen     0x98fb98
#define darkviolet     0x9400d3
#define mediumpurple     0x9370db
#define lightgreen     0x90ee90
#define darkseagreen     0x8fbc8f
#define saddlebrown     0x8b4513
#define darkmagenta     0x8b008b
#define darkred     0x8b0000
#define blueviolet     0x8a2be2
#define lightskyblue     0x87cefa
#define skyblue     0x87ceeb
#define grey     0x808080
#define olive     0x808000
#define purple     0x800080
#define maroon     0x800000
#define aquamarine     0x7fffd4
#define chartreuse     0x7fff00
#define lawngreen     0x7cfc00
#define mediumslateblue     0x7b68ee
#define lightslategray     0x778899
#define lightslategrey     0x778899
#define slategray     0x708090
#define slategrey     0x708090
#define olivedrab     0x6b8e23
#define slateblue     0x6a5acd
#define dimgrey     0x696969
#define mediumaquamarine     0x66cdaa
#define cornflowerblue     0x6495ed
#define cadetblue     0x5f9ea0
#define darkolivegreen     0x556b2f
#define indigo     0x4b0082
#define mediumturquoise     0x48d1cc
#define darkslateblue     0x483d8b
#define steelblue     0x4682b4
#define royalblue     0x4169e1
#define turquoise     0x40e0d0
#define mediumseagreen     0x3cb371
#define limegreen     0x32cd32
#define darkslategray     0x2f4f4f
#define darkslategrey     0x2f4f4f
#define seagreen     0x2e8b57
#define forestgreen     0x228b22
#define lightseagreen     0x20b2aa
#define dodgerblue     0x1e90ff
#define midnightblue     0x191970
#define aqua     0x00ffff
#define cyan     0x00ffff
#define springgreen     0x00ff7f
#define lime     0x00ff00
#define mediumspringgreen     0x00fa9a
#define darkturquoise     0x00ced1
#define deepskyblue     0x00bfff
#define darkcyan     0x008b8b
#define teal     0x008080
#define green     0x008000
#define darkgreen     0x006400
#define blue     0x0000ff
#define mediumblue     0x0000cd
#define darkblue     0x00008b
#define navy     0x000080

typedef union {
    struct {
        uint16_t r : 5;
        uint16_t g : 6;
        uint16_t b : 5;
    } rgb;
    uint16_t rgb565;
} color565;

uint16_t lui_color565(uint32_t rgb);
uint16_t lui_alpha_blend(uint16_t fr_c,uint16_t bk_c,uint8_t alpha);
uint16_t lui_alpha_blend565(uint16_t src,uint16_t dst,uint8_t alpha);

#ifdef __cplusplus
}
#endif

#endif

/*
 * This file is part of the
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _LUI_DRAW_H_
#define _LUI_DRAW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui.h"

void lui_draw_frame(int x, int y, int width, int length, int alpha, uint16_t color);
void lui_draw_line(int s_x, int s_y, int e_x, int e_y, uint16_t color);
void lui_draw_empty_frame( int x1, int y1, int x2, int y2, uint16_t color );
void lui_draw_arc( int x0, int y0, int r, int s, uint16_t color );
void lui_draw_circle( int x0, int y0, int r, uint16_t c );
void lui_draw_fill_circle( int x0, int y0, int r, uint16_t c );
void lui_draw_round_frame( int x1, int y1, int x2, int y2, int r, uint16_t c );
void lui_draw_fill_round_frame( int x1, int y1, int x2, int y2, int r, int c );
void lui_draw_mesh( int x1, int y1, int x2, int y2, uint16_t c );
void lui_draw_text(int s_x, int s_y, uint16_t color, uint8_t bold, char * tex);
void lui_draw_font_gbk16(uint16_t x, uint16_t y, uint16_t fc, char *s);
void lui_draw_icon(int x, int y, int width, int length, const unsigned int * material);
void lui_draw_png(int x, int y, int width, int length, const uint8_t * material);

void lui_set_cache_size(int x, int y, int width, int length);
lui_layout_t * lui_get_cache_size(void);
void lui_cachedev(int x, int y, int width, int length);

#ifdef __cplusplus
}
#endif

#endif

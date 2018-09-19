/*
 * This file is part of the
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _LUI_DRAW_H_
#define _LUI_DRAW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lui_config.h"
#include "lui_obj.h"
#include "lui_color.h"

void lui_draw_set_updata( void (*up)(int x, int y, int width, int length, uint16_t * color) );
void lui_draw_frame(int x, int y, int width, int length, lui_color5658_t color);
void lui_draw_line(int s_x, int s_y, int e_x, int e_y, uint16_t color);
void lui_draw_empty_frame( int x1, int y1, int x2, int y2, uint16_t color );
void lui_draw_arc( int x0, int y0, int r, int s, uint16_t color );
void lui_draw_circle( int x0, int y0, int r, uint16_t c );
void lui_draw_fill_circle( int x0, int y0, int r, uint16_t c );
void lui_draw_round_frame( int x1, int y1, int x2, int y2, int r, uint16_t c );
void lui_draw_fill_round_frame( int x1, int y1, int x2, int y2, int r, lui_color5658_t color );
void lui_draw_mesh( int x1, int y1, int x2, int y2, lui_color5658_t color );
void lui_draw_png(int x, int y, int width, int length, uint8_t * material);
void lui_draw_jpg(int x, int y, int width, int length, uint8_t * material);
void lui_draw_font(int x, int y, uint8_t wighth, uint8_t length, uint16_t color, uint8_t * mate);
void lui_draw_screen(int x, int y, int width, int length, uint32_t * material);


static uint8_t lui_draw_check_layout(int x, int y, int width, int length);
void lui_drawcache_size_set(int x, int y, int width, int length);
lui_layout_t * lui_draw_cache_size(void);
void lui_draw_cache_to_lcd(int x, int y, int width, int length);

#ifdef __cplusplus
}
#endif

#endif

/*
 * This file is part of the lui_draw.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_draw.h"
#include "lui_font_mate.h"

typedef struct _lcache {
    uint16_t array[CACHE_SIZE];
    lui_layout_t coordinate;
} lui_cache;

static lui_cache cache; 

void lui_draw_char(uint16_t x, int16_t y, unsigned char c,
                    uint16_t color, uint8_t sx, uint8_t sy);

static void l_point(int s_x, int s_y ,int m_x, int m_y, uint16_t color) {
    if( ( s_x  >= cache.coordinate.point.x && s_x  < m_x ) &&
    (  s_y >= cache.coordinate.point.y && s_y < m_y ) ) {
        int xx = (s_x-cache.coordinate.point.x);
        int yy = (s_y-cache.coordinate.point.y);
        cache.array[yy*cache.coordinate.size.width+xx] = color;
    }
}

lui_layout_t f_layout;

static uint8_t lui_draw_check_layout(int x, int y, int width, int length) {
    int maxX1 = x + width;
    int maxY1 = y + length;
    int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

    int l_x = f_layout.point.x + f_layout.size.width;
    int l_y = f_layout.point.y + f_layout.size.length;

    if (!(maxX1 < f_layout.point.x || x > l_x
                || maxY1 < f_layout.point.y || y > l_y )) {

            if (!(maxX1 < cache.coordinate.point.x || x > maxX2
                    || maxY1 < cache.coordinate.point.y || y > maxY2)) {
                int x1 = 0; int y1 = 0;
                int x2 = cache.coordinate.size.width;
                int y2 = cache.coordinate.size.length;

                if(f_layout.point.x > x) {
                    x = f_layout.point.x;
                }

                if(f_layout.point.y > y) {
                    y = f_layout.point.y;
                }

                if(maxX1 > l_x) {
                    maxX1 = l_x;
                }

                if(maxY1 > l_y) {
                    maxY1 = l_y;
                }
                return 1;
            }
    }
    return 0;
}

void lui_draw_frame(int x, int y, int width, int length, int alpha, uint16_t color) {
    int maxX1 = x + width;
    int maxY1 = y + length;
    int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

    int l_x = f_layout.point.x + f_layout.size.width;
    int l_y = f_layout.point.y + f_layout.size.length;

    if (!(maxX1 < f_layout.point.x || x > l_x
            || maxY1 < f_layout.point.y || y > l_y )) {

        if (!(maxX1 < cache.coordinate.point.x || x > maxX2
                || maxY1 < cache.coordinate.point.y || y > maxY2)) {
            int x1 = 0; int y1 = 0;
            int x2 = cache.coordinate.size.width;
            int y2 = cache.coordinate.size.length;

            if(f_layout.point.x > x) {
                x = f_layout.point.x;
            }

            if(f_layout.point.y > y) {
                y = f_layout.point.y;
            }

            if(maxX1 > l_x) {
                maxX1 = l_x;
            }

            if(maxY1 > l_y) {
                maxY1 = l_y;
            }

            if(x >= cache.coordinate.point.x) {
                x1 = x-cache.coordinate.point.x;
            }
            if(maxX1 < maxX2) {
                x2 = cache.coordinate.size.width-(maxX2-maxX1);
            }
            if(y >= cache.coordinate.point.y) {
                y1 = y-cache.coordinate.point.y;
            }
            if(maxY1 < maxY2) {
                y2 = cache.coordinate.size.length-(maxY2-maxY1);
            }
            for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
                for(int x_j = x1; x_j < x2; x_j++) {
                    if(alpha == 0) {
                        cache.array[y_i+x_j] = color;
                    } else {
                        cache.array[y_i+x_j] = lui_alpha_blend(cache.array[y_i+x_j],color,alpha);
                    }
                }
            }
        }
    }
}

void lui_draw_line(int s_x, int s_y, int e_x, int e_y, uint16_t color) {
    int maxx2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxy2 = cache.coordinate.point.y + cache.coordinate.size.length;
    if (!((s_x + e_x) < cache.coordinate.point.x || s_x > maxx2 || (s_y + e_y) < cache.coordinate.point.y || s_y > maxy2)) {
        int dx,dy,dx2,dy2,x_inc,y_inc,error,index;
        dx = e_x-s_x;//����x����
        dy = e_y-s_y;//����y����
        if (dx>=0) {
            x_inc = 1;
        } else {
            x_inc = -1;
            dx = -dx;
        }
        if (dy>=0) {
            y_inc = 1;
        } else {
            y_inc = -1;
            dy    = -dy;
        }
        dx2 = dx << 1;
        dy2 = dy << 1;
        if (dx > dy) {
            error = dy2 - dx;
            for (index=0; index <= dx; index++) {
                l_point(s_x,s_y,maxx2,maxy2,color);
                if (error >= 0) {
                    error-=dx2;
                    s_y+=y_inc;
                }
                error+=dy2;
                s_x+=x_inc;
            }
        } else {
            error = dx2 - dy;
            for (index=0; index <= dy; index++) {
                l_point(s_x,s_y,maxx2,maxy2,color);
                if (error >= 0) {
                    error-=dy2;
                    s_x+=x_inc;
                }
                error+=dx2;
                s_y+=y_inc;
            }
        }
    }
}

void lui_draw_empty_frame( int x1, int y1, int x2, int y2, uint16_t color ) {
    lui_draw_line(x1,y1,x2,y1,color);
    lui_draw_line(x1,y2,x2,y2,color);
    lui_draw_line(x1,y1,x1,y2,color);
    lui_draw_line(x2,y1,x2,y2,color);
}

void lui_draw_arc( int x0, int y0, int r, int s, uint16_t color ) {
    int maxx2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxy2 = cache.coordinate.point.y + cache.coordinate.size.length;
    int x,y,xd,yd,e;
    if ( x0<0 ) return;
    if ( y0<0 ) return;
    if ( r<=0 ) return;
    xd = 1 - (r << 1); yd = 0; e = 0; x = r; y = 0;
    while ( x >= y ) {
        // Q1
        if ( s & 0x01 ) {
            l_point(x0 + x,y0 - y,maxx2,maxy2,color);
        }
        if ( s & 0x02 ) {
            l_point(x0 + y,y0 - x,maxx2,maxy2,color);
        }
        // Q2
        if ( s & 0x04 ) {
            l_point(x0 - y,y0 - x,maxx2,maxy2,color);
        }
        if ( s & 0x08 ) {
            l_point(x0 - x,y0 - y,maxx2,maxy2,color);
        }
        // Q3
        if ( s & 0x10 ) {
            l_point(x0 - x,y0 + y,maxx2,maxy2,color);
        }
        if ( s & 0x20 ) {
            l_point(x0 - y,y0 + x,maxx2,maxy2,color);
        }

        // Q4
        if ( s & 0x40 ) {
            l_point(x0 + y,y0 + x,maxx2,maxy2,color);
        }
        if ( s & 0x80 ) {
            l_point(x0 + x,y0 + y,maxx2,maxy2,color);
        }
        y++;
        e += yd;
        yd += 2;
        if ( ((e << 1) + xd) > 0 ) {
            x--;
            e += xd;
            xd += 2;
        }
    }
}

void lui_draw_circle( int x0, int y0, int r, uint16_t c ) {
    int maxx = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxy = cache.coordinate.point.y + cache.coordinate.size.length;
    int x,y,xd,yd,e;
    if ( x0<0 ) return;
    if ( y0<0 ) return;
    if ( r<=0 ) return;
    xd = 1 - (r << 1); yd = 0; e = 0; x = r; y = 0;
    while ( x >= y ) {
        l_point(x0 - x, y0 + y, maxx, maxy, c);
        l_point(x0 - x, y0 - y, maxx, maxy, c);
        l_point(x0 + x, y0 + y, maxx, maxy, c);
        l_point(x0 + x, y0 - y, maxx, maxy, c);
        l_point(x0 - y, y0 + x, maxx, maxy, c);
        l_point(x0 - y, y0 - x, maxx, maxy, c);
        l_point(x0 + y, y0 + x, maxx, maxy, c);
        l_point(x0 + y, y0 - x, maxx, maxy, c);
        y++;
        e += yd;
        yd += 2;
        if ( ((e << 1) + xd) > 0 ) {
            x--;
            e += xd;
            xd += 2;
        }
    }
}

void lui_draw_fill_circle( int x0, int y0, int r, uint16_t c ) {
   int x,y,xd;
   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;
   xd = 3 - (r << 1);
   x = 0;
   y = r;
   while ( x <= y ) {
     if( y > 0 ) {
        lui_draw_line(x0 - x, y0 - y,x0 - x, y0 + y, c);
        lui_draw_line(x0 + x, y0 - y,x0 + x, y0 + y, c);
     }
     if( x > 0 ) {
        lui_draw_line(x0 - y, y0 - x,x0 - y, y0 + x, c);
        lui_draw_line(x0 + y, y0 - x,x0 + y, y0 + x, c);
     }
     if ( xd < 0 ) {
        xd += (x << 2) + 6;
     } else {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
   lui_draw_circle(x0, y0, r,c);
}

void lui_draw_round_frame( int x1, int y1, int x2, int y2, int r, uint16_t c ) {
   int n;
   if ( x2 < x1 ) {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 ) {
      n = y2;
      y2 = y1;
      y1 = n;
   }
   if ( r > x2 ) return;
   if ( r > y2 ) return;
   lui_draw_line(x1+r, y1, x2-r, y1, c);
   lui_draw_line(x1+r, y2, x2-r, y2, c);
   lui_draw_line(x1, y1+r, x1, y2-r, c);
   lui_draw_line(x2, y1+r, x2, y2-r, c);
   lui_draw_arc(x1+r, y1+r, r, 0x0C, c);
   lui_draw_arc(x2-r, y1+r, r, 0x03, c);
   lui_draw_arc(x1+r, y2-r, r, 0x30, c);
   lui_draw_arc(x2-r, y2-r, r, 0xC0, c);
}

void lui_draw_fill_round_frame( int x1, int y1, int x2, int y2, int r, int c ) {
   int  x,y,xd;
   x2 += x1;
   y2 += y1;
   if ( x2 < x1 ) {
      x = x2;
      x2 = x1;
      x1 = x;
   }
   if ( y2 < y1 ) {
      y = y2;
      y2 = y1;
      y1 = y;
   }
   if ( r<=0 ) return;
   xd = 3 - (r << 1); x = 0; y = r;
   lui_draw_frame(x1 + r, y1, x2 - r - x1 - r, y2-y1 + 1, 0,c);
   while ( x <= y ) {
     if( y > 0 ) {
        lui_draw_line(x2 + x - r, y1 - y + r, x2+ x - r, y + y2 - r, c);
        lui_draw_line(x1 - x + r, y1 - y + r, x1- x + r, y + y2 - r, c);
     }
     if( x > 0 ) {
        lui_draw_line(x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r, c);
        lui_draw_line(x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r, c);
     }
     if ( xd < 0 ) {
        xd += (x << 2) + 6;
     } else {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
}

void lui_draw_mesh( int x1, int y1, int x2, int y2, uint16_t c ) {
    int maxx2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxy2 = cache.coordinate.point.y + cache.coordinate.size.length;
    if (!((x1 + x2) < cache.coordinate.point.x || x1 > maxx2 || (y1 + y2) < cache.coordinate.point.y || y1 > maxy2)) {
        int n,m;
        if ( x2 < x1 ) {
            n = x2;
            x2 = x1;
            x1 = n;
        }
        if ( y2 < y1 ) {
            n = y2;
            y2 = y1;
            y1 = n;
        }
        for( m=y1; m<=y2; m+=2 )
        {
            for( n=x1; n<=x2; n+=2 )
            {
                l_point(n,m,maxx2,maxy2,c);
            }
        }
    }
}

void lui_draw_text(int s_x, int s_y, uint16_t color, uint8_t bold, char * tex) {
    int ax = s_x;
    while(*tex) {
        lui_draw_char(ax,s_y,*tex++,color,bold,bold);
        ax += (bold*7);
    }
}

void lui_draw_icon(int x, int y, int width, int length, const unsigned int * material) {
    int maxX1 = x + width;
    int maxY1 = y + length;
    int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;
    if (!(maxX1 < cache.coordinate.point.x || x > maxX2 || maxY1 < cache.coordinate.point.y || y > maxY2)) {
        int x1 = 0; int y1 = 0; int x2 = cache.coordinate.size.width; int y2 = cache.coordinate.size.length;
        if(x >= cache.coordinate.point.x) {
            x1 = x-cache.coordinate.point.x;
        }
        if(maxX1 < maxX2) {
            x2 = cache.coordinate.size.width-(maxX2-maxX1);
        }
        if(y >= cache.coordinate.point.y) {
            y1 = y-cache.coordinate.point.y;
        }
        if(maxY1 < maxY2) {
            y2 = cache.coordinate.size.length-(maxY2-maxY1);
        }
        uint16_t ptr = 0;
        if(y < cache.coordinate.point.y) {
            ptr = (cache.coordinate.point.y-y)*width;
        }
        if(x < cache.coordinate.point.x) {
            ptr += (cache.coordinate.point.x-x);
        }
        for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
            for(int x_j = x1; x_j < x2; x_j++) {
                cache.array[y_i+x_j] = lui_alpha_blend(cache.array[y_i+x_j],material[ptr],0);//material[ptr];;//alpha_blend(box.color,cache.array[val],95);
                ptr++;
            }
            if((x+width) > maxX2) {
                ptr += ( (x+width) - maxX2);
            }
            if(x < cache.coordinate.point.x) {
                ptr += (cache.coordinate.point.x-x);
            }
        }
    }
}

uint8_t acGUI_FontConsolas15_0022[ 341] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xF0, 0x00, 0x00, 0x17, 0xEF, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x07, 0xEF, 0xFF, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x06, 0xEF, 0xFF, 0xFF, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF5, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xFF, 0xFF, 0xFF, 0xFA, 0x2F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xFF, 0xFF, 0xFB, 0x30, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xFD, 0xFC, 0x40, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF6, 0x60, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0xF0,
    0xFA, 0xBB, 0xBB, 0xBB, 0xBF, 0xFF, 0xFD, 0xBB, 0xBB, 0xBB, 0xF0,
    0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
    0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

uint8_t acGUI_FontInkFree26_0041[ 64] = { /* code 0031, DIGIT ONE */
      0xFF, 0xFF, 0xFF, 0xFF,
      0xF0, 0x00, 0xB9, 0xF0,
      0xF0, 0x0A, 0xE9, 0xF0,
      0xF1, 0xBA, 0x7B, 0xF0,
      0xFD, 0x70, 0x6B, 0xF0,
      0xF3, 0x00, 0x5C, 0xF0,
      0xF0, 0x00, 0x4D, 0xF0,
      0xF0, 0x00, 0x3F, 0xF0,
      0xF0, 0x00, 0x2F, 0xF0,
      0xF0, 0x00, 0x1F, 0xF0,
      0xF0, 0x00, 0x0F, 0xF0,
      0xF0, 0x00, 0x0D, 0xF0,
      0xF0, 0x00, 0x0B, 0xF0,
      0xF0, 0x00, 0x09, 0xF0,
      0xF0, 0x00, 0x06, 0xF0,
      0xFF, 0xFF, 0xFF, 0xFF
};

uint16_t one_test[] = {
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xc638, 0xc618, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe73c, 0x8430, 0x2124, 0x0, 0x630c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0x528a, 0x0, 0x0, 0x841, 0x0, 0x9cd3, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0x52aa, 0x6b4d, 0xce59, 0x632c, 0x0, 0xce79, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x2965, 0x841, 0xffdf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf7be, 0x0, 0x39e7, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xbdf7, 0x0, 0x73ae, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x8c51, 0x0, 0xad55, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x528a, 0x0, 0xe71c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x18c3, 0x18c3, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe71c, 0x0, 0x528a, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0x528a, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2124, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0x2124, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x52aa, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
};

void lui_draw_iconxx(int x, int y, int width, int length) {
//     int df = 50 + 20*800;
//     for(int j = 0; j < 16;j++) {
//         for(int i = 0; i < 4;i++) {
// //            cache.array[df+(i*2)] = lui_alpha_blend(cache.array[df+(i*2)],0x0000,0xf-(( (acGUI_FontConsolas15_0022[j*11+i] >>8) | 0x0f)*0x0f));
// //            cache.array[df+(i*2)+1] = lui_alpha_blend(cache.array[df+(i*2)],0x0000,0x100-((acGUI_FontConsolas15_0022[j*11+i] | 0x0f )*0x0f));;
//             cache.array[df+(i*2)] = lui_alpha_blend(cache.array[df+(i*2)],0xffff,acGUI_FontInkFree26_0041[j*4+i]);
//             cache.array[df+(i*2)+1] = lui_alpha_blend(cache.array[df+(i*2)],0xffff,acGUI_FontInkFree26_0041[j*4+i] );
//         }
//         df = j*800+50+ 20*800;
//     }

    int df = 50 + 16*800;
    for(int j = 0; j < 16;j++) {
        for(int i = 0; i < 16;i++) {
            if(one_test[j*16+i] != 0xffff) {
                cache.array[df+i] = one_test[j*16+i];
            }
        }
        df = j*800+50+ 16*800;
    }
}


void lui_draw_png(int x, int y, int width, int length, const uint8_t * material) {
    int maxX1 = x + width;
    int maxY1 = y + length;
    int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

    int l_x = f_layout.point.x + f_layout.size.width;
    int l_y = f_layout.point.y + f_layout.size.length;

    if (!(maxX1 < f_layout.point.x || x > l_x
                    || maxY1 < f_layout.point.y || y > l_y )) {
        if (!(maxX1 < cache.coordinate.point.x || x > maxX2 || maxY1 < cache.coordinate.point.y || y > maxY2)) {
            int x1 = 0; int y1 = 0; int x2 = cache.coordinate.size.width; int y2 = cache.coordinate.size.length;

            uint8_t bbb = 0;
            uint16_t ptr = 0;
            if(f_layout.point.x > x) {
                ptr += (f_layout.point.x-x);
                bbb = ptr;
                x = f_layout.point.x;
            }

            if(f_layout.point.y > y) {
                ptr += (f_layout.point.y-y)*width;
                y = f_layout.point.y;
            }

            uint8_t aaa = 0;
            if(maxX1 > l_x) {
                aaa = maxX1-l_x;
                maxX1 = l_x;
            }

            if(maxY1 > l_y) {
                maxY1 = l_y;
            }

            if(x >= cache.coordinate.point.x) {
                x1 = x-cache.coordinate.point.x;
            }
            if(maxX1 < maxX2) {
                x2 = cache.coordinate.size.width-(maxX2-maxX1);
            }
            if(y >= cache.coordinate.point.y) {
                y1 = y-cache.coordinate.point.y;
            }
            if(maxY1 < maxY2) {
                y2 = cache.coordinate.size.length-(maxY2-maxY1);
            }


            if(y < cache.coordinate.point.y) {
                ptr += (cache.coordinate.point.y-y)*width;
            }
            if(x < cache.coordinate.point.x) {
                ptr += (cache.coordinate.point.x-x);
            }
            ptr *= 3;
            for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
                for(int x_j = x1; x_j < x2; x_j++) {
                    uint16_t color = (uint16_t)( material[ptr+1]<<8)+material[ptr];
                    cache.array[y_i+x_j] = lui_alpha_blend(cache.array[y_i+x_j],color, 0xff-material[ptr+2]);//material[ptr];;//alpha_blend(box.color,cache.array[val],95);
                    ptr += 3;
                }
                ptr += bbb*3;
                if((x+width) > maxX2) {
                    ptr += ( (x+width) - maxX2) * 3;
                }
                if(x < cache.coordinate.point.x) {
                    ptr += (cache.coordinate.point.x-x) * 3;
                }
                if(aaa > 0) {
                    ptr += aaa*3;
                }
            }
        }
    }
}

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);        //�õ�����һ���ַ���Ӧ������ռ���ֽ���
     num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩

      int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

    for(t=0;t<csize;t++)
    {
        if(size==12)temp=asc2_1206[num][t];          //����1206����
        else if(size==16)temp=asc2_1608[num][t];    //����1608����
        else if(size==24)temp=asc2_2412[num][t];    //����2412����
        else return;                                //û�е��ֿ�
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)l_point(x,y,maxX2,maxY2,0X3030);
            else if(mode==0)l_point(x,y,maxX2,maxY2,0XFFFF);
            temp<<=1;
            y++;
            if(y>=240)return;//��������
            if((y-y0)==size)
            {
                y=y0;
                x++;
                if(x>=320)return;//��������
                break;
            }
        }
    }
}


void lui_draw_font_gbk16(uint16_t x, uint16_t y, uint16_t fc, char *s) {
    unsigned char i,j,c;
    unsigned short k,x0;
    int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;
    x0=x;
    while(*s) {
        if((*s) < 128) {
            k=*s;
            if (k==13) {
                x=x0;
                y+=16;
            } else {
                if (k>32) k-=32; else k=0;
                for(i=0;i<16;i++) {
                    for(j=0;j<8;j++) {
                        if(asc16[k*16+i]&(0x80>>j)) l_point(x+j,y+i,maxX2,maxY2,fc);
                    }
                }
                x+=8;
//                for(i=0;i<22;i++) {
//                    for(j=0;j<2;j++) {
//                        c=*(consolas11+k*22*2+i*2+j);
//                        for (int b_k=0;b_k<8;b_k++) {
//                            if(c&(0x80>>b_k))    l_point(x+j*8+b_k,y+i,maxX2,maxY2,fc);
//                        }
//                    }
//                }
//                x+=10;
            }
            s++;
        } else {
//            for (k=0;k<hz16_num;k++) {
//                if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1))) {
//                    for(i=0;i<16;i++) {
//                        for(j=0;j<8;j++) {
//                            if(hz16[k].Msk[i*2]&(0x80>>j)) l_point(x+j,y+i,maxX2,maxY2,fc);
//                        }
//                        for(j=0;j<8;j++) {
//                            if(hz16[k].Msk[i*2+1]&(0x80>>j)) l_point(x+j+8,y+i,maxX2,maxY2,fc);
//                        }
//                    }
//                }
//            }
            s+=2;x+=16;
        }
    }
}


void lui_draw_char(uint16_t x, int16_t y, unsigned char c,
              uint16_t color, uint8_t sx, uint8_t sy)  {
    if(c>127) return;
    if((x >= LCD_WIDTH) || // Clip right
      (y >= LCD_LENGTH) || // Clip bottom
      ((x + 8 * sx - 1) < 0)  || // Clip left
      ((y + 16 * sy - 1) < 0))    // Clip top
            return;
    uint16_t recIdx = font_7x8_CharOffs[c];
    uint16_t recNum = font_7x8_CharOffs[c+1]-recIdx;
    for(int i=0; i<recNum; i++) {
        int v = font_7x8_Rects[i+recIdx];
        int xf = v & 0xf;
        int yf = 1+((v & 0xf0)>>4);
        int wf = 1+((v & 0xf00)>>8);
        int hf = 1+((v & 0xf000)>>12);
        lui_draw_frame(x+xf*sx, y+yf*sy, wf*sx, hf*sy, 0,color);
    }
}


void lui_set_cache_size(int x, int y, int width, int length) {
    for(int i = 0; i < CACHE_SIZE; i++) {
        cache.array[i] = 0x00;
    }
    cache.coordinate.point.x = x;
    cache.coordinate.point.y = y;
    cache.coordinate.size.width = width;
    cache.coordinate.size.length = length;
}

lui_layout_t * lui_get_cache_size(void) {
    return &cache.coordinate;
}

extern void lsdl_box(int x, int y, int width, int length, uint16_t * color);
void lui_cachedev(int x, int y, int width, int length) {
    lsdl_box(x,y,width,length,cache.array);
}

//
//const uint8_t fs[] = {
//0x00,0x00,0x00,0x00,0x38,0x0C,0x3C,0x1C,0x1E,0x3C,0x0E,0x78,0x06,0xF0,0x01,0xC0,
//0x03,0x80,0x07,0x70,0x0F,0x78,0x1E,0x3C,0x3C,0x1C,0x38,0x0C,0x00,0x00,0x00,0x00,
//};
//
//void Gui_DrawFont_Num642(int x, int y, int fc, uint16_t num)
//{
//    unsigned char i,j,k,c;
//    //lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//    //w=w/8;
//    int maxX2 = cache.coordinate.x + cache.coordinate.size.width;
//    int maxY2 = cache.coordinate.y + cache.coordinate.size.length;
//    for(i=0;i<16;i++) {
//        for(j=0;j<2;j++) {
//            c=*(fs+num*16*2+i*2+j);
//            for (k=0;k<8;k++) {
//                if(c&(0x80>>k))    l_point(x+j*8+k,y+i,maxX2,maxY2,fc);
//            }
//        }
//    }
//}
//
//

//int s[3][3] = {1,2,1,2,4,2,1,2,1};
//int a,b,c;
//
//float pixel_value(int x,int y) {
//    int i,j;
//    static int k = 0;
//    float q = 0.0,d,d1,d2,t1,t2;
//    if(k == 1) {
//        t1 = a * (x - 1.0/3) + b * (y+1.0/3) + c;
//        d1 = 4 * t1 * t1 - a*a - b*b;
//        t2 = a * (x + 1.0/3) + b * (y - 1.0/3) + c;
//        d2 = 4 * t2 * t2 - a * a - b*b;
//        if(d1 <= 0 && d2 <= 0) {
//            q = 1;
//        } else {
//            //q = color - value(x,y);
//        }
//        if(!k) {
//            t1 = a*x + b * y + c;
//            d = 4*t1*t1 - a*a - b*b;
//            if(d> 0) {
//                for(i =0; i < 3; i++) {
//                    t1 = a*(x+1.0/3*(i-1)) + b * (y+1.0/3) + c;
//                    d = 4*t1*t1 - a*a - b*b;
//                    if(d <= 0) {
//                        q += (float)s[i][2]/16;
//                    }
//                }
//            }
//        }
//    }
//}


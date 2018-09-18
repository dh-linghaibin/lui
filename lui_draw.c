/*
 * This file is part of the lui_draw.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_draw.h"
#include <math.h>

typedef struct _lcache {
    uint16_t array[CACHE_SIZE];
    lui_layout_t coordinate;
} lui_cache;

static lui_cache cache; 
lui_layout_t f_layout;

static void l_point(int s_x, int s_y ,int m_x, int m_y, uint16_t color) {
    if( ( s_x  >= cache.coordinate.point.x && s_x  < m_x ) &&
    (  s_y >= cache.coordinate.point.y && s_y < m_y ) ) {
        int xx = (s_x-cache.coordinate.point.x);
        int yy = (s_y-cache.coordinate.point.y);
        cache.array[yy*cache.coordinate.size.width+xx] = color;
    }
}

static void l_point_alpha(int s_x, int s_y ,int m_x, int m_y, uint16_t color,uint8_t alpha) {
    if( ( s_x  >= cache.coordinate.point.x && s_x  < m_x ) &&
    (  s_y >= cache.coordinate.point.y && s_y < m_y ) ) {
        int xx = (s_x-cache.coordinate.point.x);
        int yy = (s_y-cache.coordinate.point.y);
        int pos = yy*cache.coordinate.size.width+xx;
        cache.array[pos] = lui_alpha_blend(cache.array[pos],color,alpha);
    }
}

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

int Sgn(float x)
{
if(x>0)
return 1;
else
return -1;
}

void cg_antialiased_line(int x1,int y1,int x2,int y2,uint16_t crColor)  
{  
    int d, x, y, ax, ay, sx, sy, dx, dy, fx, fy, bk;  
      
    dx = x2-x1;  fx=abs(dx);  ax = fx<<1;  sx = Sgn(dx);
    dy = y2-y1;  fy=abs(dy);  ay = fy<<1;  sy = Sgn(dy);
      
    x = x1;  
    y = y1;  
  
    //// +    
    int incrE,incrNE;         
    // double invDenom=20.0/(2.0*sqrt(1.0*dx*dx+dy*dy));  
    // double temp=0;  
    // double temp2=0;  
    //// +  
  
    if (ax>ay) {     /* x dominant */  
        // d = ay-fx;  
        // /// +  
        // int two_v_dx=0;  
        // incrE=(dy<<1);  
        // incrNE=((dy-dx)<<2);  
        // double two_dx_invDenom=2.0*dx*invDenom;  
        // /// +  
        // for (;;) {  
        //     /// +  
        //     temp=two_v_dx*invDenom;  
        //     temp2=sx>0?temp:-temp;  
        //     // bk=getpixel(x,y);putpixel(x,y,LERP_COLOR(crColor,bk,WCG_FILTER(temp)));  
        //     // bk=getpixel(x,y+sy);putpixel(x,y+sy,LERP_COLOR(crColor,bk,WCG_FILTER(two_dx_invDenom-temp2)));  
        //     // bk=getpixel(x,y-sy);putpixel(x,y-sy,LERP_COLOR(crColor,bk,WCG_FILTER(two_dx_invDenom+temp2)));  
        //     /// +  
        //     if (x==x2) return;
        //     if (d>=0) {  
        //         two_v_dx=d-fx;   /// +
        //         y += sy;  
        //         d -= ax;
        //     }
        //     else
        //         two_v_dx=d+fx;   /// +
        //     x += sx;
        //     d += ay;
              
        // }  
    }  
    else {          /* y dominant */
        d = ax-fy;
        /// +  
        int two_v_dy=0;
        // incrE=(dx<<1);
        // incrNE=((dx-dy)<<1);
        // double two_dy_invDenom=300*dy*invDenom;
        /// +
        for (;;) {
            /// +  
            // temp=two_v_dy*invDenom;
            // temp2=sy>0?temp:-temp;
            l_point_alpha(x,y,800,480,crColor,0);
            // l_point_alpha(x+sx,y,800,480,crColor,two_dy_invDenom-temp2);
            // l_point_alpha(x-sx,y,800,480,crColor,two_dy_invDenom-temp2);
            /// +  
            if (y==y2) return;
            if (d>=0) {
                two_v_dy=d-fy;  /// +
                x += sx;
                d -= ay;
            }
            else
            two_v_dy=d+fy;   /// +
            y += sy;
            d += ax;
        }
    }
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
        dx = e_x-s_x;
        dy = e_y-s_y;
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
    lui_draw_frame(x1 + r, y1, x2 - r - x1 - r, (y2-y1), 100,c);
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

void lui_draw_font(int x, int y, uint8_t wighth, uint8_t length, uint16_t color, uint8_t * mate) {
    uint8_t font_w = wighth;
    uint8_t font_l = length;

    int maxX1 = x + font_w;
    int maxY1 = y + font_l;
    int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
    int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

    int l_x = f_layout.point.x + f_layout.size.width;
    int l_y = f_layout.point.y + f_layout.size.length;

    if (!(maxX1 < f_layout.point.x || x > l_x
                    || maxY1 < f_layout.point.y || y > l_y )) {
        if (!(maxX1 < cache.coordinate.point.x || x > maxX2 || maxY1 < cache.coordinate.point.y || y > maxY2)) {
            int x1 = 0; int y1 = 0; int x2 = cache.coordinate.size.width; int y2 = cache.coordinate.size.length;

            uint16_t deviation_y = 0;
            uint32_t ptr = 0;
            if(f_layout.point.x > x) {
                ptr += (f_layout.point.x-x);
                deviation_y = ptr;
                x = f_layout.point.x;
            }

            if(f_layout.point.y > y) {
                ptr += (f_layout.point.y-y)*font_w;
                y = f_layout.point.y;
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
                ptr += (cache.coordinate.point.y-y)*font_w;
            }
            if(x < cache.coordinate.point.x) {
                ptr += (cache.coordinate.point.x-x);
            }
            for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
                for(int x_j = x1; x_j < x2; x_j++) {
                    if(mate[ptr] != 0) {
                        if(mate[ptr] == 0xff) {
                            cache.array[y_i+x_j] = color;
                        } else {
                            cache.array[y_i+x_j] = lui_alpha_blend(color,cache.array[y_i+x_j],mate[(ptr)]);
                        }
                    }
                    ptr++;
                }
                ptr += deviation_y;
                if((x+font_w) > maxX2) {
                    ptr += ( (x+font_w) - maxX2);
                }
                if(x < cache.coordinate.point.x) {
                    ptr += (cache.coordinate.point.x-x);
                }
            }
        }
    }
}

void lui_draw_jpg(int x, int y, int width, int length, uint8_t * material) {
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

            uint16_t deviation_y = 0;
            uint32_t ptr = 0;
            if(f_layout.point.x > x) {
                ptr += (f_layout.point.x-x);
                deviation_y = ptr;
                x = f_layout.point.x;
            }

            if(f_layout.point.y > y) {
                ptr += (f_layout.point.y-y)*width;
                y = f_layout.point.y;
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
            ptr *= 2;
            for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
                for(int x_j = x1; x_j < x2; x_j++) {
                    cache.array[y_i+x_j] = (uint16_t)( material[ptr+1]<<8)+material[ptr];
                    ptr += 2;
                }
                ptr += deviation_y*2;
                if((x+width) > maxX2) {
                    ptr += ( (x+width) - maxX2) * 2;
                }
                if(x < cache.coordinate.point.x) {
                    ptr += (cache.coordinate.point.x-x) * 2;
                }
            }
        }
    }
}

void lui_draw_png(int x, int y, int width, int length, uint8_t * material) {
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

            uint16_t deviation_y = 0;
            uint16_t ptr = 0;
            if(f_layout.point.x > x) {
                ptr += (f_layout.point.x-x);
                deviation_y = ptr;
                x = f_layout.point.x;
            }

            if(f_layout.point.y > y) {
                ptr += (f_layout.point.y-y)*width;
                y = f_layout.point.y;
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
                    if(material[ptr+2] != 0) {
                    uint16_t color = (uint16_t)( material[ptr+1]<<8)+material[ptr];
                    cache.array[y_i+x_j] = lui_alpha_blend(cache.array[y_i+x_j],color, 0xff-material[ptr+2]);
                    }
                    ptr += 3;
                }
                ptr += deviation_y*3;
                if((x+width) > maxX2) {
                    ptr += ( (x+width) - maxX2) * 3;
                }
                if(x < cache.coordinate.point.x) {
                    ptr += (cache.coordinate.point.x-x) * 3;
                }
            }
        }
    }
}

void lui_draw_screen(int x, int y, int width, int length, uint32_t * material) {
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

            uint16_t deviation_y = 0;
            uint32_t ptr = 0;
            if(f_layout.point.x > x) {
                ptr += (f_layout.point.x-x);
                deviation_y = ptr;
                x = f_layout.point.x;
            }

            if(f_layout.point.y > y) {
                ptr += (f_layout.point.y-y)*width;
                y = f_layout.point.y;
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
            for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
                for(int x_j = x1; x_j < x2; x_j++) {
                    cache.array[y_i+x_j] = (uint16_t)( material[ptr] );
                    ptr++;
                }
                ptr += deviation_y;
                if((x+width) > maxX2) {
                    ptr += ( (x+width) - maxX2);
                }
                if(x < cache.coordinate.point.x) {
                    ptr += (cache.coordinate.point.x-x);
                }
            }
        }
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

typedef int16_t lv_coord_t;
typedef uint8_t lv_opa_t;


#define LV_MATH_MIN(a,b) (a<b?a:b)
#define LV_MATH_MAX(a,b) (a>b?a:b)
#define LV_MATH_ABS(x) ((x)>0?(x):(-(x)))

typedef struct
{
    lv_coord_t x;
    lv_coord_t y;
}lv_point_t;

typedef struct
{
    lv_coord_t x1;
    lv_coord_t y1;
    lv_coord_t x2;
    lv_coord_t y2;
}lv_area_t;

#define CIRCLE_AA_NON_LINEAR_OPA_THRESHOLD  5   /*Circle segment greater then this value will be anti-aliased by a non-linear (cos) opacity mapping*/

#define LINE_WIDTH_CORR_BASE 64
#define LINE_WIDTH_CORR_SHIFT 6

#define LABEL_RECOLOR_PAR_LENGTH    6

#define SHADOW_OPA_EXTRA_PRECISION      0       /*Calculate with 2^x bigger shadow opacity values to avoid rounding errors*/
#define SHADOW_BOTTOM_AA_EXTRA_RADIUS   3       /*Add extra radius with LV_SHADOW_BOTTOM to cover anti-aliased corners*/

#define LV_OPA_TRANSP    0
#define LV_OPA_0         0
#define LV_OPA_10        25
#define LV_OPA_20        51
#define LV_OPA_30        76
#define LV_OPA_40        102
#define LV_OPA_50        127
#define LV_OPA_60        153
#define LV_OPA_70        178
#define LV_OPA_80        204
#define LV_OPA_90        229
#define LV_OPA_100       255
#define LV_OPA_COVER     255
/**
 * Copy an area
 * @param dest pointer to the destination area
 * @param src pointer to the source area
 */
inline static void lv_area_copy(lv_area_t * dest, const lv_area_t * src)
{
    memcpy(dest, src, sizeof(lv_area_t));
}

/**
 * Get the width of an area
 * @param area_p pointer to an area
 * @return the width of the area (if x1 == x2 -> width = 1)
 */
static inline lv_coord_t lv_area_get_width(const lv_area_t * area_p)
{
    return area_p->x2 - area_p->x1 + 1;
}

/**
 * Get the height of an area
 * @param area_p pointer to an area
 * @return the height of the area (if y1 == y2 -> height = 1)
 */
static inline lv_coord_t lv_area_get_height(const lv_area_t * area_p)
{
    return area_p->y2 - area_p->y1 + 1;
}

static lv_opa_t antialias_get_opa(lv_coord_t seg, lv_coord_t px_id, lv_opa_t line_opa)
{
    /* How to calculate the opacity of pixels on the edges which makes the anti-aliasing?
     * For example we have a line like this (y = -0.5 * x):
     *
     *  | _ _
     *    * * |
     *
     * Anti-aliased pixels come to the '*' characters
     * Calculate what percentage of the pixels should be covered if real line (not rasterized) would be drawn:
     * 1. A real line should start on (0;0) and end on (2;1)
     * 2. So the line intersection coordinates on the first pixel: (0;0) (1;0.5) -> 25% covered pixel in average
     * 3. For the second pixel: (1;0.5) (2;1) -> 75% covered pixel in average
     * 4. The equation: (px_id * 2 + 1) / (segment_width * 2)
     *                   segment_width: the line segment which is being anti-aliased (was 2 in the example)
     *                   px_id: pixel ID from 0 to  (segment_width - 1)
     *                   result: [0..1] coverage of the pixel
     */

    /*Accelerate the common segment sizes to avoid division*/
    static const  lv_opa_t seg1[1] = {128};
    static const  lv_opa_t seg2[2] = {64, 192};
    static const  lv_opa_t seg3[3] = {42, 128, 212};
    static const  lv_opa_t seg4[4] = {32, 96, 159, 223};
    static const  lv_opa_t seg5[5] = {26,  76, 128, 178, 230};
    static const  lv_opa_t seg6[6] = {21,  64, 106, 148, 191, 234};
    static const  lv_opa_t seg7[7] = {18,  55, 91, 128, 164, 200, 237};
    static const  lv_opa_t seg8[8] = {16,  48, 80, 112, 143, 175, 207, 239};

    static const lv_opa_t * seg_map[] = {seg1, seg2, seg3, seg4,
                                         seg5, seg6, seg7, seg8};

    if(seg == 0) return LV_OPA_TRANSP;
    else if(seg < 8) return (uint32_t)((uint32_t)seg_map[seg - 1][px_id] * line_opa) >> 8;
    else {
        return ((px_id * 2 + 1) * line_opa) / (2 * seg);
    }

}

/**
 * Approximate the opacity for anti-aliasing.
 * Used  the first segment of a circle which is the longest and have the most non-linearity (cos)
 * @param seg
 * @param px_id
 * @param line_opa
 * @return
 */
static lv_opa_t antialias_get_opa_circ(lv_coord_t seg, lv_coord_t px_id, lv_opa_t opa)
{
    static const  lv_opa_t opa_map[8] = {250, 242, 221, 196, 163, 122, 74, 18};

    if(seg == 0) return LV_OPA_TRANSP;
    else if(seg == 1) return LV_OPA_80;
    else {

        uint8_t id = (uint32_t) ((uint32_t)px_id * (sizeof(opa_map) - 1)) / (seg - 1);
        return (uint32_t) ((uint32_t) opa_map[id] * opa) >> 8;

    }

}

/**
 * Draw a line
 * @param p1 first point of the line
 * @param p2 second point of the line
 * @param maskthe line will be drawn only on this area
 * @param lines_p pointer to a line style
 */
void lv_draw_line(const lv_point_t * point1, const lv_point_t * point2, const lv_area_t * mask)
{

    // if(style->line.width == 0) return;
    if(point1->x == point2->x && point1->y == point2->y) return;

    lv_point_t p1;
    lv_point_t p2;

    /*Be sure always x1 < x2*/
    if(point1->x < point2->x) {
        p1.x = point1->x;
        p1.y = point1->y;
        p2.x = point2->x;
        p2.y = point2->y;
    } else {
        p1.x = point2->x;
        p1.y = point2->y;
        p2.x = point1->x;
        p2.y = point1->y;
    }

    lv_coord_t dx = LV_MATH_ABS(p2.x - p1.x);
    lv_coord_t sx = p1.x < p2.x ? 1 : -1;
    lv_coord_t dy = LV_MATH_ABS(p2.y - p1.y);
    lv_coord_t sy = p1.y < p2.y ? 1 : -1;
    lv_coord_t err = (dx > dy ? dx : -dy) / 2;
    lv_coord_t e2;
    uint8_t hor = dx > dy ? 1 : 0;	/*Rather horizontal or vertical*/
    lv_coord_t last_x = p1.x;
    lv_coord_t last_y = p1.y;
    lv_point_t act_point;
    act_point.x = p1.x;
    act_point.y = p1.y;


    lv_coord_t width;
    uint16_t wcor;
    uint16_t width_half = 0;
    uint16_t width_1 = 0;
    static const uint8_t width_corr_array[] = {
            64, 64, 64, 64, 64, 64, 64, 64, 64, 65, 65, 65, 65, 65, 66, 66, 66, 66, 66,
            67, 67, 67, 68, 68, 68, 69, 69, 69, 70, 70, 71, 71, 72, 72, 72, 73, 73, 74,
            74, 75, 75, 76, 77, 77, 78, 78, 79, 79, 80, 81, 81, 82, 82, 83, 84, 84, 85,
            86, 86, 87, 88, 88, 89, 90, 91,
    };

    if(hor == 0) {
        wcor = (dx * LINE_WIDTH_CORR_BASE) / dy;
    } else  {
        wcor = (dy * LINE_WIDTH_CORR_BASE) / dx;
    }


    // width = style->line.width - 1;
    width = 5 - 1;

    uint8_t aa_invert = 0;
    aa_invert = p1.y < p2.y ? 0 : 1;      /*Direction of opacity increase on the edges*/
    if(p1.x != p2.x && p1.y != p2.y) width--;    /*Because of anti aliasing (no anti aliasing on hor. and ver. lines)*/

    /*Make the correction on lie width*/
    if(width > 0) {
        width = (width * width_corr_array[wcor]);
        width = width >> LINE_WIDTH_CORR_SHIFT;
        width_half = width >> 1;
        width_1 = width & 0x1 ? 1 : 0;
    }

    /*Special case draw a horizontal line*/
    if(p1.y == p2.y ) {
        lv_area_t act_area;
        act_area.x1 = p1.x;
        act_area.x2 = p2.x;
        act_area.y1 = p1.y - width_half - width_1;
        act_area.y2 = p2.y + width_half ;

        lv_area_t draw_area;
        draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
        draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
        draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
        draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
        // fill_fp(&draw_area, mask, style->line.color, style->line.opa);
        return;
    }

    /*Special case draw a vertical line*/
    if(p1.x == p2.x ) {
        lv_area_t act_area;
        act_area.x1 = p1.x - width_half;
        act_area.x2 = p2.x + width_half + width_1;
        act_area.y1 = p1.y;
        act_area.y2 = p2.y;

        lv_area_t draw_area;
        draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
        draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
        draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
        draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
        // fill_fp(&draw_area, mask, style->line.color, style->line.opa);
        return;
    }


    while(1){
        if(hor == 1 && last_y != act_point.y) {
            lv_area_t act_area;
            lv_area_t draw_area;
            act_area.x1 = last_x;
            act_area.x2 = act_point.x - sx;
            act_area.y1 = last_y - width_half ;
            act_area.y2 = act_point.y - sy  + width_half + width_1;
            last_y = act_point.y;
            last_x = act_point.x;
            draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
            draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
            draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
            draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
            if(width >= 0) {
                // fill_fp(&draw_area, mask, style->line.color, style->line.opa);
            }
            lv_coord_t seg_w = lv_area_get_width(&draw_area);  /*Segment width*/
            lv_point_t aa_p1;
            lv_point_t aa_p2;

            aa_p1.x = draw_area.x1;
            aa_p1.y = draw_area.y1 - 1;

            aa_p2.x = draw_area.x1;
            aa_p2.y = draw_area.y1 + width + 1;

            lv_coord_t i;
            for(i = 0; i  < seg_w; i++) {
                // lv_opa_t aa_opa = antialias_get_opa(seg_w, i, style->line.opa);
                lv_opa_t aa_opa = antialias_get_opa(seg_w, i, 5);

                // px_fp(aa_p1.x + i, aa_p1.y, mask, style->line.color, aa_invert ? aa_opa : style->line.opa - aa_opa);
                // px_fp(aa_p2.x + i, aa_p2.y, mask, style->line.color, aa_invert ? style->line.opa - aa_opa :  aa_opa);
            }
        }
        if (hor == 0 && last_x != act_point.x) {
            lv_area_t act_area;
            lv_area_t draw_area;
            act_area.x1 = last_x - width_half;
            act_area.x2 = act_point.x  - sx + width_half + width_1;
            act_area.y1 = last_y ;
            act_area.y2 = act_point.y - sy;
            last_y = act_point.y;
            last_x = act_point.x;

            draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
            draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
            draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
            draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
            if(width >= 0) {
                // fill_fp(&draw_area, mask, style->line.color, style->line.opa);
            }
            lv_coord_t seg_h = lv_area_get_height(&draw_area);  /*Segment height*/
            lv_point_t aa_p1;
            lv_point_t aa_p2;

            aa_p1.x = draw_area.x1 - 1;
            aa_p1.y = draw_area.y1;

            aa_p2.x = draw_area.x1 + width + 1;
            aa_p2.y = draw_area.y1;

            lv_coord_t i;
            for(i = 0; i  < seg_h; i++) {
                // lv_opa_t aa_opa = antialias_get_opa(seg_h, i, style->line.opa);

                // px_fp(aa_p1.x, aa_p1.y + i, mask, style->line.color, aa_invert ? aa_opa : style->line.opa - aa_opa);
                // px_fp(aa_p2.x, aa_p2.y + i, mask, style->line.color, aa_invert ? style->line.opa - aa_opa :  aa_opa);
            }
        }

        /*Calc. the next point of the line*/
        if (act_point.x == p2.x && act_point.y == p2.y) break;
        e2 = err;
        if (e2 >-dx) {
            err -= dy;
            act_point.x += sx;
        }
        if (e2 < dy) {
            err += dx;
            act_point.y += sy;
        }
    }
    /*Draw the last part of the line*/
    if(hor == 1) {
        lv_area_t act_area;
        lv_area_t draw_area;
        act_area.x1 = last_x;
        act_area.x2 = act_point.x;
        act_area.y1 = last_y - width_half ;
        act_area.y2 = act_point.y + width_half + width_1;

        draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
        draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
        draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
        draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
        if(width >= 0) {
            // fill_fp(&draw_area, mask, style->line.color, style->line.opa);
        }
        lv_coord_t seg_w = lv_area_get_width(&draw_area);  /*Segment width*/
        lv_point_t aa_p1;
        lv_point_t aa_p2;

        aa_p1.x = draw_area.x1;
        aa_p1.y = draw_area.y1 - 1;

        aa_p2.x = draw_area.x1;
        aa_p2.y = draw_area.y1 + width + 1;

        lv_coord_t i;
        for(i = 0; i  < seg_w; i++) {
            // lv_opa_t aa_opa = antialias_get_opa(seg_w, i, style->line.opa);

            // px_fp(aa_p1.x + i, aa_p1.y, mask, style->line.color, aa_invert ? aa_opa : style->line.opa - aa_opa);
            // px_fp(aa_p2.x + i, aa_p2.y, mask, style->line.color, aa_invert ? style->line.opa - aa_opa :  aa_opa);
        }
    }
    if (hor == 0) {
        lv_area_t act_area;
        lv_area_t draw_area;
        act_area.x1 = last_x - width_half;
        act_area.x2 = act_point.x + width_half + width_1;
        act_area.y1 = last_y;
        act_area.y2 = act_point.y;

        draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
        draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
        draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
        draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
        if(width >= 0) {
            // fill_fp(&draw_area, mask, style->line.color, style->line.opa);
        }

        lv_coord_t seg_h = lv_area_get_height(&draw_area);  /*Segment height*/
        lv_point_t aa_p1;
        lv_point_t aa_p2;

        aa_p1.x = draw_area.x1 - 1;
        aa_p1.y = draw_area.y1;

        aa_p2.x = draw_area.x1 + width + 1;
        aa_p2.y = draw_area.y1;

        lv_coord_t i;
        for(i = 0; i  < seg_h; i++) {
            // lv_opa_t aa_opa = antialias_get_opa(seg_h, i, style->line.opa);

            // px_fp(aa_p1.x, aa_p1.y + i, mask, style->line.color, aa_invert ? aa_opa : style->line.opa - aa_opa);
            // px_fp(aa_p2.x, aa_p2.y + i, mask, style->line.color, aa_invert ? style->line.opa - aa_opa :  aa_opa);
        }
    }
}

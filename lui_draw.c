/*
 * This file is part of the lui_draw.c
 *
 *  Copyright (c) : 2018 linghaibin
 *      Author: a6735
 */

#include "lui_draw.h"
#include "lui_font.h"

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

static uint16_t l_point_get(int s_x, int s_y ,int m_x, int m_y) {
    if( ( s_x  >= cache.coordinate.point.x && s_x  < m_x ) &&
    (  s_y >= cache.coordinate.point.y && s_y < m_y ) ) {
        int xx = (s_x-cache.coordinate.point.x);
        int yy = (s_y-cache.coordinate.point.y);
        return cache.array[yy*cache.coordinate.size.width+xx];
    }
}

void DrawWuLine ( short X0, short Y0, short X1, short Y1,
         short BaseColor, short NumLevels, unsigned short IntensityBits)
{
   unsigned short IntensityShift, ErrorAdj, ErrorAcc;
   unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
   short DeltaX, DeltaY, Temp, XDir;

   /* Make sure the line runs top to bottom */
   if (Y0 > Y1) {
      Temp = Y0; Y0 = Y1; Y1 = Temp;
      Temp = X0; X0 = X1; X1 = Temp;
   }
   /* Draw the initial pixel, which is always exactly intersected by
      the line and so needs no weighting */
   l_point(X0, Y0, 800, 800, BaseColor);

   if ((DeltaX = X1 - X0) >= 0) {
      XDir = 1;
   } else {
      XDir = -1;
      DeltaX = -DeltaX; /* make DeltaX positive */
   }
   /* Special-case horizontal, vertical, and diagonal lines, which
      require no weighting because they go right through the center of
      every pixel */
   if ((DeltaY = Y1 - Y0) == 0) {
      /* Horizontal line */
      while (DeltaX-- != 0) {
         X0 += XDir;
         l_point(X0, Y0, 800, 800, BaseColor);
      }
      return;
   }
   if (DeltaX == 0) {
      /* Vertical line */
      do {
         Y0++;
         l_point(X0, Y0, 800, 800, BaseColor);
      } while (--DeltaY != 0);
      return;
   }
   if (DeltaX == DeltaY) {
      /* Diagonal line */
      do {
         X0 += XDir;
         Y0++;
         l_point(X0, Y0, 800, 800, BaseColor);
      } while (--DeltaY != 0);
      return;
   }
   /* Line is not horizontal, diagonal, or vertical */
   ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
   /* # of bits by which to shift ErrorAcc to get intensity level */
   IntensityShift = 16 - IntensityBits;
   /* Mask used to flip all bits in an intensity weighting, producing the
      result (1 - intensity weighting) */
   WeightingComplementMask = NumLevels - 1;
   /* Is this an X-major or Y-major line? */
   if (DeltaY > DeltaX) {
      /* Y-major line; calculate 16-bit fixed-point fractional part of a
         pixel that X advances each time Y advances 1 pixel, truncating the
         result so that we won't overrun the endpoint along the X axis */
      ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
      /* Draw all pixels other than the first and last */
      while (--DeltaY) {
         ErrorAccTemp = ErrorAcc;   /* remember current accumulated error */
         ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
         if (ErrorAcc <= ErrorAccTemp) {
            /* The error accumulator turned over, so advance the X coord */
            X0 += XDir;
         }
         Y0++; /* Y-major, so always advance Y */
         /* The IntensityBits most significant bits of ErrorAcc give us the
            intensity weighting for this pixel, and the complement of the
            weighting for the paired pixel */
         Weighting = ErrorAcc >> IntensityShift;
         l_point(X0, Y0, 800, 800, BaseColor + Weighting);
         l_point(X0 + XDir, Y0, 800, 800,BaseColor + (Weighting ^ WeightingComplementMask));
        //        printf("%d-",DeltaY);
        // l_point(X0, Y0, 800, 800, lui_alpha_blend565(0x00,0xffff,Weighting));
        // l_point(X0 + XDir, Y0, 800, 800, lui_alpha_blend565(0x00,0xffff,Weighting));
        //  printf("%d-",ErrorAcc);
      }
      /* Draw the final pixel, which is 
         always exactly intersected by the line
         and so needs no weighting */
      l_point(X1, Y1, 800, 800, BaseColor);
      return;
   }
   /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
      pixel that Y advances each time X advances 1 pixel, truncating the
      result to avoid overrunning the endpoint along the X axis */
   ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
   /* Draw all pixels other than the first and last */
   while (--DeltaX) {
      ErrorAccTemp = ErrorAcc;   /* remember current accumulated error */
      ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
      if (ErrorAcc <= ErrorAccTemp) {
         /* The error accumulator turned over, so advance the Y coord */
         Y0++;
      }
      X0 += XDir; /* X-major, so always advance X */
      /* The IntensityBits most significant bits of ErrorAcc give us the
         intensity weighting for this pixel, and the complement of the
         weighting for the paired pixel */
      Weighting = ErrorAcc >> IntensityShift;
    //   l_point(X0, Y0, 800, 800, BaseColor + Weighting);
    //   l_point(X0 + XDir, Y0, 800, 800,
    //         BaseColor + (Weighting ^ WeightingComplementMask));
    l_point(X0, Y0, 800, 800, lui_alpha_blend565(0x00,0xffff,DeltaY));
      l_point(X0 + XDir, Y0, 800, 800,lui_alpha_blend565(0x00,0xffff,DeltaY));
   }
   /* Draw the final pixel, which is always exactly intersected by the line
      and so needs no weighting */
   l_point(X1, Y1, 800, 800, BaseColor);
}

void DrawWuLine2( int X0, int Y0, int X1, int Y1, uint16_t clrLine )
{
    /* Make sure the line runs top to bottom */
    if (Y0 > Y1)
    {
        int Temp = Y0; Y0 = Y1; Y1 = Temp;
        Temp = X0; X0 = X1; X1 = Temp;
    }
    
    /* Draw the initial pixel, which is always exactly intersected by
    the line and so needs no weighting */
    l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH, clrLine );
    
    int XDir, DeltaX = X1 - X0;
    if( DeltaX >= 0 )
    {
        XDir = 1;
    }
    else
    {
        XDir   = -1;
        DeltaX = 0 - DeltaX; /* make DeltaX positive */
    }
    
    /* Special-case horizontal, vertical, and diagonal lines, which
    require no weighting because they go right through the center of
    every pixel */
    int DeltaY = Y1 - Y0;
    if (DeltaY == 0)
    {
        /* Horizontal line */
        while (DeltaX-- != 0)
        {
            X0 += XDir;
            l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH, clrLine );
        }
        return;
    }
    if (DeltaX == 0)
    {
        /* Vertical line */
        do
        {
            Y0++;
            l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH, clrLine );
        } while (--DeltaY != 0);
        return;
    }
    
    if (DeltaX == DeltaY)
    {
        /* Diagonal line */
        do
        {
            X0 += XDir;
            Y0++;
            l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH, clrLine );
        } while (--DeltaY != 0);
        return;
    }
    
    unsigned short ErrorAdj;
    unsigned short ErrorAccTemp, Weighting;
    
    /* Line is not horizontal, diagonal, or vertical */
    unsigned short ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
    
    uint8_t rl = 0;//GetRValue( clrLine );
    uint8_t gl = 0;//GetGValue( clrLine );
    uint8_t bl = 0;//GetBValue( clrLine );
    double grayl = rl * 0.299 + gl * 0.587 + bl * 0.114;
    
    /* Is this an X-major or Y-major line? */
    if (DeltaY > DeltaX)
    {
    /* Y-major line; calculate 16-bit fixed-point fractional part of a
    pixel that X advances each time Y advances 1 pixel, truncating the
        result so that we won't overrun the endpoint along the X axis */
        ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
        /* Draw all pixels other than the first and last */
        while (--DeltaY) {
            ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
            ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
            if (ErrorAcc <= ErrorAccTemp) {
                /* The error accumulator turned over, so advance the X coord */
                X0 += XDir;
            }
            Y0++; /* Y-major, so always advance Y */
                  /* The IntensityBits most significant bits of ErrorAcc give us the
                  intensity weighting for this pixel, and the complement of the
            weighting for the paired pixel */
            Weighting = ErrorAcc >> 8;
            // ASSERT( Weighting < 256 );
            // ASSERT( ( Weighting ^ 255 ) < 256 );
            
            // COLORREF clrBackGround = ::GetPixel( pDC->m_hDC, X0, Y0 );
            uint8_t rb = 0;//GetRValue( clrBackGround );
            uint8_t gb = 0;//GetGValue( clrBackGround );
            uint8_t bb = 0;//GetBValue( clrBackGround );
            double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
            
            uint8_t rr = ( rb > rl ? ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rl - rb ) + rb ) ) );
            uint8_t gr = ( gb > gl ? ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gl - gb ) + gb ) ) );
            uint8_t br = ( bb > bl ? ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bl - bb ) + bb ) ) );
            l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH, lui_color_rgb( rr, gr, br ) );
            
            // clrBackGround = ::GetPixel( pDC->m_hDC, X0 + XDir, Y0 );
            rb = 0;//GetRValue( clrBackGround );
            gb = 0;//GetGValue( clrBackGround );
            bb = 0;//GetBValue( clrBackGround );
            grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
            
            rr = ( rb > rl ? ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rl - rb ) + rb ) ) );
            gr = ( gb > gl ? ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gl - gb ) + gb ) ) );
            br = ( bb > bl ? ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bl - bb ) + bb ) ) );
            l_point( X0 + XDir, Y0, LCD_WIDTH, LCD_LENGTH,lui_color_rgb( rr, gr, br ) );
        }
        /* Draw the final pixel, which is always exactly intersected by the line
        and so needs no weighting */
        l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH,  clrLine );
        return;
    }
    /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
    pixel that Y advances each time X advances 1 pixel, truncating the
    result to avoid overrunning the endpoint along the X axis */
    ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
    /* Draw all pixels other than the first and last */
    while (--DeltaX) {
        ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
        ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
        if (ErrorAcc <= ErrorAccTemp) {
            /* The error accumulator turned over, so advance the Y coord */
            Y0++;
        }
        X0 += XDir; /* X-major, so always advance X */
                    /* The IntensityBits most significant bits of ErrorAcc give us the
                    intensity weighting for this pixel, and the complement of the
        weighting for the paired pixel */
        Weighting = ErrorAcc >> 8;
        // ASSERT( Weighting < 256 );
        // ASSERT( ( Weighting ^ 255 ) < 256 );
        
        // COLORREF clrBackGround = ::GetPixel( pDC->m_hDC, X0, Y0 );
        uint8_t rb = 0;//GetRValue( clrBackGround );
        uint8_t gb = 0;//GetGValue( clrBackGround );
        uint8_t bb = 0;//GetBValue( clrBackGround );
        double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
        
        uint8_t rr = ( rb > rl ? ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rl - rb ) + rb ) ) );
        uint8_t gr = ( gb > gl ? ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gl - gb ) + gb ) ) );
        uint8_t br = ( bb > bl ? ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bl - bb ) + bb ) ) );
        
        l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH, lui_color_rgb( rr, gr, br ) );
        
        // clrBackGround = ::GetPixel( pDC->m_hDC, X0, Y0 + 1 );
        rb = 0;//GetRValue( clrBackGround );
        gb = 0;//GetGValue( clrBackGround );
        bb = 0;//GetBValue( clrBackGround );
        grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;
        
        rr = ( rb > rl ? ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rl - rb ) + rb ) ) );
        gr = ( gb > gl ? ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gl - gb ) + gb ) ) );
        br = ( bb > bl ? ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( uint8_t )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bl - bb ) + bb ) ) );
        
        l_point( X0, Y0 + 1, LCD_WIDTH, LCD_LENGTH, lui_color_rgb( rr, gr, br ) );
    }
    
    /* Draw the final pixel, which is always exactly intersected by the line
    and so needs no weighting */
    l_point(X0, Y0, LCD_WIDTH, LCD_LENGTH, clrLine );
}

void swap(int *a,int *b)
{
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}

int ipart(float x) {
	return (int)x;
}

float fpart(float x) {
	return x - floor(x);
}

float rfpart(float x) {
	return 1 - fpart(x);
}

void xxwu(int x0, int y0, int x1, int y1, uint16_t c) {
	int steep = abs(y1 - y0) > abs(x1 - x0) ;

	// swap the co-ordinates if slope > 1 or we
	// draw backwards
	if (steep) {
		swap(&x0 , &y0);
		swap(&x1 , &y1);
	}

	if (x0 > x1) {
		swap(&x0 ,&x1);
		swap(&y0 ,&y1);
	}

	//compute the slope
	float dx = x1-x0;
	float dy = y1-y0;
	float gradient = dy/dx;
	if (dx == 0.0) {
		gradient = 1;
	}

	int xpxl1 = x0;
	int xpxl2 = x1;
	float intersectY = y0;

	// main loop
	if (steep) {
		int x;
		for (x = xpxl1 ; x <=xpxl2 ; x++) {
			// pixel coverage is determined by fractional
			// part of y co-ordinate
			// img->set(ipart(intersectY), x,
			// 		c * rfpart(intersectY));
			// img->set(ipart(intersectY)-1, x,
			// 		c * fpart(intersectY));
            l_point(ipart(intersectY), x, LCD_WIDTH, LCD_LENGTH, c * rfpart(intersectY) );
            l_point(ipart(intersectY)-1, x, LCD_WIDTH, LCD_LENGTH, c * fpart(intersectY) );
            intersectY += gradient;
            // printf("%d-",x);
		}
	} else {
		int x;
		for (x = xpxl1 ; x <=xpxl2 ; x++) {
			// pixel coverage is determined by fractional
			// part of y co-ordinate
			// img->set(x, ipart(intersectY),
			// 		c * rfpart(intersectY));
			// img->set(x, ipart(intersectY)-1,
			// 		c * fpart(intersectY));
			intersectY += gradient;
		}
	}
}

void DrawWuCirlce (int x, int y, int r)
{
	short x1, y1, x2, y2;
	float dt = 5;

	for ( float theta= 0; theta< 360; theta += dt )
	{
		x1 = (short)( r*cos(theta*3.141593/180.0)+x);
		y1 = (short)(-r*sin(theta*3.141593/180.0)+y);

		x2 = (short)( r*cos((theta+dt)*3.141593/180.0)+x);
		y2 = (short)(-r*sin((theta+dt)*3.141593/180.0)+y);
		
		DrawWuLine2(x1, y1, x2, y2, 0);
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

            uint16_t aaa = 0;
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
                if(aaa > 0) {
                    ptr += aaa;
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

            uint16_t bbb = 0;
            uint32_t ptr = 0;
            if(f_layout.point.x > x) {
                ptr += (f_layout.point.x-x);
                bbb = ptr;
                x = f_layout.point.x;
            }

            if(f_layout.point.y > y) {
                ptr += (f_layout.point.y-y)*width;
                y = f_layout.point.y;
            }

            uint16_t aaa = 0;
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
            ptr *= 2;
            for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
                for(int x_j = x1; x_j < x2; x_j++) {
                    cache.array[y_i+x_j] = (uint16_t)( material[ptr+1]<<8)+material[ptr];
                    ptr += 2;
                }
                ptr += bbb*2;
                if((x+width) > maxX2) {
                    ptr += ( (x+width) - maxX2) * 2;
                }
                if(x < cache.coordinate.point.x) {
                    ptr += (cache.coordinate.point.x-x) * 2;
                }
                if(aaa > 0) {
                    ptr += aaa*2;
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

            uint16_t bbb = 0;
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

            uint16_t aaa = 0;
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
                    if(material[ptr+2] != 0) {
                    uint16_t color = (uint16_t)( material[ptr+1]<<8)+material[ptr];
                    cache.array[y_i+x_j] = lui_alpha_blend(cache.array[y_i+x_j],color, 0xff-material[ptr+2]);
                    }
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

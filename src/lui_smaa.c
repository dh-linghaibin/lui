/*
* This file is part of the
*
* Copyright (c) 2016-2018 linghaibin
*
*/
#include "lui_smaa.h"

static uint8_t   abAABuffer[20];   /* This could be changed to dynamic memory ... */
static uint8_t*  pabAABuffer;
static int  _x0, _x1, _y, _x0_InUse, _x1_InUse;

static void _CleanLine(void) {
  _y = -16383;  /* Invalidate */  
  _x0_InUse =  16383;
  _x1_InUse = -16383;
}

static void _FlushLine(void) {
  int i;
  int iEnd = _x1_InUse-_x0;
  int IMax = GUI_Context.AA_Factor * GUI_Context.AA_Factor;
  for (i =_x0_InUse-_x0; i<=iEnd; i++) {
    int Intens = *(pabAABuffer+i);
    if (Intens) {
      /* Check we can use line draw */
      if (Intens == IMax) {
        int j;
        for (j=i; j<iEnd; ) {
          if (*(pabAABuffer+j+1) != IMax) {
            break;
          }
          j++;
        }
        /* Draw the full pixel(s) */
        if (j!=i) {
          pLCD_HLPrev->pfDrawHLine(_x0+i, _y, _x0+j);
          i = j;  /*xxx*/
        } else {
          LCD_HL_DrawPixel (_x0+i,_y);
        }
      } else {
        LCD_SetPixelAA(_x0+i,_y, (15*Intens+IMax/2)/IMax);
      }
    }    
  }
  _CleanLine();
}

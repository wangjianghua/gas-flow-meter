/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI.c
Purpose     : Empty driver
              This driver does no perform any function, but it can be
              used for 2 purposes:
              a) Satisfy all externals so an application can be
                compiled and linked in target hardware even if the
                driver is not already available
              b) Template for a starting point for a new driver.
----------------------------------------------------------------------   
Adapting to a new system (creating a new driver):
  In this case, the first step is to fill the routines 
  LCD_L0_GetPixelIndex, LCD_L0_SetPixelIndex and LCD_L0_Init with
  functionality, which is sufficient to make the hardware work.
  A second (optional) step would be to optimize higher level routines. 
----------------------------------------------------------------------   
Version-Date---Author-Explanation                                        
----------------------------------------------------------------------   
x.xx.xx 140710 华兄   a) 创建于COG12864平台，邮箱: 591881218@qq.com
1.00.00 020417 JE     a) Changed to have only to adapt _GetPixelIndex
                         and _SetPixelIndex
0.90.00 020214 JE     a) First release
---------------------------END-OF-HEADER------------------------------
*/

#include "includes.h"


GUI_EXTERN GUI_SADDR GUI_CONTEXT        GUI_Context;        /* Thread wide globals */

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#ifndef LCD_INIT_CONTROLLER
  #define LCD_INIT_CONTROLLER()         lcd_init()
#endif

/*********************************************************************
*
*       Macros for MIRROR_, SWAP_ and LUT_
*/
#if (!defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
  #if   (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) y
  #elif (!LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) x
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) y
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) x
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
  #endif
#else
  #if   ( defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #elif (!defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) y
  #elif ( defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #endif
#endif

/*********************************************************************
*
*       Exported functions
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_L0_SetPixelIndex
*
* Purpose:
*   Sets the index of the given pixel. The upper layers
*   calling this routine make sure that the coordinates are in range, so
*   that no check on the parameters needs to be performed.
*/
void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) {
  /* Convert logical into physical coordinates (Dep. on LCDConf.h) */
  #if LCD_SWAP_XY | LCD_MIRROR_X| LCD_MIRROR_Y
    int xPhys = LOG2PHYS_X(x, y);
    int yPhys = LOG2PHYS_Y(x, y);
  #else
    #define xPhys x
    #define yPhys y
  #endif
  /* Write into hardware ... Adapt to your system */
  if((yPhys >= LCD_YSIZE) || (xPhys >= LCD_XSIZE))
  {

  }
  else
  {
    if(PixelIndex)
    {
      LCD_DISP_RAM[yPhys >> 3][xPhys] |= 0x01 << (yPhys & 0x07);   
    }
    else  
    {
      LCD_DISP_RAM[yPhys >> 3][xPhys] &= ~(0x01 << (yPhys & 0x07));
    }
  }  
}

/*********************************************************************
*
*       LCD_L0_GetPixelIndex
*
* Purpose:
*   Returns the index of the given pixel. The upper layers
*   calling this routine make sure that the coordinates are in range, so
*   that no check on the parameters needs to be performed.
*/
unsigned int LCD_L0_GetPixelIndex(int x, int y) {
  LCD_PIXELINDEX PixelIndex;
  /* Convert logical into physical coordinates (Dep. on LCDConf.h) */
  #if LCD_SWAP_XY | LCD_MIRROR_X| LCD_MIRROR_Y
    int xPhys = LOG2PHYS_X(x, y);
    int yPhys = LOG2PHYS_Y(x, y);
  #else
    #define xPhys x
    #define yPhys y
  #endif
  /* Read from hardware ... Adapt to your system */
  if(LCD_DISP_RAM[yPhys >> 3][xPhys] & (0x01 << (yPhys & 0x07)))
  {
    PixelIndex = 1;
  }
  else
  {
    PixelIndex = 0;
  }

  return PixelIndex;
}

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/

/*********************************************************************
*
*       Draw Bitmap 1 BPP
*/
static void  _DrawBitLine1BPP(int x, int y, U8 const GUI_UNI_PTR *p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX Index0 = *(pTrans+0);
  LCD_PIXELINDEX Index1 = *(pTrans+1);
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      do {
        LCD_L0_SetPixelIndex(x++, y, (*p & (0x80 >> Diff)) ? Index1 : Index0);
			  if (++Diff == 8) {
          Diff = 0;
				  p++;
			  }
		  } while (--xsize);
      break;
    case LCD_DRAWMODE_TRANS:
      do {
  		  if (*p & (0x80 >> Diff))
          LCD_L0_SetPixelIndex(x, y, Index1);
        x++;
			  if (++Diff == 8) {
          Diff = 0;
				  p++;
			  }
		  } while (--xsize);
      break;
    case LCD_DRAWMODE_XOR:;
      do {
  		  if (*p & (0x80 >> Diff)) {
          int Pixel = LCD_L0_GetPixelIndex(x, y);
          LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
        }
        x++;
			  if (++Diff == 8) {
          Diff = 0;
				  p++;
			  }
		  } while (--xsize);
      break;
	}
}

/*********************************************************************
*
*       LCD_L0_XorPixel
*/
void LCD_L0_XorPixel(int x, int y) {
  LCD_PIXELINDEX PixelIndex = LCD_L0_GetPixelIndex(x, y);
  LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - PixelIndex - 1);
}

/*********************************************************************
*
*       LCD_L0_DrawHLine
*/
void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; x0 <= x1; x0++) {
      LCD_L0_XorPixel(x0, y);
    }
  } else {
    for (; x0 <= x1; x0++) {
      LCD_L0_SetPixelIndex(x0, y, LCD_COLORINDEX);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawVLine
*/
void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (; y0 <= y1; y0++) {
      LCD_L0_XorPixel(x, y0);
    }
  } else {
    for (; y0 <= y1; y0++) {
      LCD_L0_SetPixelIndex(x, y0, LCD_COLORINDEX);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_FillRect
*/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
  for (; y0 <= y1; y0++) {
    LCD_L0_DrawHLine(x0, y0, x1);
  }
}

/*********************************************************************
*
*       LCD_L0_DrawBitmap
*/
void LCD_L0_DrawBitmap(int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
  int i;
  /* Use _DrawBitLineXBPP */
  for (i=0; i<ysize; i++) {
    switch (BitsPerPixel) {
    case 1:
      _DrawBitLine1BPP(x0, i + y0, pData, Diff, xsize, pTrans);
      break;
    }
    pData += BytesPerLine;
  }
}

/*********************************************************************
*
*       LCD_On / LCD_Off
*/
void LCD_On (void) {
#ifdef LCD_ON
  LCD_ON();
#endif
}

void LCD_Off (void) {
#ifdef LCD_OFF
  LCD_OFF();
#endif
}

/*********************************************************************
*
*       LCD_L0_Init
*
* Purpose:
*   Initialises the LCD-controller.
*/
int LCD_L0_Init(void) {
  memset(LCD_DISP_RAM, 0, LCD_DISP_RAM_SIZE);

  LCD_INIT_CONTROLLER();
  return 0;
}

/*********************************************************************
*
*       GUI_GetFontDistY
*/
int GUI_GetFontDistY(void) {
  int r;
  GUI_LOCK();
  r = GUI_Context.pAFont->YDist * GUI_Context.pAFont->YMag;
  GUI_UNLOCK();
  return r;
}

/*********************************************************************
*
*       GUI_GetCharDistX
*/
int GUI_GetCharDistX(U16 c) {
  int r;
  GUI_LOCK();
  r = GUI_Context.pAFont->pfGetCharDistX(c);
  GUI_UNLOCK();
  return r;
}

/*********************************************************************
*
*       GUI_UC__GetCharCodeInc
*/
U16 GUI_UC__GetCharCodeInc(const char GUI_UNI_PTR ** ps) {
  const char GUI_UNI_PTR * s;
  U16 r;
  s   = *ps;
  r   = GUI_UC__GetCharCode(s);
  s  += GUI_UC__GetCharSize(s);
  *ps = s;
  return r;
}

/*********************************************************************
*
*       GUI__GetLineNumChars
*/
int GUI__GetLineNumChars(const char GUI_UNI_PTR *s, int MaxNumChars) {
  int NumChars = 0;
  if (s) {
    for (; NumChars < MaxNumChars; NumChars++) {
      U16 Data = GUI_UC__GetCharCodeInc(&s);
      if ((Data == 0) || (Data == '\n')) {
        break;
      }
    }
  }
  return NumChars;
}

/*********************************************************************
*
*       GUI_GetLineDistX
*
*  This routine is used to calculate the length of a line in pixels.
*/
int GUI__GetLineDistX(const char GUI_UNI_PTR *s, int MaxNumChars) {
  int Dist = 0;
  if (s) {
    U16 Char;
    
    while (--MaxNumChars >= 0) {
      Char  = GUI_UC__GetCharCodeInc(&s);
      Dist += GUI_GetCharDistX(Char);
    }
  }
  return Dist;
}

/*********************************************************************
*
*       GUI_GetYAdjust
*
* Returns adjustment in vertical (Y) direction
*
* Note: The return value needs to be subtracted from
*       the y-position of the character.
*/
int GUI_GetYAdjust(void) {
  int r = 0;
  GUI_LOCK();
  switch (GUI_Context.TextAlign & GUI_TA_VERTICAL) {
	case GUI_TA_BOTTOM:
		r = GUI_Context.pAFont->YSize - 1;
    break;
	case GUI_TA_VCENTER:
		r = GUI_Context.pAFont->YSize / 2;
    break;
	case GUI_TA_BASELINE:
		r = GUI_Context.pAFont->YSize / 2;
	}
  GUI_UNLOCK();
  return r;
}

/*********************************************************************
*
*       GUI_RectsIntersect
*
* Purpose:
*   Check if rectangle do intersect.
*
* Return value:
*   0 if they do not.
*   1 if they do.
*/
int GUI_RectsIntersect(const GUI_RECT* pr0, const GUI_RECT* pr1) {
  if (pr0->y0 <= pr1->y1) {
    if (pr1->y0 <= pr0->y1) {
      if (pr0->x0 <= pr1->x1) {
        if (pr1->x0 <= pr0->x1) {
          return 1;
        }
      }
    }
  }
  return 0;
}

/*********************************************************************
*
*       GUI_DispNextLine
*/
void GUI_DispNextLine(void) {
  GUI_LOCK();
  GUI_Context.DispPosY += GUI_GetFontDistY();
  GUI_Context.DispPosX  = GUI_Context.LBorder;
  GUI_UNLOCK();
}

/*********************************************************************
*
*       GL_DispChar
*/
void GL_DispChar(U16 c) {
  /* check for control characters */
  if (c == '\n') {
    GUI_DispNextLine();
  } else {
    if (c != '\r') {
      GUI_LOCK();
      GUI_Context.pAFont->pfDispChar(c);
      GUI_UNLOCK();
    }
  }
}

/*********************************************************************
*
*       _DispLine
*/
static void _DispLine(const char GUI_UNI_PTR *s, int Len, const GUI_RECT* pr) {
  /* Check if we have anything to do at all ... */
  if (GUI_Context.pClipRect_HL) {
    if (GUI_RectsIntersect(GUI_Context.pClipRect_HL, pr) == 0)
      return;
  }
  
  U16 Char;
    
  while (--Len >= 0) {
    Char = GUI_UC__GetCharCodeInc(&s);
    GUI_Context.pAFont->pfDispChar(Char);
  }
}

/*********************************************************************
*
*       GUI__DispLine
*/
void GUI__DispLine(const char GUI_UNI_PTR *s, int MaxNumChars, const GUI_RECT* pr) {
  GUI_RECT r;
  
  r = *pr;
   
  GUI_Context.DispPosX = r.x0;
  GUI_Context.DispPosY = r.y0;
  
  /* Do the actual drawing via routine call. */
  _DispLine(s, MaxNumChars, &r);
}

/*********************************************************************
*
*       GUI_UC__NumChars2NumBytes
*/
int GUI_UC__NumChars2NumBytes(const char GUI_UNI_PTR * s, int NumChars) {
  int CharSize, NumBytes = 0;
  while (NumChars--) {
    CharSize = GUI_UC__GetCharSize(s);
    s += CharSize;    
    NumBytes += CharSize;
  }
  return NumBytes;
}

/*********************************************************************
*
*       GUI_DispString
*/
void GUI_DispString(const char GUI_UNI_PTR *s) {
  int xAdjust, yAdjust, xOrg;
  int FontSizeY;
  if (!s)
    return;
  GUI_LOCK();
  FontSizeY = GUI_GetFontDistY();
  xOrg = GUI_Context.DispPosX;
 /* Adjust vertical position */
  yAdjust = GUI_GetYAdjust();
  GUI_Context.DispPosY -= yAdjust;
  for (; *s; s++) {
    GUI_RECT r;
    int LineNumChars = GUI__GetLineNumChars(s, 0x7fff);
    int xLineSize    = GUI__GetLineDistX(s, LineNumChars);
  /* Check if x-position needs to be changed due to h-alignment */
    switch (GUI_Context.TextAlign & GUI_TA_HORIZONTAL) { 
      case GUI_TA_CENTER: xAdjust = xLineSize / 2; break;
      case GUI_TA_RIGHT:  xAdjust = xLineSize; break;
      default:            xAdjust = 0;
    }
    r.x0 = GUI_Context.DispPosX -= xAdjust;
    r.x1 = r.x0 + xLineSize - 1;    
    r.y0 = GUI_Context.DispPosY;
    r.y1 = r.y0 + FontSizeY - 1;    
    GUI__DispLine(s, LineNumChars, &r);
    GUI_Context.DispPosY = r.y0;
    s += GUI_UC__NumChars2NumBytes(s, LineNumChars);
    if ((*s == '\n') || (*s == '\r')) {
      switch (GUI_Context.TextAlign & GUI_TA_HORIZONTAL) { 
      case GUI_TA_CENTER:
      case GUI_TA_RIGHT:
        GUI_Context.DispPosX = xOrg;
        break;
      default:
        GUI_Context.DispPosX = GUI_Context.LBorder;
        break;
      }
      if (*s == '\n')
        GUI_Context.DispPosY += FontSizeY;
    } else {
      GUI_Context.DispPosX = r.x0 + xLineSize;
    }
    if (*s == 0)    /* end of string (last line) reached ? */
      break;
  }
  GUI_Context.DispPosY += yAdjust;
  GUI_Context.TextAlign &= ~GUI_TA_HORIZONTAL;
  GUI_UNLOCK();
}

/*********************************************************************
*
*       GUI_DispStringAt
*/
void GUI_DispStringAt(const char GUI_UNI_PTR *s, int x, int y) {
  GUI_LOCK();
  GUI_Context.DispPosX = x;
  GUI_Context.DispPosY = y;
  GUI_DispString(s);
  GUI_UNLOCK();
}

/*********************************************************************
*
*       GUI_DispRevStringAt
*/
void GUI_DispRevStringAt(const char GUI_UNI_PTR *s, int x, int y) {
  GUI_LOCK();
  GUI_Context.DispPosX = x;
  GUI_Context.DispPosY = y;
  GUI_Context.DrawMode = LCD_DRAWMODE_REV;
  GUI_DispString(s);
  GUI_Context.DrawMode = LCD_DRAWMODE_NORMAL;
  GUI_UNLOCK();
}

/*********************************************************************
*
*       GUIPROP_FindChar
*/
static const GUI_FONT_PROP* GUIPROP_FindChar(const GUI_FONT_PROP* pProp, U16P c) {
  for (; pProp; pProp=(const GUI_FONT_PROP*) pProp->pNext) {
    if ((c>=pProp->First) && (c<=pProp->Last))
      break;
  }
  return pProp;
}

/*********************************************************************
*
*       LCD_SetDrawMode
*/
LCD_DRAWMODE LCD_SetDrawMode(LCD_DRAWMODE dm) {
  LCD_DRAWMODE OldDM = GUI_Context.DrawMode;
  if ((GUI_Context.DrawMode^dm) & LCD_DRAWMODE_REV) {
    LCD_PIXELINDEX temp = LCD_BKCOLORINDEX;
    LCD_BKCOLORINDEX    = LCD_COLORINDEX;
    LCD_COLORINDEX = temp;
  }
  GUI_Context.DrawMode = dm;
  return OldDM;
}

/*********************************************************************
*
*       LCD_FillRect
*/
void LCD_FillRect(int x0, int y0, int x1, int y1) {
  /* Perform clipping and check if there is something to do */
  CLIP_X();
  if (x1<x0)
    return;
  CLIP_Y();
  if (y1<y0)
    return;
  /* Call driver to draw */
  LCD_L0_FillRect(x0,y0,x1,y1);
}

/*********************************************************************
*
*       LCD_DrawBitmap
*/
void LCD_DrawBitmap(int x0, int y0, int xsize, int ysize, int xMul, int yMul,
                       int BitsPerPixel, int BytesPerLine,
                       const U8 GUI_UNI_PTR * pPixel, const LCD_PIXELINDEX* pTrans)
{
  U8  Data = 0;
  int x1, y1;
  /* Handle rotation if necessary */
  #if GUI_SUPPORT_ROTATION
  if (GUI_pLCD_APIList) {
    GUI_pLCD_APIList->pfDrawBitmap(x0, y0, xsize, ysize, xMul, yMul, BitsPerPixel, BytesPerLine, pPixel, pTrans);
    return;
  }
  #endif
  /* Handle the optional Y-magnification */
  y1 = y0 + ysize - 1;
  x1 = x0 + xsize - 1;
/*  Handle BITMAP without magnification */
  if ((xMul | yMul) == 1) {
    int Diff;
    /*  Clip y0 (top) */
    Diff = GUI_Context.ClipRect.y0 - y0;
    if (Diff > 0) {
      ysize -= Diff;
      if (ysize <= 0) {
		    return;
      }
      y0     = GUI_Context.ClipRect.y0;
      #if GUI_SUPPORT_LARGE_BITMAPS                       /* Required only for 16 bit CPUs if some bitmaps are >64kByte */
        pPixel += (U32)     Diff * (U32)     BytesPerLine;
      #else
        pPixel += (unsigned)Diff * (unsigned)BytesPerLine;
      #endif
    }
    /*  Clip y1 (bottom) */
    Diff = y1 - GUI_Context.ClipRect.y1;
    if (Diff > 0) {
      ysize -= Diff;
      if (ysize <= 0) {
		    return;
      }
    }
    /*        Clip right side    */
    Diff = x1 - GUI_Context.ClipRect.x1;
    if (Diff > 0) {
      xsize -= Diff;
    }
    /*        Clip left side ... (The difficult side ...)    */
    Diff = 0;
    if (x0 < GUI_Context.ClipRect.x0) {
      Diff = GUI_Context.ClipRect.x0 - x0;
			xsize -= Diff;
			switch (BitsPerPixel) {
			case 1:
  			pPixel+= (Diff>>3); x0 += (Diff>>3)<<3; Diff &=7;
				break;
			case 2:
	  		pPixel+= (Diff>>2); x0 += (Diff>>2)<<2; Diff &=3;
				break;
			case 4:
				pPixel+= (Diff>>1); x0 += (Diff>>1)<<1; Diff &=1;
				break;
			case 8:
				pPixel+= Diff;      x0 += Diff; Diff=0;
				break;
			case 16:
				pPixel+= (Diff<<1); x0 += Diff; Diff=0;
				break;
			}
    }
    if (xsize <=0) {
		  return;
    }
    LCD_L0_DrawBitmap(x0, y0, xsize, ysize, BitsPerPixel, BytesPerLine, pPixel, Diff, pTrans);
  } else {
    /**** Handle BITMAP with magnification ***/
    int x,y;
    int yi;
    int Shift = 8-BitsPerPixel;
    for (y=y0, yi=0; yi<ysize; yi++, y+= yMul, pPixel+=BytesPerLine) {
      int yMax = y+yMul-1;
      /* Draw if within clip area (Optimization ... "if" is not required !) */
      if ((yMax >= GUI_Context.ClipRect.y0) && (y <= GUI_Context.ClipRect.y1)) {
        int BitsLeft =0;
        int xi;
        const U8 GUI_UNI_PTR * pDataLine = pPixel;
        for (x=x0, xi=0; xi<xsize; xi++, x+=xMul) {
          U8  Index;
          if (!BitsLeft) {
            Data = *pDataLine++;
            BitsLeft =8;
          }
          Index = Data>>Shift;
          Data    <<= BitsPerPixel;
          BitsLeft -= BitsPerPixel;
          if (Index || ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) ==0)) {
            LCD_PIXELINDEX  OldColor = LCD_COLORINDEX;
            if (pTrans) {
              LCD_COLORINDEX = *(pTrans+Index);
            } else {
              LCD_COLORINDEX = Index;
            }
            LCD_FillRect(x,y, x+xMul-1, yMax);
            LCD_COLORINDEX = OldColor;
          }
        }
      }
    }
  }
}

/*********************************************************************
*
*       LCD_Color2Index_1
*/
unsigned LCD_Color2Index_1(LCD_COLOR Color) {
  int r,g,b;
  r = Color      &255;
  g = (Color>>8) &255;
  b = Color>>16;
  return (r+g+b+383) /(3*255);
}

/*********************************************************************
*
*       LCD_Index2Color_1
*/
LCD_COLOR LCD_Index2Color_1(int Index) {
  return Index ? 0xFFFFFF : 0;
}

/*********************************************************************
*
*       LCD_GetColorIndex
*/
int LCD_GetColorIndex(void) {
  return LCD_COLORINDEX;
}

/*********************************************************************
*
*       LCD_L0_Index2Color
*/
LCD_COLOR LCD_L0_Index2Color(int Index) {
  #if LCD_REVERSE
    Index ^= ((1<<LCD_BITSPERPIXEL)-1);
  #endif
  return LCD_Index2Color_1(Index);
}

/*********************************************************************
*
*       LCD_Index2Color
*/
LCD_COLOR LCD_Index2Color(int Index) {
  return LCD_L0_Index2Color(Index);
}

/*********************************************************************
*
*       _GetColorIndex
*/
static int _GetColorIndex(int i)  /* i is 0 or 1 */ {
  return  (GUI_Context.DrawMode & LCD_DRAWMODE_REV) ? i-1 : i;
}

/*********************************************************************
*
*       LCD_SetColorIndex
*/
void LCD_SetColorIndex(int Index) {
  LCD_ACOLORINDEX[_GetColorIndex(1)] = Index;
}

/*********************************************************************
*
*           LCD_L0_Color2Index
*/
unsigned int LCD_L0_Color2Index(LCD_COLOR Color) {
  unsigned int Index;
  Index = LCD_Color2Index_1(Color);
  #if LCD_REVERSE
    Index ^= ((1<<LCD_BITSPERPIXEL)-1);
  #endif
  return Index;
}

/*********************************************************************
*
*       LCD_Color2Index
*/
int LCD_Color2Index(LCD_COLOR Color) {
  return LCD_L0_Color2Index(Color);
}

/*********************************************************************
*
*       LCD_SetColor
*/
void LCD_SetColor(GUI_COLOR color) {
  if (GUI_Context.Color != color) {
    GUI_Context.Color = color;
    LCD_SetColorIndex(LCD_Color2Index(color));
  }
}

/*********************************************************************
*
*       GUI_GetColor
*/
GUI_COLOR GUI_GetColor(void) {
  GUI_COLOR r;
  GUI_LOCK();
  r = LCD_Index2Color(LCD_GetColorIndex());
  GUI_UNLOCK();
  return r;
}

/*********************************************************************
*
*       GUI_SetColor
*/
void GUI_SetColor(GUI_COLOR color) {
  GUI_LOCK(); {
    LCD_SetColor(color);
  } GUI_UNLOCK();
}

/*********************************************************************
*
*       LCD_GetBkColorIndex
*/
int LCD_GetBkColorIndex(void) {
  return LCD_BKCOLORINDEX;
}

/*********************************************************************
*
*       GUI_GetBkColor
*/
GUI_COLOR GUI_GetBkColor(void) {
  GUI_COLOR r;
  GUI_LOCK();
  r = LCD_Index2Color(LCD_GetBkColorIndex());
  GUI_UNLOCK();
  return r;
}

/*********************************************************************
*
*       GUIPROP_DispChar
*
* Purpose:
*   This is the routine that displays a character. It is used by all
*   other routines which display characters as a subroutine.
*/
void GUIPROP_DispChar(U16P c) {
  int BytesPerLine;
  GUI_DRAWMODE DrawMode = GUI_Context.TextMode;
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  if (pProp) {
    GUI_DRAWMODE OldDrawMode;
    const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo+(c-pProp->First);
    BytesPerLine = pCharInfo->BytesPerLine;
    OldDrawMode  = LCD_SetDrawMode(DrawMode);
    LCD_DrawBitmap( GUI_Context.DispPosX, GUI_Context.DispPosY,
                       pCharInfo->XSize,
											 GUI_Context.pAFont->YSize,
                       GUI_Context.pAFont->XMag,
											 GUI_Context.pAFont->YMag,
                       1,     /* Bits per Pixel */
                       BytesPerLine,
                       pCharInfo->pData,
                       &LCD_BKCOLORINDEX
                       );
    /* Fill empty pixel lines */
    if (GUI_Context.pAFont->YDist > GUI_Context.pAFont->YSize) {
      int YMag = GUI_Context.pAFont->YMag;
      int YDist = GUI_Context.pAFont->YDist * YMag;
      int YSize = GUI_Context.pAFont->YSize * YMag;
      if (DrawMode != LCD_DRAWMODE_TRANS) {
        LCD_COLOR OldColor = GUI_GetColor();
        GUI_SetColor(GUI_GetBkColor());
        LCD_FillRect(GUI_Context.DispPosX, 
                     GUI_Context.DispPosY + YSize, 
                     GUI_Context.DispPosX + pCharInfo->XSize, 
                     GUI_Context.DispPosY + YDist);
        GUI_SetColor(OldColor);
      }
    }
    LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
    GUI_Context.DispPosX += pCharInfo->XDist * GUI_Context.pAFont->XMag;
  }
}

/*********************************************************************
*
*       GUIPROP_GetCharDistX
*/
int GUIPROP_GetCharDistX(U16P c) {
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  return (pProp) ? (pProp->paCharInfo+(c-pProp->First))->XSize * GUI_Context.pAFont->XMag : 0;
}

/*********************************************************************
*
*       GUIPROP_GetFontInfo
*/
void GUIPROP_GetFontInfo(const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO* pfi) {
  pfi->Flags = GUI_FONTINFO_FLAG_PROP;
}

/*********************************************************************
*
*       GUIPROP_IsInFont
*/
char GUIPROP_IsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c) {
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(pFont->p.pProp, c);
  return (pProp==NULL) ? 0 : 1;
}

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetCharCode
*
* Purpose:
*   Return the UNICODE character code of the current character.
*/
static U16 _GetCharCode(const char GUI_UNI_PTR * s) {
  U16 r;
  U8 Char = *(const U8*)s;
  if ((Char & 0x80) == 0) {                /* Single byte (ASCII)  */
    r = Char;
	return r;
  }else{                                   /* Double byte sequence */
    r = Char;
    r <<= 8;
    Char = *(++s);
    r |= Char;
	return r;
  }
}

/*********************************************************************
*
*       _GetCharSize
*
* Purpose:
*   Return the number of bytes of the current character.
*/
static int _GetCharSize(const char GUI_UNI_PTR * s) {
  U8 Char = *s;
  if ((Char & 0x80) == 0) {
    return 1;
  } else{
    return 2;
  }
}

/*********************************************************************
*
*       _CalcSizeOfChar
*
* Purpose:
*   Return the number of bytes needed for the given character.
*/
static int _CalcSizeOfChar(U16 Char) {
  int r;
  if (Char & 0x0080) {         /* Double byte sequence */
    r = 2;
  } else {
    r = 1;
  }
  return r;
}

/*********************************************************************
*
*       _Encode
*
* Purpose:
*   Encode character into 1/2/3 bytes.
*/
static int _Encode(char *s, U16 Char) {
  int r;
  r = _CalcSizeOfChar(Char);
  switch (r) {
  case 1:
    *s = (char)Char;
    break;
  case 2:
    *s++ = Char & 0xff;
    *s   = (Char >> 8) & 0xff;
    break;
  }
  return r;
}

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _API_Table
*/
const GUI_UC_ENC_APILIST GUI__API_TableNone = {
  _GetCharCode,     /*  return character code as U16 */
  _GetCharSize,     /*  return size of character: 1 */
  _CalcSizeOfChar,  /*  return size of character: 1 */
  _Encode           /*  Encode character */
};

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_UC_SetEncodeNone
*/
void GUI_UC_SetEncodeNone(void) {
  GUI_LOCK();
  GUI_Context.pUC_API = &GUI__API_TableNone;
  GUI_UNLOCK();
}

/*********************************************************************
*
*       LCD_SetBkColorIndex
*/
void LCD_SetBkColorIndex(int Index) {
  LCD_ACOLORINDEX[_GetColorIndex(0)] = Index;
}

/*********************************************************************
*
*       LCD_SetBkColor
*/
void LCD_SetBkColor(GUI_COLOR color) {
  if (GUI_Context.BkColor != color) {
    GUI_Context.BkColor = color;
    LCD_SetBkColorIndex(LCD_Color2Index(color));
  }
}

/*********************************************************************
*
*       GUI_SetBkColor
*/
void GUI_SetBkColor(GUI_COLOR color) {
  GUI_LOCK(); {
    LCD_SetBkColor(color);
  } GUI_UNLOCK();
}

/*********************************************************************
*
*       GUI_Init
*/
void GUI_Init(void)
{
  LCD_L0_Init();
  GUI_SetBkColor(GUI_DEFAULT_BKCOLOR);
  GUI_SetColor(GUI_DEFAULT_COLOR);
  GUI_Context.ClipRect.x0 = 0;
  GUI_Context.ClipRect.y0 = 0;
  GUI_Context.ClipRect.x1 = LCD_XSIZE - 1;
  GUI_Context.ClipRect.y1 = LCD_YSIZE - 1;
  GUI_UC_SetEncodeNone();
  GUI_Context.TextAlign = 0;
  GUI_Context.pAFont = &GUI_FontTable;
}


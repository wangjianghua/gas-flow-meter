/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI.h
Purpose     : GUI API include file
----------------------------------------------------------------------   
Version-Date---Author-Explanation                                        
----------------------------------------------------------------------   
x.xx.xx 140710 »ªÐÖ   a) ´´½¨ÓÚCOG12864Æ½Ì¨£¬ÓÊÏä: 591881218@qq.com
---------------------------END-OF-HEADER------------------------------
*/

#ifndef  GUI_H
#define  GUI_H


#define S8    signed char
#define I8    signed char
#define U8    unsigned char     /* unsigned 8  bits. */
#define I16   signed short      /*   signed 16 bits. */
#define U16   unsigned short    /* unsigned 16 bits. */
#define I32   signed long       /*   signed 32 bits. */
#define U32   unsigned long     /* unsigned 32 bits. */
#define I16P  I16               /*   signed 16 bits OR MORE ! */
#define U16P  U16               /* unsigned 16 bits OR MORE ! */

#ifndef NULL
#define NULL ((void *)0)
#endif

/*      *********************************
        *
        *      Standard colors
        *
        *********************************
*/

#define GUI_BLUE          0xFF0000
#define GUI_GREEN         0x00FF00
#define GUI_RED           0x0000FF
#define GUI_CYAN          0xFFFF00
#define GUI_MAGENTA       0xFF00FF
#define GUI_YELLOW        0x00FFFF
#define GUI_LIGHTBLUE     0xFF8080
#define GUI_LIGHTGREEN    0x80FF80
#define GUI_LIGHTRED      0x8080FF
#define GUI_LIGHTCYAN     0xFFFF80
#define GUI_LIGHTMAGENTA  0xFF80FF
#define GUI_LIGHTYELLOW   0x80FFFF
#define GUI_DARKBLUE      0x800000
#define GUI_DARKGREEN     0x008000
#define GUI_DARKRED       0x000080
#define GUI_DARKCYAN      0x808000
#define GUI_DARKMAGENTA   0x800080
#define GUI_DARKYELLOW    0x008080
#define GUI_WHITE         0xFFFFFF
#define GUI_LIGHTGRAY     0xD3D3D3
#define GUI_GRAY          0x808080
#define GUI_DARKGRAY      0x404040
#define GUI_BLACK         0x000000
#define GUI_BROWN         0x2A2AA5

#define GUI_INVALID_COLOR 0xFFFFFFF      /* Invalid color - more than 24 bits */

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE          (128)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE          (64)    /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL   (1)

#define LCD_ON              LCD_BACKLIGHT_ON
#define LCD_OFF             LCD_BACKLIGHT_OFF

#define LCD_DISP_RAM        lcd_disp_buf
#define LCD_DISP_RAM_SIZE   sizeof(lcd_disp_buf)

/* Text alignment flags, horizontal */
#define GUI_TA_HORIZONTAL  (3<<0)
#define GUI_TA_LEFT        (0<<0)
#define GUI_TA_RIGHT	   (1<<0)
#define GUI_TA_CENTER	   (2<<0)
#define GUI_TA_HCENTER	    GUI_TA_CENTER  /* easier to remember :-)  */

/* Text alignment flags, vertical */
#define GUI_TA_VERTICAL    (3<<2)
#define GUI_TA_TOP	       (0<<2)
#define GUI_TA_BOTTOM	   (1<<2)
#define GUI_TA_BASELINE    (2<<2)
#define GUI_TA_VCENTER     (3<<2)

#define GUI_FONTINFO_FLAG_PROP (1<<0)    /* Is proportional */
#define GUI_FONTINFO_FLAG_MONO (1<<1)    /* Is monospaced */
#define GUI_FONTINFO_FLAG_AA   (1<<2)    /* Is an antialiased font */
#define GUI_FONTINFO_FLAG_AA2  (1<<3)    /* Is an antialiased font, 2bpp */
#define GUI_FONTINFO_FLAG_AA4  (1<<4)    /* Is an antialiased font, 4bpp */

#ifndef  GUI_EXTERN
  #define GUI_EXTERN
#endif

#ifndef GUI_SADDR
  #define GUI_SADDR
#endif

#ifndef GUI_DEFAULT_BKCOLOR
  #define GUI_DEFAULT_BKCOLOR GUI_BLACK
#endif

#ifndef GUI_DEFAULT_COLOR
  #define GUI_DEFAULT_COLOR   GUI_WHITE
#endif

/*
      *********************************
      *                               *
      *      Drawing modes            *
      *                               *
      *********************************
*/

#define LCD_DRAWMODE_NORMAL (0)
#define LCD_DRAWMODE_XOR    (1<<0)
#define LCD_DRAWMODE_TRANS  (1<<1)
#define LCD_DRAWMODE_REV    (1<<2)

/*********************************************************************
*
*      Global defaults for all displays
*
**********************************************************************
*/

#define LCD_PIXELINDEX U16

#ifndef LCD_NUM_COLORS
  #define LCD_NUM_COLORS (1L<<LCD_BITSPERPIXEL)
#endif

/**************************************************************
*
*      Defines for constants
*
***************************************************************
*/

#define	________	0x0
#define	_______X	0x1
#define	______X_	0x2
#define	______XX	0x3
#define	_____X__	0x4
#define	_____X_X	0x5
#define	_____XX_	0x6
#define	_____XXX	0x7
#define	____X___	0x8
#define	____X__X	0x9
#define	____X_X_	0xa
#define	____X_XX	0xb
#define	____XX__	0xc
#define	____XX_X	0xd
#define	____XXX_	0xe
#define	____XXXX	0xf
#define	___X____	0x10
#define	___X___X	0x11
#define	___X__X_	0x12
#define	___X__XX	0x13
#define	___X_X__	0x14
#define	___X_X_X	0x15
#define	___X_XX_	0x16
#define	___X_XXX	0x17
#define	___XX___	0x18
#define	___XX__X	0x19
#define	___XX_X_	0x1a
#define	___XX_XX	0x1b
#define	___XXX__	0x1c
#define	___XXX_X	0x1d
#define	___XXXX_	0x1e
#define	___XXXXX	0x1f
#define	__X_____	0x20
#define	__X____X	0x21
#define	__X___X_	0x22
#define	__X___XX	0x23
#define	__X__X__	0x24
#define	__X__X_X	0x25
#define	__X__XX_	0x26
#define	__X__XXX	0x27
#define	__X_X___	0x28
#define	__X_X__X	0x29
#define	__X_X_X_	0x2a
#define	__X_X_XX	0x2b
#define	__X_XX__	0x2c
#define	__X_XX_X	0x2d
#define	__X_XXX_	0x2e
#define	__X_XXXX	0x2f
#define	__XX____	0x30
#define	__XX___X	0x31
#define	__XX__X_	0x32
#define	__XX__XX	0x33
#define	__XX_X__	0x34
#define	__XX_X_X	0x35
#define	__XX_XX_	0x36
#define	__XX_XXX	0x37
#define	__XXX___	0x38
#define	__XXX__X	0x39
#define	__XXX_X_	0x3a
#define	__XXX_XX	0x3b
#define	__XXXX__	0x3c
#define	__XXXX_X	0x3d
#define	__XXXXX_	0x3e
#define	__XXXXXX	0x3f
#define	_X______	0x40
#define	_X_____X	0x41
#define	_X____X_	0x42
#define	_X____XX	0x43
#define	_X___X__	0x44
#define	_X___X_X	0x45
#define	_X___XX_	0x46
#define	_X___XXX	0x47
#define	_X__X___	0x48
#define	_X__X__X	0x49
#define	_X__X_X_	0x4a
#define	_X__X_XX	0x4b
#define	_X__XX__	0x4c
#define	_X__XX_X	0x4d
#define	_X__XXX_	0x4e
#define	_X__XXXX	0x4f
#define	_X_X____	0x50
#define	_X_X___X	0x51
#define	_X_X__X_	0x52
#define	_X_X__XX	0x53
#define	_X_X_X__	0x54
#define	_X_X_X_X	0x55
#define	_X_X_XX_	0x56
#define	_X_X_XXX	0x57
#define	_X_XX___	0x58
#define	_X_XX__X	0x59
#define	_X_XX_X_	0x5a
#define	_X_XX_XX	0x5b
#define	_X_XXX__	0x5c
#define	_X_XXX_X	0x5d
#define	_X_XXXX_	0x5e
#define	_X_XXXXX	0x5f
#define	_XX_____	0x60
#define	_XX____X	0x61
#define	_XX___X_	0x62
#define	_XX___XX	0x63
#define	_XX__X__	0x64
#define	_XX__X_X	0x65
#define	_XX__XX_	0x66
#define	_XX__XXX	0x67
#define	_XX_X___	0x68
#define	_XX_X__X	0x69
#define	_XX_X_X_	0x6a
#define	_XX_X_XX	0x6b
#define	_XX_XX__	0x6c
#define	_XX_XX_X	0x6d
#define	_XX_XXX_	0x6e
#define	_XX_XXXX	0x6f
#define	_XXX____	0x70
#define	_XXX___X	0x71
#define	_XXX__X_	0x72
#define	_XXX__XX	0x73
#define	_XXX_X__	0x74
#define	_XXX_X_X	0x75
#define	_XXX_XX_	0x76
#define	_XXX_XXX	0x77
#define	_XXXX___	0x78
#define	_XXXX__X	0x79
#define	_XXXX_X_	0x7a
#define	_XXXX_XX	0x7b
#define	_XXXXX__	0x7c
#define	_XXXXX_X	0x7d
#define	_XXXXXX_	0x7e
#define	_XXXXXXX	0x7f
#define	X_______	0x80
#define	X______X	0x81
#define	X_____X_	0x82
#define	X_____XX	0x83
#define	X____X__	0x84
#define	X____X_X	0x85
#define	X____XX_	0x86
#define	X____XXX	0x87
#define	X___X___	0x88
#define	X___X__X	0x89
#define	X___X_X_	0x8a
#define	X___X_XX	0x8b
#define	X___XX__	0x8c
#define	X___XX_X	0x8d
#define	X___XXX_	0x8e
#define	X___XXXX	0x8f
#define	X__X____	0x90
#define	X__X___X	0x91
#define	X__X__X_	0x92
#define	X__X__XX	0x93
#define	X__X_X__	0x94
#define	X__X_X_X	0x95
#define	X__X_XX_	0x96
#define	X__X_XXX	0x97
#define	X__XX___	0x98
#define	X__XX__X	0x99
#define	X__XX_X_	0x9a
#define X__XX_XX	0x9b
#define X__XXX__	0x9c
#define X__XXX_X	0x9d
#define	X__XXXX_	0x9e
#define	X__XXXXX	0x9f
#define	X_X_____	0xa0
#define	X_X____X	0xa1
#define	X_X___X_	0xa2
#define	X_X___XX	0xa3
#define	X_X__X__	0xa4
#define	X_X__X_X	0xa5
#define	X_X__XX_	0xa6
#define	X_X__XXX	0xa7
#define	X_X_X___	0xa8
#define	X_X_X__X	0xa9
#define	X_X_X_X_	0xaa
#define	X_X_X_XX	0xab
#define	X_X_XX__	0xac
#define	X_X_XX_X	0xad
#define	X_X_XXX_	0xae
#define	X_X_XXXX	0xaf
#define	X_XX____	0xb0
#define X_XX___X	0xb1
#define	X_XX__X_	0xb2
#define	X_XX__XX	0xb3
#define	X_XX_X__	0xb4
#define	X_XX_X_X	0xb5
#define	X_XX_XX_	0xb6
#define	X_XX_XXX	0xb7
#define	X_XXX___	0xb8
#define	X_XXX__X	0xb9
#define	X_XXX_X_	0xba
#define	X_XXX_XX	0xbb
#define	X_XXXX__	0xbc
#define	X_XXXX_X	0xbd
#define	X_XXXXX_	0xbe
#define	X_XXXXXX	0xbf
#define	XX______	0xc0
#define	XX_____X	0xc1
#define	XX____X_	0xc2
#define	XX____XX	0xc3
#define	XX___X__	0xc4
#define	XX___X_X	0xc5
#define	XX___XX_	0xc6
#define	XX___XXX	0xc7
#define	XX__X___	0xc8
#define	XX__X__X	0xc9
#define	XX__X_X_	0xca
#define	XX__X_XX	0xcb
#define	XX__XX__	0xcc
#define	XX__XX_X	0xcd
#define	XX__XXX_	0xce
#define XX__XXXX	0xcf
#define	XX_X____	0xd0
#define	XX_X___X	0xd1
#define	XX_X__X_	0xd2
#define	XX_X__XX	0xd3
#define	XX_X_X__	0xd4
#define	XX_X_X_X	0xd5
#define	XX_X_XX_	0xd6
#define	XX_X_XXX	0xd7
#define	XX_XX___	0xd8
#define	XX_XX__X	0xd9
#define	XX_XX_X_	0xda
#define	XX_XX_XX	0xdb
#define	XX_XXX__	0xdc
#define	XX_XXX_X	0xdd
#define	XX_XXXX_	0xde
#define	XX_XXXXX	0xdf
#define	XXX_____	0xe0
#define	XXX____X	0xe1
#define	XXX___X_	0xe2
#define	XXX___XX	0xe3
#define	XXX__X__	0xe4
#define	XXX__X_X	0xe5
#define	XXX__XX_	0xe6
#define	XXX__XXX	0xe7
#define	XXX_X___	0xe8
#define	XXX_X__X	0xe9
#define	XXX_X_X_	0xea
#define	XXX_X_XX	0xeb
#define	XXX_XX__	0xec
#define	XXX_XX_X	0xed
#define	XXX_XXX_	0xee
#define	XXX_XXXX	0xef
#define	XXXX____	0xf0
#define	XXXX___X	0xf1
#define	XXXX__X_	0xf2
#define	XXXX__XX	0xf3
#define	XXXX_X__	0xf4
#define	XXXX_X_X	0xf5
#define	XXXX_XX_	0xf6
#define	XXXX_XXX	0xf7
#define	XXXXX___	0xf8
#define	XXXXX__X	0xf9
#define	XXXXX_X_	0xfa
#define	XXXXX_XX	0xfb
#define	XXXXXX__	0xfc
#define	XXXXXX_X	0xfd
#define	XXXXXXX_	0xfe
#define	XXXXXXXX	0xff

/* Define "universal pointer". Normally, this is not needed (define will expand to nothing)
   However, on some systems (AVR - IAR compiler) it can be necessary ( -> __generic),
   since a default pointer can access RAM only, not the built-in Flash
*/
#ifndef GUI_UNI_PTR
  #define GUI_UNI_PTR
  #define GUI_UNI_PTR_USED 0
#else
  #define GUI_UNI_PTR_USED 1
#endif

/*
      ****************************************
      *                                      *
      *      FONT info structure             *
      *                                      *
      ****************************************

This structure is used when retrieving information about a font.
It is designed for future expansion without incompatibilities.
*/

typedef struct {
  U16 Flags;
  U8 Baseline;
  U8 LHeight;     /* height of a small lower case character (a,x) */
  U8 CHeight;     /* height of a small upper case character (A,X) */
} GUI_FONTINFO;

/*
      ****************************************
      *                                      *
      *      FONT methods                    *
      *                                      *
      ****************************************

The parameter to the methods called pFont should be of type
GUI_FONT, but unfortunately a lot of compilers can not handle
forward declarations right ...
So it ends up to be a void pointer.
*/

typedef struct GUI_FONT GUI_FONT;

typedef void GUI_DISPCHAR(U16 c);
typedef int  GUI_GETCHARDISTX(U16P c);
typedef void GUI_GETFONTINFO(const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi);
typedef char GUI_ISINFONT   (const GUI_FONT GUI_UNI_PTR * pFont, U16 c);

#define DECLARE_FONT(Type)                                     \
void GUI##Type##_DispChar    (U16P c);                         \
int  GUI##Type##_GetCharDistX(U16P c);                         \
void GUI##Type##_GetFontInfo (const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi); \
char GUI##Type##_IsInFont    (const GUI_FONT GUI_UNI_PTR * pFont, U16 c)

/* PROP: Proportional fonts */
DECLARE_FONT(PROP);
#define GUI_FONTTYPE_PROP       \
  GUIPROP_DispChar,             \
	GUIPROP_GetCharDistX,         \
	GUIPROP_GetFontInfo,          \
	GUIPROP_IsInFont,             \
  (tGUI_ENC_APIList*)0

/**********************************************************************
*
*                 FONT Encoding
*
***********************************************************************
*/

typedef int  tGUI_GetLineDistX(const char GUI_UNI_PTR *s, int Len);
typedef int  tGUI_GetLineLen(const char GUI_UNI_PTR *s, int MaxLen);
typedef void tGL_DispLine(const char GUI_UNI_PTR *s, int Len);

/*      *************************************************************
        *                                                           *
        *                Typedefs                                   *
        *                                                           *
        *************************************************************
*/

typedef int LCD_DRAWMODE;
typedef U32 LCD_COLOR;

/********************************************************
*
*     Data structures
*
*********************************************************
*/

typedef struct { I16P x,y; } GUI_POINT;
typedef struct { I16 x0,y0,x1,y1; } LCD_RECT;
/*typedef struct { GUI_POINT P0, P1; } LCD_RECT; */

typedef struct {
  int              NumEntries; 
  char             HasTrans;         
  const LCD_COLOR GUI_UNI_PTR * pPalEntries; 
} LCD_LOGPALETTE; 

/*      *************************************************************
        *                                                           *
        *                Structures                                 *
        *                                                           *
        *************************************************************
*/

typedef LCD_COLOR       GUI_COLOR;
typedef LCD_LOGPALETTE  GUI_LOGPALETTE;
typedef LCD_DRAWMODE    GUI_DRAWMODE;
typedef LCD_RECT        GUI_RECT;

typedef struct {
  void      (* pfDraw)(int x0,int y0,int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag);
  GUI_COLOR (* pfIndex2Color)(int Index);
} GUI_BITMAP_METHODS;

typedef struct {
  U16P XSize;
  U16P YSize;
  U16P BytesPerLine;
  U16P BitsPerPixel;
  const U8 GUI_UNI_PTR * pData;
  const GUI_LOGPALETTE GUI_UNI_PTR * pPal;
  const GUI_BITMAP_METHODS * pMethods;
} GUI_BITMAP;

/* This structure may not be changed because the data that it
   expects is read in binary form (via any kind of interface,
   at runtime).
   This structure should therefor not be changed.
*/
typedef struct {
  U16 ID;           /* Version 1.00 => 100*/
  U16 Version;
  U16 XSize;
  U16 YSize;
  U16 BytesPerLine;
  U16 BitsPerPixel;
  U16 NumColors;
  U16 HasTrans;
} GUI_BITMAP_STREAM;

typedef struct {
  int x,y;
  unsigned char Pressed;
} GUI_PID_STATE;

/* Translation list. Translates a character code into up to 2
   indices of images to display on top of each other;
   'á' -> index('a'), index('´') */
typedef struct {
  I16P c0;
  I16P c1;
} GUI_FONT_TRANSLIST;

typedef struct {
  U16P FirstChar;
  U16P LastChar;
  const GUI_FONT_TRANSLIST GUI_UNI_PTR * pList;
} GUI_FONT_TRANSINFO;

typedef struct {
  U8 XSize;
  U8 XDist;
  U8 BytesPerLine;
  const unsigned char GUI_UNI_PTR * pData;
} GUI_CHARINFO;

typedef struct GUI_FONT_PROP {
  U16P First;                                /* first character               */
  U16P Last;                                 /* last character                */
  const GUI_CHARINFO GUI_UNI_PTR * paCharInfo;            /* address of first character    */
  const struct GUI_FONT_PROP GUI_UNI_PTR * pNext;        /* pointer to next */
} GUI_FONT_PROP;

typedef struct {
  const unsigned char GUI_UNI_PTR * pData;
  const U8 GUI_UNI_PTR * pTransData;
  const GUI_FONT_TRANSINFO GUI_UNI_PTR * pTrans;
  U16P FirstChar;
  U16P LastChar;
  U8 XSize;
  U8 XDist;
  U8 BytesPerLine;
} GUI_FONT_MONO;

typedef struct GUI_FONT_INFO {
  U16P First;                        /* first character               */
  U16P Last;                         /* last character                */
  const GUI_CHARINFO* paCharInfo;    /* address of first character    */
  const struct GUI_FONT_INFO* pNext; /* pointer to next */
} GUI_FONT_INFO;

/**********************************************************************
*
*                 FONT Encoding
*
***********************************************************************
*/

typedef struct {
  tGUI_GetLineDistX*          pfGetLineDistX;
  tGUI_GetLineLen*            pfGetLineLen;
  tGL_DispLine*               pfDispLine;
} tGUI_ENC_APIList;

struct GUI_FONT {
  GUI_DISPCHAR*     pfDispChar; 
  GUI_GETCHARDISTX* pfGetCharDistX; 
  GUI_GETFONTINFO*  pfGetFontInfo; 
  GUI_ISINFONT*     pfIsInFont;
  const tGUI_ENC_APIList* pafEncode;
  U8 YSize;
  U8 YDist;
  U8 XMag;
  U8 YMag;
  union {
    const void          GUI_UNI_PTR * pFontData;
    const GUI_FONT_MONO GUI_UNI_PTR * pMono;
    const GUI_FONT_PROP GUI_UNI_PTR * pProp;
  } p;
  U8 Baseline;
  U8 LHeight;     /* height of a small lower case character (a,x) */
  U8 CHeight;     /* height of a small upper case character (A,X) */
};

/**********************************************************************
*
*         UNICODE Encoding
*
***********************************************************************
*/
typedef U16  tGUI_GetCharCode(const char GUI_UNI_PTR *s);
typedef int  tGUI_GetCharSize(const char GUI_UNI_PTR *s);
typedef int  tGUI_CalcSizeOfChar(U16 Char);
typedef int  tGUI_Encode(char *s, U16 Char);

typedef struct {
  tGUI_GetCharCode*            pfGetCharCode;
  tGUI_GetCharSize*            pfGetCharSize;
  tGUI_CalcSizeOfChar*         pfCalcSizeOfChar;
  tGUI_Encode*                 pfEncode;
} GUI_UC_ENC_APILIST;

/*********************************************************************
*
*             GUI_CONTEXT
*
**********************************************************************

  This structure is public for one reason only:
  To allow the application to save and restore the context.
*/

typedef union {
  U8  aColorIndex8[2];
  U16 aColorIndex16[2];
} LCD_COLORINDEX_UNION;

typedef struct {
/* Variables in LCD module */
  LCD_COLORINDEX_UNION LCD;
  LCD_RECT       ClipRect;
  U8             DrawMode;
  U8             SelLayer;
  U8             TextStyle;
/* Variables in GL module */
  GUI_RECT* pClipRect_HL;                /* High level clip rectangle ... Speed optimization so drawing routines can optimize */
  U8        PenSize;
  U8        PenShape;
  U8        LineStyle;
  U8        FillStyle;
/* Variables in GUICHAR module */
  const GUI_FONT           GUI_UNI_PTR * pAFont;
  const GUI_UC_ENC_APILIST * pUC_API;    /* Unicode encoding API */
  I16P LBorder;
  I16P DispPosX, DispPosY;
  I16P DrawPosX, DrawPosY;
  I16P TextMode, TextAlign;
  GUI_COLOR Color, BkColor;           /* Required only when changing devices and for speed opt (caching) */
/* Variables in WM module */
  #if GUI_WINSUPPORT
    const GUI_RECT* WM__pUserClipRect;
    GUI_HWIN hAWin;
    int xOff, yOff;
  #endif
/* Variables in MEMDEV module (with memory devices only) */
  #if GUI_SUPPORT_DEVICES
    const tLCDDEV_APIList* pDeviceAPI;  /* function pointers only */
    GUI_HMEM    hDevData;
    GUI_RECT    ClipRectPrev;
  #endif
/* Variables in Anitaliasing module */
  #if GUI_SUPPORT_AA
    const tLCD_HL_APIList* pLCD_HL;     /* Required to reroute drawing (HLine & Pixel) to the AA module */
    U8 AA_Factor;
    U8 AA_HiResEnable;
  #endif
} GUI_CONTEXT;

#define LCD_BKCOLORINDEX GUI_Context.LCD.aColorIndex16[0]
#define LCD_COLORINDEX   GUI_Context.LCD.aColorIndex16[1]
#define LCD_ACOLORINDEX  GUI_Context.LCD.aColorIndex16

#define GUI_UC__GetCharSize(sText)  GUI_Context.pUC_API->pfGetCharSize(sText)
#define GUI_UC__GetCharCode(sText)  GUI_Context.pUC_API->pfGetCharCode(sText)

#define GUI_LOCK()
#define GUI_UNLOCK()

#define CLIP_X() \
  if (x0 < GUI_Context.ClipRect.x0) { x0 = GUI_Context.ClipRect.x0; } \
  if (x1 > GUI_Context.ClipRect.x1) { x1 = GUI_Context.ClipRect.x1; }

#define CLIP_Y() \
  if (y0 < GUI_Context.ClipRect.y0) { y0 = GUI_Context.ClipRect.y0; } \
  if (y1 > GUI_Context.ClipRect.y1) { y1 = GUI_Context.ClipRect.y1; }

/* ×Ö¿â */
extern const GUI_FONT GUI_FontTable;

/** 
 *  Ë¢ÆÁ·¶Àý: 
 *
 *  sprintf(disp_buf, "»¶Ó­Ê¹ÓÃ:");
 *  GUI_DispStringAt(disp_buf, 0, 16);  
 *  sprintf(disp_buf, "COG12864");
 *  GUI_DispStringAt(disp_buf, 16, 32); 
 *
 *  lcd_disp_map((unsigned char *)LCD_DISP_RAM);
 */
extern unsigned char lcd_disp_buf[LCD_YSIZE / 8][LCD_XSIZE];

void GUI_Init(void);
void LCD_On(void);
void LCD_Off(void);
void GUI_DispStringAt(const char GUI_UNI_PTR *s, int x, int y);
void GUI_DispRevStringAt(const char GUI_UNI_PTR *s, int x, int y);


#endif   /* ifdef GUI_H */

/*************************** End of file ****************************/

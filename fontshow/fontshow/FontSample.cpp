#include "stdafx.h"
#include "FontSample.h"

// NOTE: These bitmap tables are raw data and are device dependent and
//       have been found in a number of places on the internet. Since
//       the raw data is device dependent, any of these tables may require
//       some kind of a transformation or to be iterated over in a
//       from right to left rather than left to right or bits may need
//       to be iterated over from least significant to most significant
//       rather than most significant to least significant.
//
// See https://stackoverflow.com/questions/67465098/differences-in-bitmap-or-rasterized-font-bitmaps-and-text-display-on-3-5-tft-lc

FontSample::FontSample(const unsigned char *table /* = nullptr */ , unsigned char nRows /* = 0 */, unsigned char nCols /* = 0 */, unsigned short nFlags /* = 0 */)
{
	font_table.table = table;
	font_table.nRows = nRows;
	font_table.nCols = nCols;
	font_table.bFlags = nFlags;
}


FontSample::~FontSample()
{
}


//  following table from URL https://forum.arduino.cc/t/font-generation-for-bitmaps/161582/11

const unsigned char font_table_5_col[96][5] = {             // IDM_FILE_TABLE_5_COL
	{ 0x00, 0x00, 0x00, 0x00, 0x00 } // 20
	,{ 0x00, 0x00, 0x5f, 0x00, 0x00 } // 21 !
	,{ 0x00, 0x07, 0x00, 0x07, 0x00 } // 22 "
	,{ 0x14, 0x7f, 0x14, 0x7f, 0x14 } // 23 #
	,{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 } // 24 $
	,{ 0x23, 0x13, 0x08, 0x64, 0x62 } // 25 %
	,{ 0x36, 0x49, 0x55, 0x22, 0x50 } // 26 &
	,{ 0x00, 0x05, 0x03, 0x00, 0x00 } // 27 �
	,{ 0x00, 0x1c, 0x22, 0x41, 0x00 } // 28 (
	,{ 0x00, 0x41, 0x22, 0x1c, 0x00 } // 29 )
	,{ 0x14, 0x08, 0x3e, 0x08, 0x14 } // 2a *
	,{ 0x08, 0x08, 0x3e, 0x08, 0x08 } // 2b +
	,{ 0x00, 0x50, 0x30, 0x00, 0x00 } // 2c ,
	,{ 0x08, 0x08, 0x08, 0x08, 0x08 } // 2d -
	,{ 0x00, 0x60, 0x60, 0x00, 0x00 } // 2e .
	,{ 0x20, 0x10, 0x08, 0x04, 0x02 } // 2f /
	,{ 0x3e, 0x51, 0x49, 0x45, 0x3e } // 30 0
	,{ 0x00, 0x42, 0x7f, 0x40, 0x00 } // 31 1
	,{ 0x42, 0x61, 0x51, 0x49, 0x46 } // 32 2
	,{ 0x21, 0x41, 0x45, 0x4b, 0x31 } // 33 3
	,{ 0x18, 0x14, 0x12, 0x7f, 0x10 } // 34 4
	,{ 0x27, 0x45, 0x45, 0x45, 0x39 } // 35 5
	,{ 0x3c, 0x4a, 0x49, 0x49, 0x30 } // 36 6
	,{ 0x01, 0x71, 0x09, 0x05, 0x03 } // 37 7
	,{ 0x36, 0x49, 0x49, 0x49, 0x36 } // 38 8
	,{ 0x06, 0x49, 0x49, 0x29, 0x1e } // 39 9
	,{ 0x00, 0x36, 0x36, 0x00, 0x00 } // 3a :
	,{ 0x00, 0x56, 0x36, 0x00, 0x00 } // 3b ;
	,{ 0x08, 0x14, 0x22, 0x41, 0x00 } // 3c <
	,{ 0x11, 0x11, 0x11, 0x11, 0x11 } // 3d =
	,{ 0x00, 0x41, 0x22, 0x14, 0x08 } // 3e >
	,{ 0x02, 0x01, 0x51, 0x09, 0x06 } // 3f ?
	,{ 0x32, 0x49, 0x79, 0x41, 0x3e } // 40 @
	,{ 0x7e, 0x11, 0x11, 0x11, 0x7e } // 41 A
	,{ 0x7f, 0x49, 0x49, 0x49, 0x36 } // 42 B
	,{ 0x3e, 0x41, 0x41, 0x41, 0x22 } // 43 C
	,{ 0x7f, 0x41, 0x41, 0x22, 0x1c } // 44 D
	,{ 0x7f, 0x49, 0x49, 0x49, 0x41 } // 45 E
	,{ 0x7f, 0x09, 0x09, 0x09, 0x01 } // 46 F
	,{ 0x3e, 0x41, 0x49, 0x49, 0x7a } // 47 G
	,{ 0x7f, 0x08, 0x08, 0x08, 0x7f } // 48 H
	,{ 0x00, 0x41, 0x7f, 0x41, 0x00 } // 49 I
	,{ 0x20, 0x40, 0x41, 0x3f, 0x01 } // 4a J
	,{ 0x7f, 0x08, 0x14, 0x22, 0x41 } // 4b K
	,{ 0x7f, 0x40, 0x40, 0x40, 0x40 } // 4c L
	,{ 0x7f, 0x02, 0x0c, 0x02, 0x7f } // 4d M
	,{ 0x7f, 0x04, 0x08, 0x10, 0x7f } // 4e N
	,{ 0x3e, 0x41, 0x41, 0x41, 0x3e } // 4f O
	,{ 0x7f, 0x09, 0x09, 0x09, 0x06 } // 50 P
	,{ 0x3e, 0x41, 0x51, 0x21, 0x5e } // 51 Q
	,{ 0x7f, 0x09, 0x19, 0x29, 0x46 } // 52 R
	,{ 0x46, 0x49, 0x49, 0x49, 0x31 } // 53 S
	,{ 0x01, 0x01, 0x7f, 0x01, 0x01 } // 54 T
	,{ 0x3f, 0x40, 0x40, 0x40, 0x3f } // 55 U
	,{ 0x1f, 0x20, 0x40, 0x20, 0x1f } // 56 V
	,{ 0x3f, 0x40, 0x38, 0x40, 0x3f } // 57 W
	,{ 0x63, 0x14, 0x08, 0x14, 0x63 } // 58 X
	,{ 0x07, 0x08, 0x70, 0x08, 0x07 } // 59 Y
	,{ 0x61, 0x51, 0x49, 0x45, 0x43 } // 5a Z
	,{ 0x00, 0x7f, 0x41, 0x41, 0x00 } // 5b [
	,{ 0x02, 0x04, 0x08, 0x10, 0x20 } // 5c �
	,{ 0x00, 0x41, 0x41, 0x7f, 0x00 } // 5d ]
	,{ 0x04, 0x02, 0x01, 0x02, 0x04 } // 5e ^
	,{ 0x40, 0x40, 0x40, 0x40, 0x40 } // 5f _
	,{ 0x00, 0x01, 0x02, 0x04, 0x00 } // 60 `
	,{ 0x20, 0x54, 0x54, 0x54, 0x78 } // 61 a
	,{ 0x7f, 0x48, 0x44, 0x44, 0x38 } // 62 b
	,{ 0x38, 0x44, 0x44, 0x44, 0x20 } // 63 c
	,{ 0x38, 0x44, 0x44, 0x48, 0x7f } // 64 d
	,{ 0x38, 0x54, 0x54, 0x54, 0x18 } // 65 e
	,{ 0x08, 0x7e, 0x09, 0x01, 0x02 } // 66 f
	,{ 0x0c, 0x52, 0x52, 0x52, 0x3e } // 67 g
	,{ 0x7f, 0x08, 0x04, 0x04, 0x78 } // 68 h
	,{ 0x00, 0x44, 0x7d, 0x40, 0x00 } // 69 i
	,{ 0x20, 0x40, 0x44, 0x3d, 0x00 } // 6a j
	,{ 0x7f, 0x10, 0x28, 0x44, 0x00 } // 6b k
	,{ 0x00, 0x41, 0x7f, 0x40, 0x00 } // 6c l
	,{ 0x7c, 0x04, 0x18, 0x04, 0x78 } // 6d m
	,{ 0x7c, 0x08, 0x04, 0x04, 0x78 } // 6e n
	,{ 0x38, 0x44, 0x44, 0x44, 0x38 } // 6f o
	,{ 0x7c, 0x14, 0x14, 0x14, 0x08 } // 70 p
	,{ 0x08, 0x14, 0x14, 0x18, 0x7c } // 71 q
	,{ 0x7c, 0x08, 0x04, 0x04, 0x08 } // 72 r
	,{ 0x48, 0x54, 0x54, 0x54, 0x20 } // 73 s
	,{ 0x04, 0x3f, 0x44, 0x40, 0x20 } // 74 t
	,{ 0x3c, 0x40, 0x40, 0x20, 0x7c } // 75 u
	,{ 0x1c, 0x20, 0x40, 0x20, 0x1c } // 76 v
	,{ 0x3c, 0x40, 0x30, 0x40, 0x3c } // 77 w
	,{ 0x44, 0x28, 0x10, 0x28, 0x44 } // 78 x
	,{ 0x0c, 0x50, 0x50, 0x50, 0x3c } // 79 y
	,{ 0x44, 0x64, 0x54, 0x4c, 0x44 } // 7a z
	,{ 0x00, 0x08, 0x36, 0x41, 0x00 } // 7b {
	,{ 0x00, 0x00, 0x7f, 0x00, 0x00 } // 7c |
	,{ 0x00, 0x41, 0x36, 0x08, 0x00 } // 7d }
	,{ 0x10, 0x08, 0x08, 0x10, 0x08 } // 7e
	,{ 0x78, 0x46, 0x41, 0x46, 0x78 } // 7f
};



// We also have found a smaller bitmap font table that
// is about half the size of the original bitmap font table.
// See https://github.com/dhepper/font8x8

const unsigned char font_table_7_col[96][7] = {         // IDM_FILE_TABLE_7_COL
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0020 (space)
	{ 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18 },   // U+0021 (!)
	{ 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0022 (")
	{ 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36 },   // U+0023 (#)
	{ 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C },   // U+0024 ($)
	{ 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63 },   // U+0025 (%)
	{ 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E },   // U+0026 (&)
	{ 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00 },   // U+0027 (')
	{ 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18 },   // U+0028 (()
	{ 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06 },   // U+0029 ())
	{ 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00 },   // U+002A (*)
	{ 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00 },   // U+002B (+)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C },   // U+002C (,)
	{ 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00 },   // U+002D (-)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C },   // U+002E (.)
	{ 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01 },   // U+002F (/)
	{ 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E },   // U+0030 (0)
	{ 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F },   // U+0031 (1)
	{ 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F },   // U+0032 (2)
	{ 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E },   // U+0033 (3)
	{ 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78 },   // U+0034 (4)
	{ 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E },   // U+0035 (5)
	{ 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E },   // U+0036 (6)
	{ 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C },   // U+0037 (7)
	{ 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E },   // U+0038 (8)
	{ 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E },   // U+0039 (9)
	{ 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C },   // U+003A (:)
	{ 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C },   // U+003B (;)
	{ 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18 },   // U+003C (<)
	{ 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00 },   // U+003D (=)
	{ 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06 },   // U+003E (>)
	{ 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C },   // U+003F (?)
	{ 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E },   // U+0040 (@)
	{ 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33 },   // U+0041 (A)
	{ 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F },   // U+0042 (B)
	{ 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C },   // U+0043 (C)
	{ 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F },   // U+0044 (D)
	{ 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F },   // U+0045 (E)
	{ 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F },   // U+0046 (F)
	{ 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C },   // U+0047 (G)
	{ 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33 },   // U+0048 (H)
	{ 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E },   // U+0049 (I)
	{ 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E },   // U+004A (J)
	{ 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67 },   // U+004B (K)
	{ 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F },   // U+004C (L)
	{ 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63 },   // U+004D (M)
	{ 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63 },   // U+004E (N)
	{ 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C },   // U+004F (O)
	{ 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F },   // U+0050 (P)
	{ 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38 },   // U+0051 (Q)
	{ 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67 },   // U+0052 (R)
	{ 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E },   // U+0053 (S)
	{ 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E },   // U+0054 (T)
	{ 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F },   // U+0055 (U)
	{ 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C },   // U+0056 (V)
	{ 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63 },   // U+0057 (W)
	{ 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63 },   // U+0058 (X)
	{ 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E },   // U+0059 (Y)
	{ 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F },   // U+005A (Z)
	{ 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E },   // U+005B ([)
	{ 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40 },   // U+005C (\)
	{ 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E },   // U+005D (])
	{ 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00 },   // U+005E (^)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+005F (_)
	{ 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00 },   // U+0060 (`)
	{ 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E },   // U+0061 (a)
	{ 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B },   // U+0062 (b)
	{ 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E },   // U+0063 (c)
	{ 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E },   // U+0064 (d)
	{ 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E },   // U+0065 (e)
	{ 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F },   // U+0066 (f)
	{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30 },   // U+0067 (g)
	{ 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67 },   // U+0068 (h)
	{ 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E },   // U+0069 (i)
	{ 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33 },   // U+006A (j)
	{ 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67 },   // U+006B (k)
	{ 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E },   // U+006C (l)
	{ 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63 },   // U+006D (m)
	{ 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33 },   // U+006E (n)
	{ 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E },   // U+006F (o)
	{ 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06 },   // U+0070 (p)
	{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30 },   // U+0071 (q)
	{ 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F },   // U+0072 (r)
	{ 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F },   // U+0073 (s)
	{ 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18 },   // U+0074 (t)
	{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E },   // U+0075 (u)
	{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C },   // U+0076 (v)
	{ 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36 },   // U+0077 (w)
	{ 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63 },   // U+0078 (x)
	{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30 },   // U+0079 (y)
	{ 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F },   // U+007A (z)
	{ 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38 },   // U+007B ({)
	{ 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18 },   // U+007C (|)
	{ 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07 },   // U+007D (})
	{ 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00 }    // U+007E (~)
};

const unsigned char font_table_8_col[96][8] = {         // IDM_FILE_TABLE_8_COL
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0020 (space)
	{ 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00 },   // U+0021 (!)
	{ 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0022 (")
	{ 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00 },   // U+0023 (#)
	{ 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00 },   // U+0024 ($)
	{ 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00 },   // U+0025 (%)
	{ 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00 },   // U+0026 (&)
	{ 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0027 (')
	{ 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00 },   // U+0028 (()
	{ 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00 },   // U+0029 ())
	{ 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00 },   // U+002A (*)
	{ 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00 },   // U+002B (+)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06 },   // U+002C (,)
	{ 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00 },   // U+002D (-)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00 },   // U+002E (.)
	{ 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00 },   // U+002F (/)
	{ 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00 },   // U+0030 (0)
	{ 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00 },   // U+0031 (1)
	{ 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00 },   // U+0032 (2)
	{ 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00 },   // U+0033 (3)
	{ 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00 },   // U+0034 (4)
	{ 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00 },   // U+0035 (5)
	{ 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00 },   // U+0036 (6)
	{ 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00 },   // U+0037 (7)
	{ 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00 },   // U+0038 (8)
	{ 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00 },   // U+0039 (9)
	{ 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00 },   // U+003A (:)
	{ 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06 },   // U+003B (;)
	{ 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00 },   // U+003C (<)
	{ 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00 },   // U+003D (=)
	{ 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00 },   // U+003E (>)
	{ 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00 },   // U+003F (?)
	{ 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00 },   // U+0040 (@)
	{ 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00 },   // U+0041 (A)
	{ 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00 },   // U+0042 (B)
	{ 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00 },   // U+0043 (C)
	{ 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00 },   // U+0044 (D)
	{ 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00 },   // U+0045 (E)
	{ 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00 },   // U+0046 (F)
	{ 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00 },   // U+0047 (G)
	{ 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00 },   // U+0048 (H)
	{ 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0049 (I)
	{ 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00 },   // U+004A (J)
	{ 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00 },   // U+004B (K)
	{ 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00 },   // U+004C (L)
	{ 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00 },   // U+004D (M)
	{ 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00 },   // U+004E (N)
	{ 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00 },   // U+004F (O)
	{ 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00 },   // U+0050 (P)
	{ 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00 },   // U+0051 (Q)
	{ 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00 },   // U+0052 (R)
	{ 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00 },   // U+0053 (S)
	{ 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0054 (T)
	{ 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00 },   // U+0055 (U)
	{ 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00 },   // U+0056 (V)
	{ 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00 },   // U+0057 (W)
	{ 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00 },   // U+0058 (X)
	{ 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0059 (Y)
	{ 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00 },   // U+005A (Z)
	{ 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00 },   // U+005B ([)
	{ 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00 },   // U+005C (\)
	{ 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00 },   // U+005D (])
	{ 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00 },   // U+005E (^)
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF },   // U+005F (_)
	{ 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0060 (`)
	{ 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00 },   // U+0061 (a)
	{ 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00 },   // U+0062 (b)
	{ 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00 },   // U+0063 (c)
	{ 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00 },   // U+0064 (d)
	{ 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00 },   // U+0065 (e)
	{ 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00 },   // U+0066 (f)
	{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F },   // U+0067 (g)
	{ 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00 },   // U+0068 (h)
	{ 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0069 (i)
	{ 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E },   // U+006A (j)
	{ 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00 },   // U+006B (k)
	{ 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+006C (l)
	{ 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00 },   // U+006D (m)
	{ 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00 },   // U+006E (n)
	{ 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00 },   // U+006F (o)
	{ 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F },   // U+0070 (p)
	{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78 },   // U+0071 (q)
	{ 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00 },   // U+0072 (r)
	{ 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00 },   // U+0073 (s)
	{ 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00 },   // U+0074 (t)
	{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00 },   // U+0075 (u)
	{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00 },   // U+0076 (v)
	{ 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00 },   // U+0077 (w)
	{ 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00 },   // U+0078 (x)
	{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F },   // U+0079 (y)
	{ 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00 },   // U+007A (z)
	{ 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00 },   // U+007B ({)
	{ 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00 },   // U+007C (|)
	{ 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00 },   // U+007D (})
	{ 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }    // U+007E (~)
};

const unsigned char font_table_13_col[96][13] = {         // IDM_FILE_TABLE_13_COL
	// from URL https://courses.cs.washington.edu/courses/cse457/98a/tech/OpenGL/font.c
	//  GLubyte rasters[][13] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*" ",0*/
	{ 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },/*"!",1*/
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36 },/*""",2*/
	{ 0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00 },/*"#",3*/
	{ 0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18 },/*"$",4*/
	{ 0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70 },/*"%",5*/
	{ 0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38 },/*"&",6*/
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e },/*"'",7*/
	{ 0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c },/*"(",8*/
	{ 0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30 },/*")",9*/
	{ 0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00 },/*"*",10*/
	{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00 },/*"+",11*/
	{ 0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*",",12*/
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"-",13*/
	{ 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*".",14*/
	{ 0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03 },/*"/",15*/
	{ 0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c },/*"0",16*/
	{ 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18 },/*"1",17*/
	{ 0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e },/*"2",18*/
	{ 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e },/*"3",19*/
	{ 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c },/*"4",20*/
	{ 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff },/*"5",21*/
	{ 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e },/*"6",22*/
	{ 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff },/*"7",23*/
	{ 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e },/*"8",24*/
	{ 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e },/*"9",25*/
	{ 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00 },/*":",26*/
	{ 0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00 },/*";",27*/
	{ 0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06 },/*"<",28*/
	{ 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 },/*"=",29*/
	{ 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60 },/*">",30*/
	{ 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e },/*"?",31*/
	{ 0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00 },/*"@",32*/
	{ 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18 },/*"A",33*/
	{ 0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe },/*"B",34*/
	{ 0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e },
	{ 0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc },
	{ 0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff },
	{ 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff },
	{ 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e },
	{ 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 },
	{ 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e },
	{ 0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06 },
	{ 0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3 },
	{ 0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 },
	{ 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3 },
	{ 0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3 },
	{ 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e },
	{ 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe },
	{ 0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c },
	{ 0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe },
	{ 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e },
	{ 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff },/*"T",52*/
	{ 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 },/*"U",53*/
	{ 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 },/*"V",54*/
	{ 0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 },/*"W",55*/
	{ 0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3 },
	{ 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3 },
	{ 0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff },/*"Z",58*/
	{ 0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c },
	{ 0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60 },
	{ 0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18 },
	{ 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70 },
	{ 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 },
	{ 0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03 },
	{ 0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e },
	{ 0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0 },
	{ 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00 },
	{ 0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00 },
	{ 0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0 },
	{ 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78 },
	{ 0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00 },
	{ 0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00 },
	{ 0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00 },
	{ 0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00 },
	{ 0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f },
	{ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
	{ 0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00 }
};

const unsigned char font_table_16_col[96][16] = {         // IDM_FILE_TABLE_16_COL
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*" ",0*/
	{ 0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x38,0x38,0x00,0x00 },/*"!",1*/
	{ 0x00,0xD8,0xFC,0x6C,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*""",2*/
	{ 0x00,0x00,0x00,0x6C,0x6C,0x6C,0xFF,0x36,0x36,0x36,0xFF,0x36,0x36,0x36,0x00,0x00 },/*"#",3*/
	{ 0x00,0x00,0x18,0x3C,0x7E,0x7E,0x1E,0x1C,0x38,0x78,0x78,0x7E,0x7E,0x3C,0x18,0x18 },/*"$",4*/
	{ 0x00,0x00,0x00,0x66,0x6F,0x3F,0x3F,0x3F,0x7E,0xF8,0xFC,0xFC,0xFC,0x66,0x00,0x00 },/*"%",5*/
	{ 0x00,0x00,0x00,0x1C,0x36,0x36,0x36,0x1E,0xFE,0x6F,0x7B,0x33,0xB3,0xFE,0x00,0x00 },/*"&",6*/
	{ 0x00,0x0E,0x0E,0x0C,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"'",7*/
	{ 0x00,0xC0,0x60,0x30,0x30,0x18,0x18,0x18,0x18,0x18,0x18,0x30,0x30,0x60,0xC0,0x00 },/*"(",8*/
	{ 0x00,0x06,0x0C,0x18,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x18,0x0C,0x06,0x00 },/*")",9*/
	{ 0x00,0x00,0x00,0x00,0x18,0x18,0xFF,0x3C,0x3C,0xFF,0x18,0x18,0x00,0x00,0x00,0x00 },/*"*",10*/
	{ 0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x18,0xFF,0x18,0x18,0x18,0x18,0x00,0x00,0x00 },/*"+",11*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0E,0x0C,0x07 },/*",",12*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"-",13*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0E,0x00,0x00 },/*".",14*/
	{ 0x00,0x00,0x80,0xC0,0xC0,0x60,0x60,0x30,0x30,0x18,0x18,0x0C,0x0C,0x06,0x06,0x00 },/*"/",15*/
	{ 0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00 },/*"0",16*/
	{ 0x00,0x00,0x00,0x18,0x1E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00 },/*"1",17*/
	{ 0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x60,0x60,0x30,0x18,0x0C,0xC6,0xFE,0x00,0x00 },/*"2",18*/
	{ 0x00,0x00,0x00,0x7C,0xC6,0xC6,0x60,0x38,0x60,0xC0,0xC0,0xC6,0x66,0x3C,0x00,0x00 },/*"3",19*/
	{ 0x00,0x00,0x00,0x60,0x70,0x78,0x6C,0x6C,0x66,0x66,0xFE,0x60,0x60,0xF8,0x00,0x00 },/*"4",20*/
	{ 0x00,0x00,0x00,0xFE,0x06,0x06,0x06,0x3E,0x6E,0xC0,0xC0,0xC6,0x66,0x3C,0x00,0x00 },/*"5",21*/
	{ 0x00,0x00,0x00,0x78,0x6C,0x06,0x06,0x3E,0x6E,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00 },/*"6",22*/
	{ 0x00,0x00,0x00,0xFE,0x66,0x66,0x30,0x30,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00 },/*"7",23*/
	{ 0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x6C,0x38,0x6C,0xC6,0xC6,0xC6,0x7C,0x00,0x00 },/*"8",24*/
	{ 0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xEC,0xF8,0xC0,0xC0,0x6C,0x3C,0x00,0x00 },/*"9",25*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x00,0x00,0x38,0x38,0x00,0x00 },/*":",26*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x0C },/*";",27*/
	{ 0x00,0x00,0x00,0xC0,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0xC0,0x00,0x00 },/*"<",28*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00 },/*"=",29*/
	{ 0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x0C,0x06,0x00,0x00 },/*">",30*/
	{ 0x00,0x00,0x00,0x7C,0xC6,0xC6,0xCE,0xC0,0x60,0x30,0x30,0x00,0x38,0x38,0x00,0x00 },/*"?",31*/
	{ 0x00,0x00,0x00,0x3C,0x66,0xFE,0xFF,0xFF,0xFF,0xFF,0x7F,0xC6,0x66,0x3C,0x00,0x00 },/*"@",32*/
	{ 0x00,0x00,0x00,0x18,0x18,0x38,0x3C,0x3C,0x6C,0x7C,0x66,0xC6,0xC6,0xEF,0x00,0x00 },/*"A",33*/
	{ 0x00,0x00,0x00,0x3F,0x66,0x66,0x66,0x3E,0x66,0xC6,0xC6,0xC6,0x66,0x3F,0x00,0x00 },/*"B",34*/
	{ 0x00,0x00,0x00,0xFC,0xC6,0xC6,0x03,0x03,0x03,0x03,0x03,0xC6,0x66,0x3C,0x00,0x00 },/*"C",35*/
	{ 0x00,0x00,0x00,0x3F,0x66,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x66,0x3F,0x00,0x00 },/*"D",36*/
	{ 0x00,0x00,0x00,0x7F,0xC6,0x36,0x36,0x3E,0x36,0x36,0x06,0xC6,0xC6,0x7F,0x00,0x00 },/*"E",37*/
	{ 0x00,0x00,0x00,0x7F,0xC6,0x36,0x36,0x3E,0x36,0x36,0x06,0x06,0x06,0x0F,0x00,0x00 },/*"F",38*/
	{ 0x00,0x00,0x00,0x7C,0x66,0x66,0x03,0x03,0x03,0xF3,0x63,0x66,0x66,0x3C,0x00,0x00 },/*"G",39*/
	{ 0x00,0x00,0x00,0xEF,0xC6,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xEF,0x00,0x00 },/*"H",40*/
	{ 0x00,0x00,0x00,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00 },/*"I",41*/
	{ 0x00,0x00,0x00,0xFC,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x33,0x1F },/*"J",42*/
	{ 0x00,0x00,0x00,0xFF,0x66,0x36,0x1E,0x1E,0x1E,0x36,0x36,0x66,0x66,0xFF,0x00,0x00 },/*"K",43*/
	{ 0x00,0x00,0x00,0x0F,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0xC6,0xFF,0x00,0x00 },/*"L",44*/
	{ 0x00,0x00,0x00,0xFF,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0xFF,0x00,0x00 },/*"M",45*/
	{ 0x00,0x00,0x00,0xE7,0xCE,0xCE,0xDE,0xDE,0xF6,0xF6,0xF6,0xE6,0xE6,0xCF,0x00,0x00 },/*"N",46*/
	{ 0x00,0x00,0x00,0x3C,0x66,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x66,0x3C,0x00,0x00 },/*"O",47*/
	{ 0x00,0x00,0x00,0x7F,0xC6,0xC6,0xC6,0xC6,0x7E,0x06,0x06,0x06,0x06,0x0F,0x00,0x00 },/*"P",48*/
	{ 0x00,0x00,0x00,0x3C,0x66,0xC3,0xC3,0xC3,0xC3,0xC3,0xDF,0xF7,0x76,0x3C,0xE0,0x00 },/*"Q",49*/
	{ 0x00,0x00,0x00,0x7F,0xC6,0xC6,0xC6,0x7E,0x36,0x36,0x66,0x66,0xC6,0xCF,0x00,0x00 },/*"R",50*/
	{ 0x00,0x00,0x00,0xFC,0xC6,0xC6,0x06,0x0C,0x38,0x60,0xC0,0xC6,0xC6,0x7E,0x00,0x00 },/*"S",51*/
	{ 0x00,0x00,0x00,0xFF,0xDB,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00 },/*"T",52*/
	{ 0x00,0x00,0x00,0xEF,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00 },/*"U",53*/
	{ 0x00,0x00,0x00,0xEF,0xC6,0xC6,0x66,0x6C,0x6C,0x3C,0x3C,0x38,0x18,0x18,0x00,0x00 },/*"V",54*/
	{ 0x00,0x00,0x00,0xFF,0xDB,0xDB,0xDB,0xDB,0xFF,0xFF,0x7E,0x66,0x66,0x66,0x00,0x00 },/*"W",55*/
	{ 0x00,0x00,0x00,0xEF,0xC6,0x6C,0x6C,0x38,0x38,0x38,0x6C,0x6C,0xC6,0xEF,0x00,0x00 },/*"X",56*/
	{ 0x00,0x00,0x00,0xFF,0x66,0x66,0x3C,0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00 },/*"Y",57*/
	{ 0x00,0x00,0x00,0xFE,0x63,0x60,0x30,0x30,0x18,0x0C,0x0C,0xC6,0xC6,0x7F,0x00,0x00 },/*"Z",58*/
	{ 0x00,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0x00 },/*"[",59*/
	{ 0x00,0x00,0x06,0x06,0x0C,0x0C,0x18,0x18,0x18,0x30,0x30,0x60,0x60,0x60,0xC0,0xC0 },/*"\",60*/
	{ 0x00,0x3E,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3E,0x00 },/*"]",61*/
	{ 0x00,0x78,0xCC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"^",62*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF },/*"_",63*/
	{ 0x00,0x0E,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"`",64*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xF8,0xCC,0xC6,0xC6,0xFC,0x00,0x00 },/*"a",65*/
	{ 0x00,0x00,0x00,0x07,0x06,0x06,0x06,0x3E,0x6E,0xC6,0xC6,0xC6,0x6E,0x3E,0x00,0x00 },/*"b",66*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0x06,0x06,0x06,0xCC,0x78,0x00,0x00 },/*"c",67*/
	{ 0x00,0x00,0x00,0xE0,0xC0,0xC0,0xC0,0xF8,0xCC,0xC6,0xC6,0xC6,0xEC,0xF8,0x00,0x00 },/*"d",68*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xFE,0x06,0x06,0xC6,0x7C,0x00,0x00 },/*"e",69*/
	{ 0x00,0x00,0x00,0xF0,0x98,0x18,0x18,0xFE,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00 },/*"f",70*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x66,0x66,0x3C,0x06,0x7C,0xC6,0xC6,0x7C },/*"g",71*/
	{ 0x00,0x00,0x00,0x07,0x06,0x06,0x06,0x7E,0xCE,0xC6,0xC6,0xC6,0xC6,0xEF,0x00,0x00 },/*"h",72*/
	{ 0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x1E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00 },/*"i",73*/
	{ 0x00,0x00,0x00,0x70,0x70,0x00,0x00,0x78,0x60,0x60,0x60,0x60,0x60,0x60,0x66,0x3E },/*"j",74*/
	{ 0x00,0x00,0x00,0x07,0x06,0x06,0x06,0xF6,0x36,0x1E,0x3E,0x36,0x66,0xFF,0x00,0x00 },/*"k",75*/
	{ 0x00,0x00,0x00,0x1E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00 },/*"l",76*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xB6,0xB6,0xB6,0xB6,0xB6,0xFF,0x00,0x00 },/*"m",77*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xCE,0xC6,0xC6,0xC6,0xC6,0xEF,0x00,0x00 },/*"n",78*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00 },/*"o",79*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x6E,0xC6,0xC6,0xC6,0x66,0x3E,0x06,0x0F },/*"p",80*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xCC,0xC6,0xC6,0xC6,0xCC,0xF8,0xC0,0xE0 },/*"q",81*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xDC,0x0C,0x0C,0x0C,0x0C,0x3F,0x00,0x00 },/*"r",82*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xC6,0x06,0x7C,0xC0,0xC6,0x7E,0x00,0x00 },/*"s",83*/
	{ 0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x18,0x18,0x18,0x70,0x00,0x00 },/*"t",84*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0xC6,0xC6,0xC6,0xC6,0xE6,0xFC,0x00,0x00 },/*"u",85*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0xC6,0x6C,0x6C,0x3C,0x18,0x18,0x00,0x00 },/*"v",86*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xDB,0xDB,0xFF,0xFF,0x66,0x66,0x00,0x00 },/*"w",87*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x6C,0x38,0x38,0x38,0x6C,0xFE,0x00,0x00 },/*"x",88*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0xC6,0x6C,0x6C,0x3C,0x38,0x18,0x18,0x0F },/*"y",89*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x66,0x30,0x18,0x18,0xCC,0xFE,0x00,0x00 },/*"z",90*/
	{ 0x00,0xC0,0x60,0x60,0x60,0x60,0x60,0x30,0x60,0x60,0x60,0x60,0x60,0x60,0xC0,0x00 },/*"{",91*/
	{ 0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30 },/*"|",92*/
	{ 0x00,0x0E,0x18,0x18,0x18,0x18,0x18,0x30,0x18,0x18,0x18,0x18,0x18,0x18,0x0E,0x00 },/*"}",93*/
	{ 0x1C,0x76,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"~",94*/
};

const unsigned char font_table_16_x_16_col[96][32] = {
	{ 0x0f,0x80,0x1e,0x80,0x3c,0x40,0x78,0x20,0x78,0x10,0xf0,0x08,0xe0,0x09,0xc0,0x05,
	  0xc0,0x02,0x40,0x07,0x20,0x0f,0x20,0x1e,0x10,0x1e,0x08,0x3c,0x04,0x78,0x02,0xf0 }, /* bitmap of the X11 logo as a 16 x 16 bitmap */
};

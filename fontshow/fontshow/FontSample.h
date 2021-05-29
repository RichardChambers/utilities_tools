#pragma once
class FontSample
{
public:
	struct FontTable {
		static const unsigned short  Flags_DoubleHigh = 0x001;   // display the character as twice as high as bitmap font indicates. 8 pixel high becomes 16 pixels high.
		static const unsigned short  Flags_DoubleWide = 0x002;   // display the character as twice as wide as bitmap font indicates. 8 pixel wide becomes 16 pixels wide.
		static const unsigned short  Flags_TripleHigh = 0x004;   // display the character as three as high as bitmap font indicates. 8 pixel high becomes 16 pixels high.
		static const unsigned short  Flags_TripleWide = 0x008;   // display the character as three as wide as bitmap font indicates. 8 pixel wide becomes 16 pixels wide.
		static const unsigned short  Flags_UpperOnly = 0x010;    // bitmap font table has upper case letters only
		static const unsigned short  Flags_InvertBitOrder = 0x020; // the bitmap table entries for each character are in reverse order
		static const unsigned short  Flags_RotateBits = 0x040;     // the bitmap table entries need to be rotated 90 degrees left to display properly
		static const unsigned short  Flags_NoLineFeed = 0x080;     // no line feed added to end of a text string automatically.
		static const unsigned short  Flags_WrapLine = 0x100;       // wrap text to next line
		unsigned short bFlags;    // above flags to indicate special handling.
		unsigned char nRows;     // number of rows in the bitmap font table of character bitmaps
		unsigned char nCols;     // number of columns in the bitmap font table of character bitmaps. also font height or width in pixels.
		const unsigned char *table;    // pointer to a bitmap font table of character bitmaps, static const unsigned char font_table[nRows][nCols];
								 // bitmap font table starts with bitmap for space character, 0x20, not control character 0x00.
	};

	FontSample(const unsigned char *font_table = nullptr, unsigned char nRows = 0, unsigned char nCols = 0, unsigned short nFlags = 0);
	~FontSample();

	FontTable font_table;      // configurable bitmap font font table to use for text.

};

extern const unsigned char font_table_5_col[96][5];           // IDM_FILE_TABLE_5_COL
extern const unsigned char font_table_7_col[96][7];           // IDM_FILE_TABLE_7_COL
extern const unsigned char font_table_8_col[96][8];           // IDM_FILE_TABLE_8_COL
extern const unsigned char font_table_13_col[96][13];         // IDM_FILE_TABLE_13_COL
extern const unsigned char font_table_16_col[96][16];         // IDM_FILE_TABLE_16_COL
extern const unsigned char font_table_16_x_16_col[96][32];

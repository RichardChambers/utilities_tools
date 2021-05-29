// fontshow.cpp : Defines the entry point for the application.
//
//
// For sample raster font bitmaps see URL https://github.com/viler-int10h/vga-text-mode-fonts
//

#include "stdafx.h"
#include "fontshow.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FONTSHOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FONTSHOW));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FONTSHOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FONTSHOW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#include <iostream>
#include "FontSample.h"

unsigned char fontFileBuffer[8 * 1024];

wchar_t         szFontFileName[64];

// this function is the initial font display of the comparison of the
// several font tables to show the differences between the representations.
int paintFontDisplay(HWND hWnd, unsigned short iFirst = 0, unsigned short iLast = 10, unsigned short iHeight = 0, unsigned short iWidth = 0)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	SetTextAlign(hdc, TA_CENTER);

	RECT rect;
	GetClientRect(hWnd, &rect);

	//Logical units are device dependent pixels, so this will create a handle to a logical font that is 48 pixels in height.
	//The width, when set to 0, will cause the font mapper to choose the closest matching value.
	//The font face name will be Impact.
	HFONT hFont = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH, TEXT("Courier"));
	SelectObject(hdc, hFont);

	// TODO: Add any drawing code that uses hdc here...

	POINT outPoint;
	outPoint.x = rect.left + 80;
	outPoint.y = rect.top + 20;
	for (int i = iFirst; i < iLast; i++) {
		for (int j = 0; j < 5; j++) {
			std::wstring charRep;
			for (unsigned char k = 0x80; k; k >>= 1) {
				if (font_table_5_col[i][j] & k) {
					charRep += '*';
				}
				else {
					charRep += '-';
				}
			}
			TextOut(hdc, outPoint.x, outPoint.y, charRep.c_str(), charRep.length());
			outPoint.y += 20;
		}
		outPoint.y += 20 + 20 * 11;
	}

	outPoint.x = outPoint.x + 200;
	outPoint.y = rect.top + 20;
	for (int i = iFirst; i < iLast; i++) {
		for (int j = 0; j < 8; j++) {
			std::wstring charRep;
			for (unsigned char k = 0x80; k; k >>= 1) {
				if (font_table_8_col[i][j] & k) {
					charRep += '*';
				}
				else {
					charRep += '-';
				}
			}
			TextOut(hdc, outPoint.x, outPoint.y, charRep.c_str(), charRep.length());
			outPoint.y += 20;
		}
		outPoint.y += 20 + 20 * 8;
	}

	outPoint.x = outPoint.x + 200;
	outPoint.y = rect.top + 20;
	for (int i = iFirst; i < iLast; i++) {
		for (int j = 0; j < 13; j++) {
			std::wstring charRep;
			for (unsigned char k = 0x80; k; k >>= 1) {
				if (font_table_13_col[i][j] & k) {
					charRep += '*';
				}
				else {
					charRep += '-';
				}
			}
			TextOut(hdc, outPoint.x, outPoint.y, charRep.c_str(), charRep.length());
			outPoint.y += 20;
		}
		outPoint.y += 20 + 20 * 3;
	}

	outPoint.x = outPoint.x + 200;
	outPoint.y = rect.top + 20;
	for (int i = iFirst; i < iLast; i++) {
		for (int j = 0; j < 16; j++) {
			std::wstring charRep;
			for (unsigned char k = 0x80; k; k >>= 1) {
				if (font_table_16_col[i][j] & k) {
					charRep += '*';
				}
				else {
					charRep += '-';
				}
			}
			TextOut(hdc, outPoint.x, outPoint.y, charRep.c_str(), charRep.length());
			outPoint.y += 20;
		}
		outPoint.y += 20;
	}

	EndPaint(hWnd, &ps);

	return 0;
}


// Process the bytes representing a character's glyph from left to right
// instead of right to left.
// Bits of each byte are processed Most Significant (0x80) to Least Significant (0x01).

int paintFontDisplay2(HWND hWnd, unsigned short iFirst, unsigned short iLast, unsigned short iHeight, unsigned short iWidth = 8)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT rect;
	GetClientRect(hWnd, &rect);

	//Logical units are device dependent pixels, so this will create a handle to a logical font that is 24 pixels in height.
	//The width, when set to 0, will cause the font mapper to choose the closest matching value.
	//The font face name will be Impact.
	HFONT hFont = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH, TEXT("Courier"));
	SelectObject(hdc, hFont);

	POINT outPoint;
	outPoint.x = rect.left + 80;
	outPoint.y = rect.top + 20;

	// handle the case if the raster font is wider than 8 pixels.
	// if it is then we will need to use two bytes for each line
	// of the glyph rather than one byte for each line of the glyph.
	if (iWidth > 8) iHeight *= 2;
	for (unsigned short i = iFirst; i < iLast; i++) {
		for (unsigned short j = 0; j < iHeight; j++) {
			std::wstring charRep;
			for (unsigned char k = 0x80; k; k >>= 1) {
				if (fontFileBuffer[i * iHeight + j] & k) {
					charRep += '*';
				}
				else {
					charRep += '-';
				}
			}
			if (iWidth > 8) {
				// if the width is greater than 8 pixels then we need to
				// continue drawing this line with the next byte.
				j++;
				for (unsigned char k = 0x80; k; k >>= 1) {
					if (fontFileBuffer[i * iHeight + j] & k) {
						charRep += '*';
					}
					else {
						charRep += '-';
					}
				}
			}
			TextOut(hdc, outPoint.x, outPoint.y, charRep.c_str(), charRep.length());
			outPoint.y += 20;
		}
		outPoint.y += 20;
		if (outPoint.y + 20 >= rect.bottom) {
			outPoint.x += 200;
			outPoint.y = rect.top + 20;
		}
	}

	EndPaint(hWnd, &ps);

	return 0;
}

// Process the bytes representing a character's glyph from left to right
// instead of right to left.
// Bits of each byte are processed Least Significant (0x01) to Most Significant (0x80).

int paintFontDisplay3(HWND hWnd, unsigned short iFirst, unsigned short iLast, unsigned short iHeight, unsigned short iWidth = 8)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT rect;
	GetClientRect(hWnd, &rect);

	//Logical units are device dependent pixels, so this will create a handle to a logical font that is 48 pixels in height.
	//The width, when set to 0, will cause the font mapper to choose the closest matching value.
	//The font face name will be Impact.
	HFONT hFont = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH, TEXT("Courier"));
	SelectObject(hdc, hFont);

	POINT outPoint;
	outPoint.x = rect.left + 80;
	outPoint.y = rect.top + 20;

	// handle the case if the raster font is wider than 8 pixels.
	// if it is then we will need to use two bytes for each line
	// of the glyph rather than one byte for each line of the glyph.
	if (iWidth > 8) iHeight *= 2;
	for (unsigned short i = iFirst; i < iLast; i++) {
		for (unsigned short j = 0; j < iHeight; j++) {
			std::wstring charRep;
			for (unsigned char k = 0x1; k; k <<= 1) {
				if (fontFileBuffer[i * iHeight + j] & k) {
					charRep += '*';
				}
				else {
					charRep += '-';
				}
			}
			if (iWidth > 8) {
				// if the width is greater than 8 pixels then we need to
				// continue drawing this line with the next byte.
				j++;
				for (unsigned char k = 0x01; k; k <<= 1) {
					if (fontFileBuffer[i * iHeight + j] & k) {
						charRep += '*';
					}
					else {
						charRep += '-';
					}
				}
			}
			TextOut(hdc, outPoint.x, outPoint.y, charRep.c_str(), charRep.length());
			outPoint.y += 20;
		}
		outPoint.y += 20;
		if (outPoint.y + 20 >= rect.bottom) {
			outPoint.x += 200;
			outPoint.y = rect.top + 20;
		}
	}

	EndPaint(hWnd, &ps);

	return 0;
}

// Process the bytes representing a character's glyph from right to left
// instead of left to right.
// Bits of each byte are processed Most Significant (0x80) to Least Significant (0x01).

int paintFontDisplay4(HWND hWnd, unsigned short iFirst, unsigned short iLast, unsigned short iHeight, unsigned short iWidth = 8)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT rect;
	GetClientRect(hWnd, &rect);

	//Logical units are device dependent pixels, so this will create a handle to a logical font that is 48 pixels in height.
	//The width, when set to 0, will cause the font mapper to choose the closest matching value.
	//The font face name will be Impact.
	HFONT hFont = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH, TEXT("Courier"));
	SelectObject(hdc, hFont);

	POINT outPoint;
	outPoint.x = rect.left + 80;
	outPoint.y = rect.top + 20;

	// handle the case if the raster font is wider than 8 pixels.
	// if it is then we will need to use two bytes for each line
	// of the glyph rather than one byte for each line of the glyph.
	if (iWidth > 8) iHeight *= 2;
	for (unsigned short i = iFirst; i < iLast; i++) {
		for (unsigned short j = iHeight; j > 0; j--) {
			std::wstring charRep;
			for (unsigned char k = 0x80; k; k >>= 1) {
				if (fontFileBuffer[i * iHeight + j - 1] & k) {
					charRep += '*';
				}
				else {
					charRep += '-';
				}
			}
			if (iWidth > 8) {
				// if the width is greater than 8 pixels then we need to
				// continue drawing this line with the next byte.
				j--;
				for (unsigned char k = 0x80; k; k >>= 1) {
					if (fontFileBuffer[i * iHeight + j - 1] & k) {
						charRep += '*';
					}
					else {
						charRep += '-';
					}
				}
			}
			TextOut(hdc, outPoint.x, outPoint.y, charRep.c_str(), charRep.length());
			outPoint.y += 20;
		}
		outPoint.y += 20;
		if (outPoint.y + 20 >= rect.bottom) {
			outPoint.x += 200;
			outPoint.y = rect.top + 20;
		}
	}

	EndPaint(hWnd, &ps);

	return 0;
}

HRESULT OpenFile(PCWSTR pszPath)
{
	HANDLE   hFile = INVALID_HANDLE_VALUE;
	HRESULT  hr = HRESULT_FROM_WIN32(0);
	DWORD    dwBytesRead = 0;

	// The lack of FILE_SHARE_READ or FILE_SHARE_WRITE attributes for the dwShareMode
		// parameter will cause the file to be locked from other processes.
	hFile = CreateFile(pszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		ReadFile(hFile, fontFileBuffer, sizeof(fontFileBuffer), &dwBytesRead, NULL);
		CloseHandle(hFile);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	return hr;
}

typedef int(*DisplayFunc) (HWND hWnd, unsigned short iFirst, unsigned short iLast, unsigned short iHeight, unsigned short iWidth);

static DisplayFunc myFunc = paintFontDisplay;

unsigned short  usFontHeight = 16;
unsigned short  usFontWidth = 8;
unsigned short  usFirstChar = 32;
unsigned short  usLastChar = 40;
bool            bRotateChar = false;
bool            bLeast_2_MostChar = false;
bool            bReverseByteOrder = false;


int paintFontDisplaySample(HWND hWnd, unsigned short iFirst, unsigned short iLast, FontSample &fSample)
{
	DisplayFunc pFunc = paintFontDisplay2;

	if (fSample.font_table.bFlags & FontSample::FontTable::Flags_InvertBitOrder) pFunc = paintFontDisplay3;

	memcpy(fontFileBuffer, fSample.font_table.table, sizeof(fontFileBuffer));
	return pFunc(hWnd, iFirst, iLast, fSample.font_table.nCols, 8);
}

// Message handler for font file properties dialog.
INT_PTR CALLBACK FontFile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, usFontHeight, FALSE);
		SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, usFontWidth, FALSE);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				if (!GetDlgItemText(hDlg, IDC_EDIT_FILE, szFontFileName, 80))
					*szFontFileName = 0;
				usFontHeight = GetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, NULL, FALSE);
				usFontWidth = GetDlgItemInt(hDlg, IDC_EDIT_WIDTH, NULL, FALSE);
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for font file properties dialog.
INT_PTR CALLBACK FontFileProperties(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_EDIT_FIRSTCHAR, usFirstChar, FALSE);
		SetDlgItemInt(hDlg, IDC_EDIT_LASTCHAR, usLastChar, FALSE);
		SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, usFontHeight, FALSE);
		SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, usFontWidth, FALSE);
		CheckDlgButton(hDlg, IDC_CHECK_ROTATE, (bRotateChar ? 1 : 0));
		CheckDlgButton(hDlg, IDC_CHECK_LEAST_2_MOST, (bLeast_2_MostChar ? 1 : 0));
		CheckDlgButton(hDlg, IDC_CHECK_REVERSE_BYTE_ORDER, (bReverseByteOrder ? 1 : 0));
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK) {
				usFirstChar = GetDlgItemInt(hDlg, IDC_EDIT_FIRSTCHAR, NULL, FALSE);
				usLastChar = GetDlgItemInt(hDlg, IDC_EDIT_LASTCHAR, NULL, FALSE);
				usFontHeight = GetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, NULL, FALSE);
				usFontWidth = GetDlgItemInt(hDlg, IDC_EDIT_WIDTH, NULL, FALSE);
				bRotateChar = IsDlgButtonChecked(hDlg, IDC_CHECK_ROTATE) == BST_CHECKED;
				bLeast_2_MostChar = IsDlgButtonChecked(hDlg, IDC_CHECK_LEAST_2_MOST) == BST_CHECKED;
				bReverseByteOrder = IsDlgButtonChecked(hDlg, IDC_CHECK_REVERSE_BYTE_ORDER) == BST_CHECKED;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_FILE_OPENFILE:
				{
					wchar_t  szPathName[256] = { 0 };

					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, FontFile);
					if (szFontFileName[0]) {
						wcscpy_s (szPathName, 255, L"C:\\Users\\rickc\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\fontshow\\");
						wcscat_s(szPathName, 255, szFontFileName);
						OpenFile(szPathName);

						// Default setting to be modified in the WM_PAINT handler
						myFunc = paintFontDisplay2;
					}
					InvalidateRect(hWnd, NULL, 1);
				}
				break;
			case IDM_FILE_SETPROPERTIES:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, FontFileProperties);
				InvalidateRect(hWnd, NULL, 1);
				break;
			case IDM_FILE_TABLE_COMPARE:
				myFunc = paintFontDisplay;
				usFirstChar = 32;
				usLastChar = 40;
				bRotateChar = false;
				bLeast_2_MostChar = false;
				bReverseByteOrder = false;
				InvalidateRect(hWnd, NULL, 1);
				break;
			case IDM_FILE_TABLE_5_COL:
				memcpy(fontFileBuffer, font_table_5_col, sizeof(fontFileBuffer));
				// Default setting to be modified in the WM_PAINT handler
				myFunc = paintFontDisplay2;
				bRotateChar = false;
				bLeast_2_MostChar = false;
				bReverseByteOrder = false;
				usFontHeight = 5;
				usFontWidth = 8;
				InvalidateRect(hWnd, NULL, 1);
				break;
			case IDM_FILE_TABLE_7_COL:
				memcpy(fontFileBuffer, font_table_7_col, sizeof(fontFileBuffer));
				// Default setting to be modified in the WM_PAINT handler
				myFunc = paintFontDisplay2;
				bRotateChar = false;
				bLeast_2_MostChar = false;
				bReverseByteOrder = false;
				usFontHeight = 7;
				usFontWidth = 8;
				InvalidateRect(hWnd, NULL, 1);
				break;
			case IDM_FILE_TABLE_8_COL:
				memcpy(fontFileBuffer, font_table_8_col, sizeof(fontFileBuffer));
				// Default setting to be modified in the WM_PAINT handler
				myFunc = paintFontDisplay2;
				bRotateChar = false;
				bLeast_2_MostChar = false;
				bReverseByteOrder = false;
				usFontHeight = 8;
				usFontWidth = 8;
				InvalidateRect(hWnd, NULL, 1);
				break;
			case IDM_FILE_TABLE_13_COL:
				memcpy(fontFileBuffer, font_table_13_col, sizeof(fontFileBuffer));
				// Default setting to be modified in the WM_PAINT handler
				myFunc = paintFontDisplay2;
				bRotateChar = false;
				bLeast_2_MostChar = false;
				bReverseByteOrder = false;
				usFontHeight = 13;
				usFontWidth = 8;
				InvalidateRect(hWnd, NULL, 1);
				break;
			case IDM_FILE_TABLE_16_COL:
				memcpy(fontFileBuffer, font_table_16_col, sizeof(fontFileBuffer));
				// Default setting to be modified in the WM_PAINT handler
				myFunc = paintFontDisplay2;
				bRotateChar = false;
				bLeast_2_MostChar = false;
				bReverseByteOrder = false;
				usFontHeight = 16;
				usFontWidth = 8;
				InvalidateRect(hWnd, NULL, 1);
				break;
			case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
		//   bLeast_2_MostChar is true so process from LSB to MSB.
		//   Process bytes of character from left to right.
		if (bLeast_2_MostChar) myFunc = paintFontDisplay3;

		//   bReverseByteOrder is true so process from bytes of character from right to left
		//   rather than left to right.
		//   Process bits of each character from MSB to LSB.
		if (bReverseByteOrder) myFunc = paintFontDisplay4;

		myFunc(hWnd, usFirstChar, usLastChar, usFontHeight, usFontWidth);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct {
	char* first;   // return dest
	char* last;    // return dest + strlen(dest)
} StrRet;

typedef struct {
	char* sSrc;        // return dest
	char* sMatch;      // return dest + strlen(dest)
	char* sMatchLast;  // sMatch + strlen(sMatch)
} StrRetMatch;

#if defined(__cplusplus)
extern "C" {
#endif
	StrRet strcpyret(char* dest, const char* src);
	StrRet strcpysliceret(char* dest, const char* src, int iFirst, int iLast);
	StrRet stridsliceret(char* src, int iFirst, int iLast);
	StrRet strcpypullret(char* dest, const char* src, short aFirst, short aLast, short opCode);
	StrRet strinsertret(char* dest, const char* srcFirst, const char* srcLast);
	StrRetMatch strmatch(const char* regexp, const char* src);
#if defined(__cplusplus)
};
#endif


/*
 *  strcpyret() copy a text string.
**/
StrRet strcpyret(char* dest, const char* src)
{
	StrRet x = { dest, dest };

	while ((*dest = *src++)) dest++;

	x.last = dest;

	return x;
}

/*
 *  strcpysliceret() copy a slice of a text string. The slice is
 *  indicated by two indexes, iFirst the beginning of the slice as
 *  a zero based index to the first text character and iLast the
 *  end of the slice as a zero based index to the last text character
 *  plus 1.
 *
 *  The range is src[iFirst::iLast] and the text from iFirst up to but
 *  not including iLast is copied from the source to the destination.
 *
 *  If iFirst == iLast then the number of characters is zero.
 *  If iFirst < iLast then the number of characters is iLast - iFirst
 *
 *  If iFirst is negative then it is a zero based offset from the end of the string.
 *  If iLast is negative then it is a zero based offset from the end of the string.
 *  If iFirst is negative and iLast is zero then iLast becomes strlen() + 1.
**/
StrRet strcpysliceret(char* dest, const char* src, int iFirst, int iLast)
{
	StrRet x = { dest, dest };

	if (iFirst < 0 || iLast < 0) {
		int iLen = strlen(src);

		if (iFirst < 0 && iLast == 0) iLast = iLen + 1;
		if (iFirst < 0) iFirst += iLen;
		if (iLast < 0) iLast += iLen;
	}

	if (iFirst >= 0 && iFirst < iLast) {
		const char* srcLast = src + iLast;
		src += iFirst;
		while (src < srcLast && (*dest = *src++)) dest++;
	}

	*dest = 0;
	x.last = dest;
	return x;
}

/*
 *  stridsliceret() identify a slice of a text string. The slice is
 *  indicated by two indexes, iFirst the beginning of the slice as
 *  a zero based index to the first text character and iLast the
 *  end of the slice as a zero based index to the last text character
 *  plus 1.
 *
 *  The range is src[iFirst::iLast] and the text from iFirst up to but
 *  not including iLast is copied from the source to the destination.
 *
 *  If iFirst == iLast then the number of characters is zero.
 *  If iFirst < iLast then the number of characters is iLast - iFirst
 *
 *  If iFirst is negative then it is a zero based offset from the end of the string.
 *  If iLast is negative then it is a zero based offset from the end of the string.
 *  If iFirst is negative and iLast is zero then iLast becomes strlen() + 1.
**/
StrRet stridsliceret(char* src, int iFirst, int iLast)
{
	StrRet x = { src, src };

	if (iFirst < 0 || iLast < 0) {
		int iLen = strlen(src);

		if (iFirst < 0 && iLast == 0) iLast = iLen + 1;
		if (iFirst < 0) iFirst += iLen;
		if (iLast < 0) iLast += iLen;
	}

	if (iFirst >= 0 && iFirst < iLast) {
		const char* srcLast = src + iLast;
		src += iFirst;
		x.first = src;
		while (src < srcLast && *src) src++;
	}

	x.last = src;
	return x;
}

StrRet strcpypullret(char* dest, const char* src, short aFirst, short aLast, short opCode)
{
	StrRet x = { dest, dest };

	while (*src && *src != aFirst) src++;

	while ((*dest = *src++) && *dest != aLast) dest++;

	*dest = 0;
	x.last = dest;
	return x;
}

StrRet strinsertret(char* dest, const char* srcFirst, const char* srcLast)
{
	StrRet x = { dest, dest };

	int iLen = strlen(dest);

	int iInsert = srcLast - srcFirst;
	if (*srcLast) iInsert++;

	if (iInsert > 0) {
		char* destEnd = dest + iLen + iInsert;
		char* destFront = dest + iLen;

		while (destFront >= dest) *destEnd-- = *destFront--;

		while (srcFirst <= srcLast && *srcFirst) *dest++ = *srcFirst++;

		x.last = dest;
	}

	return x;
}


static const char* matchstar(int c, const char* regexp, const char* text);

/* matchhere: search for regexp at beginning of text */
static const char* matchhere(const char* regexp, const char* text)
{
	if (regexp[0] == '\0')
		return text;
	if (regexp[1] == '*')
		return matchstar(regexp[0], regexp + 2, text);
	if (regexp[0] == '$' && regexp[1] == '\0')
		return (*text == '\0') ? text : NULL;
	if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
		return matchhere(regexp + 1, text + 1);
	return NULL;
}

/* matchstar: search for c*regexp at beginning of text */
static const char* matchstar(int c, const char* regexp, const char* text)
{
	do {    /* a * matches zero or more instances */
		if (matchhere(regexp, text))
			return text;
	} while (*text != '\0' && (*text++ == c || c == '.'));
	return NULL;
}

/* match: search for regexp anywhere in text */
StrRetMatch strmatch(const char* regexp, const char* src)
{
	StrRetMatch x = { src, src, 0 };

	if (regexp[0] == '^') {
		x.sMatch = matchhere(regexp + 1, src);
		return x;
	}
	do {    /* must look even if string is empty */
		if (matchhere(regexp, src)) {
			x.sMatch = src;
			return x;
		}
	} while (*src++ != '\0');
	return x;
}



int main_Source_02(void)
{
    char x1[] = "01234567890ABCDEF";

    char y1[64];

    StrRet xRet;

	printf("   strcpysliceret(y1, x1, 1, 5).first  %s\n", strcpysliceret(y1, x1, 1, 5).first);
	printf("   strcpysliceret(y1, x1, 1, 1).first  %s\n", strcpysliceret(y1, x1, 1, 1).first);
	printf("   strcpysliceret(y1, x1, 6, 8).first  %s\n", strcpysliceret(y1, x1, 6, 8).first);
	printf("   strcpysliceret(y1, x1, -4, 0).first  %s\n", strcpysliceret(y1, x1, -4, 0).first);
	printf("   strcpysliceret(y1, x1, -4, -1).first  %s\n", strcpysliceret(y1, x1, -4, -1).first);
	printf("   strcpysliceret(y1, x1, 1, -1).first  %s\n", strcpysliceret(y1, x1, 1, -1).first);

    strcpyret(strcpysliceret(y1, x1, 1, -1).last, "__end");
	printf("  strcpyret(strcpysliceret(y1, x1, 1, -1).last, \"__end\")  y1 %s\n", y1);
    strcpypullret(y1, x1, '5', 'A', 0);
	printf("  strcpypullret(y1, x1, '5', 'A', 0)  y1 %s\n", y1);
    strcpypullret(y1, x1, '5', 'G', 0);
	printf("  strcpypullret(y1, x1, '5', 'G', 0)  y1 %s\n", y1);
    strcpypullret(y1, x1, 'X', 'G', 0);
    printf("  strcpypullret(y1, x1, 'X', 'G', 0)  y1 %s\n", y1);

    xRet = stridsliceret(x1, -5, -1);

    strcpy(y1, "abcdefghijk");

    strinsertret(y1 + strlen(y1) - 1, xRet.first, xRet.last);
    printf("  strinsertret(y1 + strlen(y1) - 1, xRet.first, xRet.last)  y1  %s\n", y1);

    StrRetMatch xRet2 = strmatch("a*r", "xyzaaaaaaaabcjjj");

    printf("strmatch(\"a*r\", \"xyzaaaaaaaabcjjj\");  \"%s\"    \"%s\"\n", xRet2.sSrc, xRet2.sMatch);

    return 0;
}

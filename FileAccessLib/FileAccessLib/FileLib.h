
/*
	Header file for FileLib.cpp which contains a set of functions that implement a simple, lightweight
	indexed file library for C or C++ applications.

	Author:  Richard Chambers
	Date:    Dec-30-2011

	Copyright:  Richard Chambers
	Licensing:  The Code Project license (http://www.codeproject.com/)

	
**/

// struct for use with function FileLibReadHeader() to allow user of this library to request
// some of the file management data from a file.  The user can not use this to change the
// file management data, only to inspect the data.
typedef struct {
	int nMaxRecordCount;        // maximum number of records that can be inserted into the file
	int nCurrentRecordCount;    // current number of records that has been inserted into the file
} FileLibFileInfo;


// struct for an iterator used with function FileLibIterate().  See initializer function that follows.
typedef struct {
	long           nIndexNumber;     // current index
	unsigned long  fulProcessFlags;  // indicates the current status of the iteration
} FileLibIterator;

// inline function to initialize an iterator before using it with function FileLibIterate().
inline void FileLibIteratorInit (FileLibIterator &AnIterator) { AnIterator.nIndexNumber = 0; AnIterator.fulProcessFlags = 0; }


// following function FileLibcheckFile() may not be compiled into the function library
// depending on whether it is desire and compatible functionality or not.
//
// to use this function, it must be turned on in the function library and then
// an array needs to be provided for a place to keep data needed during the
// checking procedure.
typedef struct {
	long  nIndexNumber;              // the index offset as a relative count
	unsigned long  nRecordOffset;    // the actual file offset for the record of the index
	unsigned long  fulProcessFlags;  // indicates the current status of the index
} FileLibFileCheckBuffer;
int FileLibCheckFile (char *aszFileName, FileLibFileCheckBuffer *aCheckBuf, int nCheckBufCount);

// -----------------      library function prototypes      -----------------------------------------
// The folowing function prototypes are the interface into the library.
// All except one of the functions have two versions, a file handle version and a file name version.
// The purpose of the file handle version is to allow a user to open the file and to then use
// the file handle in order to eliminate the additional overhead of opening and closing the file
// each time the file is accessed.
int FileLibCreate (char *aszFileName, int nRecords, int nIndexSize, int nRecordSize, int nHeaderSize);

int FileLibReadHeaderFh (FILE *hFile, void *pHeader, FileLibFileInfo *pFileInfo);
int FileLibReadHeader (char *aszFileName, void *pHeader, FileLibFileInfo *pFileInfo);

int FileLibWriteHeaderFh (FILE *hFile, void *pHeader);
int FileLibWriteHeader (char *aszFileName, void *pHeader);

int FileLibInsertFh (FILE *hFile, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));
int FileLibInsert (char *aszFileName, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));

int FileLibRetrieveFh (FILE *hFile, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));
int FileLibRetrieve (char *aszFileName, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));

int FileLibDeleteFh (FILE *hFile, void *pIndex, int (*pFunc)(void *pIndex1, void *pIndex2));
int FileLibDelete (char *aszFileName, void *pIndex, int (*pFunc)(void *pIndex1, void *pIndex2));

int FileLibUpdateFh (FILE *hFile, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));
int FileLibUpdate (char *aszFileName, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));

int FileLibIterateFh (FILE *hFile, FileLibIterator *pIterator, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));
int FileLibIterate (char *aszFileName, FileLibIterator *pIterator, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2));

// --------    Function return value constants          ---------------------
// The following constants are used to indicate various error conditions.
// These are all negative numbers allowing a quick check for an error by checking
// if the function returned a negative number or not.
const int FileLibStatus_FileNotFound = -1;
const int FileLibStatus_FileInvalid = -2;
const int FileLibStatus_FileFull = -3;
const int FileLibStatus_RecordNotFound = -4;
const int FileLibStatus_RecordExistsInsertFail = -5;
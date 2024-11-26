
/*
	Source code file for FileLib.h which contains a set of functions that implement a simple, lightweight
	indexed file library for C or C++ applications.

	Author:  Richard Chambers
	Date:    Dec-30-2011

	Copyright:  Richard Chambers
	Licensing:  The Code Project license (http://www.codeproject.com/)

	
**/

// following define required by Microsoft Visual Studio 2005
#include "stdafx.h"

// Include the Standard C Library headers needed for some of the functionality.
// We are using the Standard C Library file I/O primitives so the stdio.h file must
// be included.  And we are using the memory primitives to move data so the
// memory.h file must also be included.
#include <stdio.h>
#include <memory.h>

// include the function prototypes and exported data types that these functions use
// to allow compiler to check interfaces.
#include "FileLib.h"


// Following define allows for changing the algorithm used for index inserts and deletes
// to do it one index record per file access or to use a buffer that does multiple index
// records at a time to help improve throughput by reducing file access overhead.
//#define RECORD_AT_TIME  1

// Following define indicates if the function FileLibCheckFile()
// will be provided by the library or not.  This function will perform
// a check of a file to see if it is consistent or not.
//#define PROVIDE_FILELIBCHECKFILE  1


typedef unsigned long FileLibRecordOffset;                // type for the actual byte offset within the file

typedef unsigned char FileLibIndexKey [384];              // byte array for holding the user's index data

const unsigned long FileLibFileSignature = 0xf01a2b3c;    // set of bytes acting as a signature to help determine if file is a valid file

const long FileLibSearchInvalidFile = 0x7fffff;           // error code used with internal function FileLibSearch() to indicate a problem with file

const int  FileLibWindowBufferSize = 2048;

// The file header contains the file content management information we need
// in order to insert, retrieve, and delete records from this file.  Because
// different files will contain different types of objects with different
// structures, we do not want to need to know the internal structure of these
// various objects.  We treat all objects in a file as being of the same type
// of object with the same size.
typedef struct {
	unsigned long ulSignature1;               // special, identifying signature to check this is our type of file
	FileLibRecordOffset ulRecordAreaOffset;   // offset to where the record data is stored in the file
	long nMaxRecords;             // maximum number of records to be stored in the file, also max of indexes
	long nRecordSize;             // number of bytes in a record.  this determines number of bytes stored/retrieved
	long nIndexSize;              // number of bytes of user index data to a record.  this is user index data only
	long nStoredIndexSize;        // number of bytes total in an index to a record.  user index data plus file management data
	long nHeaderSize;             // number of bytes of user header data.  This allows user to have their own file management information
	long nRecordCount;            // current number of records that are stored in the file.  also number of indexes
	unsigned long ulSignature2;               // special, identifying signature to check this is our type of file.
} FileLibHeader;

// The actual index information that is stored contains a file management section
// which is just the block number associated with the index, and the user index data
// area which contains the index information provided by the user.  This data can serve
// several purposes however it's main purpose is to provide a way to order the indexes
// so that the records can be found and retrieved or deleted.
typedef struct {
	long              nRecordBlockNumber;   // block number of the record corresponding to this index
	FileLibIndexKey   uchIndex;             // index data from the user
} FileLibIndex;


long FileLibSearch (FILE *hFile, void *pIndex, long *pnRecordBlockNumber, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	FileLibHeader  MyFileHeader;
	FileLibIndex   MyIndex;
	long           nIndexBlockNumber = 1;

	// read in the file header so that we have the various sizing data we
	// need for our file offset calculations.
	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 != FileLibFileSignature || MyFileHeader.ulSignature2 != FileLibFileSignature) {
		return FileLibSearchInvalidFile;
	}

	if (MyFileHeader.nMaxRecords < 1)
		return FileLibSearchInvalidFile;

	if (MyFileHeader.nRecordCount < 1 && MyFileHeader.nMaxRecords > 0)
		return -1;    // indicate that this is record number 1 and that a record does not exist yet.

	// calculate the file offset to where the index area begins
	FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;

	// Create the sequential search buffer that we will use to just read in a section of the index area
	// of the file and do a sequential search.  We start with doing a binary search by probing records
	// from the file until we have a range of indices that will fit in to the sequential search buffer.
	// At that point we switch from binary search to sequential search to eliminate file access overhead.
	unsigned char  IndexSequentialSearchBuffer[1048];
	long           nCountIndexSequentialSearchBuffer = sizeof(IndexSequentialSearchBuffer)/MyFileHeader.nStoredIndexSize;

	// perform a binary search to locate the index specified.
	// we will return a positive number indicating the index number if found.
	// we will return a negative number indicating the index number if not found.
	// the negative number indicates the position for an insertion.
	int iCmp = 0;   // indicates the result of the comparison function used for the binary search

	long nIndexBlockNumberMax = MyFileHeader.nRecordCount;
	long nIndexBlockNumberMin = 1;

	// if the indexes from index number minimum up to and including index number maximum will fit
	// into the memory buffer then just do sequential search otherwise do a binary search.
	while (nIndexBlockNumberMax - nIndexBlockNumberMin >= nCountIndexSequentialSearchBuffer) {
		nIndexBlockNumber = ((nIndexBlockNumberMax - nIndexBlockNumberMin) / 2) + nIndexBlockNumberMin;
		fseek (hFile, (nIndexBlockNumber - 1)*MyFileHeader.nStoredIndexSize + nOffsetIndex, SEEK_SET);
		fread (&MyIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
		iCmp = pFunc (pIndex, MyIndex.uchIndex);
		if (iCmp == 0) {
			// found the index so lets return the index number if the caller has
			// provided the address to put the index number of the block where the
			// record data corresponding to this index value is located.
			if (pnRecordBlockNumber) *pnRecordBlockNumber = MyIndex.nRecordBlockNumber;
			return nIndexBlockNumber;
		} else if (iCmp < 0) {
			// the index being searched for is below the index we found at this file location
			// so we set our maximum for the next search range to the current block number so that
			// our next search will be in the lower part of the current range.
			nIndexBlockNumberMax = nIndexBlockNumber;
		} else {
			// the index being searched for is above the index we found at this file location
			// so we set our minimum for the next search range to the current block number so that
			// our next search will be in the upper part of the current range.
			nIndexBlockNumberMin = nIndexBlockNumber;
		}
	}

	// We have not found the index while doing the binary search and are now within the range that
	// we will change from the binary search to just a straight sequential search.  
	fseek (hFile, (nIndexBlockNumberMin - 1)*MyFileHeader.nStoredIndexSize + nOffsetIndex, SEEK_SET);
	fread (IndexSequentialSearchBuffer, MyFileHeader.nStoredIndexSize, nCountIndexSequentialSearchBuffer, hFile);
	FileLibIndex  *pMyIndex = (FileLibIndex *)&IndexSequentialSearchBuffer[0];
	iCmp = -1;
	for (nIndexBlockNumber = nIndexBlockNumberMin; nIndexBlockNumber <= nIndexBlockNumberMax; nIndexBlockNumber++) {
		iCmp = pFunc (pIndex, pMyIndex->uchIndex);
		if (iCmp <= 0) {
			break;
		}
		pMyIndex = (FileLibIndex *)(((unsigned char *)pMyIndex) + MyFileHeader.nStoredIndexSize);
	}

	// If we have found the index then we will return the index block number as a positive value.
	// If we did not find the index then we are returning the index block number as a negative value
	// indicating that the index was not found however we do know where it needs to be inserted
	// if the caller wants to insert it.
	// We will return this index as an index offset that is one based.
	// so one means the first index, two means the second, etc.
	if (pnRecordBlockNumber) *pnRecordBlockNumber = pMyIndex->nRecordBlockNumber;
	if (iCmp != 0 || nIndexBlockNumber > nIndexBlockNumberMax) nIndexBlockNumber *= -1;

	return nIndexBlockNumber;
}
	// ===============================================

/*
	FileLibCreate () is the function that is used to create and initialize the file we are
	going to use for a particular object.  The caller of this function has to provide a
	maximum number of records the file will contain since the index for the records is
	included in the file contents.

	Returns:	Success: 1
				Unsuccessful: 0
				Index too large: -1

	This function returns a value of one (1) if successful.  If unsuccessful then either
	a value of zero (0) will be returned in the general case or a negative one (-1) in the
	specific failure of if the index record size exceeds the internal index buffer.

	The structure of the file created is:
	  - file management header containing information like number of records
	  - user file content header containing user defined data like a version number (optional)
	  - index section that contains all of the index records
	  - record section that contains all of the record data (this is actually optional)
**/
int FileLibCreate (char *aszFileName, int nRecords, int nIndexSize, int nRecordSize, int nHeaderSize)
{
	FileLibHeader  MyFileHeader;
	FileLibIndex   tempbufIndex;
	int            iStatus = 0;    // assume we are not successful

	// Set the signatures to allow a check that the file has been initialized
	MyFileHeader.ulSignature1 = FileLibFileSignature;
	MyFileHeader.ulSignature2 = FileLibFileSignature;

	// Set the file management data.  This is the data we need to know about the
	// various sizes of index and record as well as the maximum number of records for the file.
	// We are using a stored index size which includes the block number for the record associated
	// with the index so the stored index size is the size of the stored index as well as the
	// size of the block number.
	// We also allow the user to have their own header information that is stored with the file.
	// This user header information is not physical file information but is information that the
	// user of this file wants to have to apply to the information in the file.
	MyFileHeader.nMaxRecords = nRecords;
	MyFileHeader.nRecordSize = nRecordSize;
	MyFileHeader.nIndexSize = nIndexSize;
	MyFileHeader.nStoredIndexSize = sizeof(FileLibIndex) - sizeof(tempbufIndex.uchIndex) + nIndexSize;
	MyFileHeader.nHeaderSize = nHeaderSize;
	MyFileHeader.nRecordCount = 0;

	if (MyFileHeader.nIndexSize > sizeof(FileLibIndexKey)) {
		return -1;   // if the index record size is larger than our internal buffer then not allowed.
	}

	// Calculate the offset to where the stored records begins.  In order to properly accomodate the
	// fact that the index area will have an insertion sorted list of indexes and that we will be moving
	// indexes, we will add one more index than maximum number of records to the index area.  This
	// will make our logic easier as we will not have to worry about a special case for the last
	// index to be added thereby filling up the index space to make sure we do not run into the
	// record storage area when moving indexes during inserts and/or deletes.
	MyFileHeader.ulRecordAreaOffset = sizeof(MyFileHeader) + nHeaderSize + MyFileHeader.nStoredIndexSize * (nRecords + 1);

	FILE *hFile = fopen (aszFileName, "wb");

	FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;
	FileLibRecordOffset nOffsetRecords = nOffsetIndex + MyFileHeader.nIndexSize * MyFileHeader.nMaxRecords;

	if (hFile) {
		// create a temporary user header area, zero it out for use when we init the
		// file.  we are only going to init the file management header area, the user
		// header area, and the index area of the file.  The record area will be filled
		// in as records are actually added.
		fseek (hFile, 0, SEEK_SET);
		fwrite (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

		unsigned char   tempbuf[512];
		memset (tempbuf, 0, sizeof(tempbuf));
		if (nHeaderSize > 0)
			fwrite (tempbuf, nHeaderSize, 1, hFile);

		// Now we will initialize the index area of the file.  Each index that is stored has two
		// parts, the physical file record block index number which is not seen by the user of
		// this function package and the user index containing the data the user is putting into
		// each index in order to access the record data associated with the index.  When
		// doing file actions such as insert, delete, retrieve, or update the user will specify
		// the index along with a comparison function that we will use to find the specified
		// index as well as the data associated with the index.
		memset (&tempbufIndex, 0, sizeof(tempbufIndex));

		fseek (hFile, nOffsetIndex, SEEK_SET);
		tempbufIndex.nRecordBlockNumber = 1;
		for (long iLoop = 1; iLoop <= MyFileHeader.nMaxRecords; iLoop++) {
			fwrite (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
			tempbufIndex.nRecordBlockNumber++;
		}
		fclose (hFile);
		iStatus = 1;       // indicate we were successful
	}

	return iStatus;
}
	// -----------------------------------------------

/*
	FileLibReadHeaderFh () reads the user provided header information and also provides some
	information about the file such as the maximum number of records and the current number of
	records in the file.

	The user provided header will contain the same information as was written to the file by
	the function FileLibWriteHeaderFh().  We do not allow the user to change the file management
	data such as the maximum number of records so the FileLibFileInfo is read only data.
**/
int FileLibReadHeaderFh (FILE *hFile, void *pHeader, FileLibFileInfo *pFileInfo)
{
	FileLibHeader  MyFileHeader;
	int            iStatus = 0;    // assume not successful

	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 == FileLibFileSignature && MyFileHeader.ulSignature2 == FileLibFileSignature) {

		// If the user has specified a physical file info buffer then provide the
		// physical file information to them.
		if (pFileInfo) {
			iStatus = 1;    // indicate success
			pFileInfo->nCurrentRecordCount = MyFileHeader.nRecordCount;
			pFileInfo->nMaxRecordCount = MyFileHeader.nMaxRecords;
		}

		if (pHeader && MyFileHeader.nHeaderSize > 0) {
			iStatus = 1;    // indicate success
			fread (pHeader, MyFileHeader.nHeaderSize, 1, hFile);
		}
	} else {
		iStatus = FileLibStatus_FileInvalid;
	}

	return iStatus;
}

int FileLibReadHeader (char *aszFileName, void *pHeader, FileLibFileInfo *pFileInfo)
{
	int  iStatus = FileLibStatus_FileNotFound;

	FILE *hFile = fopen (aszFileName, "rb");

	if (hFile) {
		iStatus = FileLibReadHeaderFh (hFile, pHeader, pFileInfo);
		fclose (hFile);
	}

	return iStatus;
}

/*
	FileLibWriteHeaderFh () writes the user provided header information into the file.  This
	data could be anything the user of the file considers valuable such as version information.

	The user provided header will contain the same information as can be read from the file by
	the function FileLibReadHeaderFh().  We do not allow the user to change the file management
	data such as the maximum number of records so the FileLibFileInfo is read only data.
**/
int FileLibWriteHeaderFh (FILE *hFile, void *pHeader)
{
	FileLibHeader  MyFileHeader;
	int            iStatus = 0;    // assume not successful

	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 == FileLibFileSignature && MyFileHeader.ulSignature2 == FileLibFileSignature) {
		if (pHeader && MyFileHeader.nHeaderSize > 0) {
			iStatus = 1;    // indicate success
			fseek (hFile, sizeof(FileLibHeader), SEEK_SET);
			fwrite (pHeader, MyFileHeader.nHeaderSize, 1, hFile);
		}
	} else {
		iStatus = FileLibStatus_FileInvalid;
	}

	return iStatus;
}

int FileLibWriteHeader (char *aszFileName, void *pHeader)
{
	int  iStatus = FileLibStatus_FileNotFound;

	FILE *hFile = fopen (aszFileName, "r+b");

	if (hFile) {
		iStatus = FileLibWriteHeaderFh (hFile, pHeader);
		fclose (hFile);
	}

	return iStatus;
}
	// -----------------------------------------------

int FileLibInsertFh (FILE *hFile, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	int            iStatus = FileLibStatus_FileNotFound;   // default is to indicate an error
	FileLibHeader  MyFileHeader;

	iStatus = 0;    // indicate that we were able to open the file though not yet an insert

	// Read the file header which contains the management data
	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 != FileLibFileSignature || MyFileHeader.ulSignature2 != FileLibFileSignature) {
		return FileLibStatus_FileInvalid;
	}

	if (MyFileHeader.nRecordCount >= MyFileHeader.nMaxRecords) {
		return FileLibStatus_FileFull;
	}

	FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;

	long nBlockNumber = 0;
	if (MyFileHeader.nRecordCount < 1) {
		nBlockNumber = -1;                // we will use the first index and this is a new record
		MyFileHeader.nRecordCount = 0;    // ensure that record count is zero and not negative for some reason.
	} else {
		nBlockNumber = FileLibSearch (hFile, pIndex, 0, pFunc);
		if (nBlockNumber == FileLibSearchInvalidFile)
			return FileLibStatus_FileInvalid;
	}

	bool bNewRecord = (nBlockNumber < 0);
	if (nBlockNumber < 0) nBlockNumber *= -1;

	if (!bNewRecord) {
		iStatus = FileLibStatus_RecordExistsInsertFail;
	} else {
		FileLibRecordOffset nOffsetInsert = nOffsetIndex + (nBlockNumber - 1) * MyFileHeader.nStoredIndexSize;
		FileLibRecordOffset nOffsetLast = nOffsetIndex + MyFileHeader.nRecordCount * MyFileHeader.nStoredIndexSize;

		// read the index entry that contains the record block index we are going to use to
		// store the data associated with the new index that we are inserting.
		FileLibIndex   tempbufIndexNew;
		fseek (hFile, nOffsetLast, SEEK_SET);
		fread (&tempbufIndexNew, MyFileHeader.nStoredIndexSize, 1, hFile);

		if (nBlockNumber <= MyFileHeader.nRecordCount) {
			// we need to insert this new index into the list of indexes so
			// we will move the index records from this one and above up one position
			// to make room for this new index we are inserting.
#if defined(RECORD_AT_TIME)
			nOffsetLast -= MyFileHeader.nStoredIndexSize;
			while (nOffsetLast >= nOffsetInsert) {
				fseek (hFile, nOffsetLast, SEEK_SET);
				fread (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
				fseek (hFile, nOffsetLast+MyFileHeader.nStoredIndexSize, SEEK_SET);
				fwrite (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
				nOffsetLast -= MyFileHeader.nStoredIndexSize;
			}
#else
			// create a buffer to be used for moving the indices in large groups rather than
			// one at a time and figure out the proper number of bytes to move at each time.
			// We want the window size to be a multiple of the stored index size to make the
			// file offset calculations easier and we want to make sure that the number of
			// indices will fit into the buffer we use to move data in the file.
			unsigned char uchInsertBuffer[FileLibWindowBufferSize];
			int nInsertWindowSize = sizeof(uchInsertBuffer);
			nInsertWindowSize = (nInsertWindowSize / MyFileHeader.nStoredIndexSize) * MyFileHeader.nStoredIndexSize;

			while (nOffsetLast > nOffsetInsert + nInsertWindowSize) {
				nOffsetLast -= nInsertWindowSize;
				fseek (hFile, nOffsetLast, SEEK_SET);
				fread (uchInsertBuffer, nInsertWindowSize, 1, hFile);
				fseek (hFile, nOffsetLast + MyFileHeader.nStoredIndexSize, SEEK_SET);
				fwrite (uchInsertBuffer, nInsertWindowSize, 1, hFile);
			}

			if (nOffsetLast > nOffsetInsert) {
				nInsertWindowSize = nOffsetLast - nOffsetInsert;
				fseek (hFile, nOffsetInsert, SEEK_SET);
				fread (uchInsertBuffer, nInsertWindowSize, 1, hFile);
				fseek (hFile, nOffsetInsert + MyFileHeader.nStoredIndexSize, SEEK_SET);
				fwrite (uchInsertBuffer, nInsertWindowSize, 1, hFile);
			}
#endif
		}

		// we put the new index into this location in the index area of the file.
		memmove (tempbufIndexNew.uchIndex, pIndex, MyFileHeader.nIndexSize);
		fseek (hFile, nOffsetInsert, SEEK_SET);
		fwrite (&tempbufIndexNew, MyFileHeader.nStoredIndexSize, 1, hFile);
		MyFileHeader.nRecordCount++;

		if (pRecord && MyFileHeader.nRecordSize > 0) {
			nOffsetInsert = MyFileHeader.ulRecordAreaOffset + (tempbufIndexNew.nRecordBlockNumber - 1) * MyFileHeader.nRecordSize;
			fseek (hFile, nOffsetInsert, SEEK_SET);
			fwrite (pRecord, MyFileHeader.nRecordSize, 1, hFile);
		}
		iStatus = 1;

		// Update the file header data and then lets flush the file changes to disk.
		fseek (hFile, 0, SEEK_SET);
		fwrite (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);
		fflush (hFile);
	}

	return iStatus;
}


int FileLibInsert (char *aszFileName, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	int   iStatus = FileLibStatus_FileNotFound;   // default is to indicate an error

	FILE  *hFile = fopen (aszFileName, "r+b");

	if (hFile) {
		iStatus = FileLibInsertFh (hFile, pIndex, pRecord, pFunc);
		fclose (hFile);
	}

	return iStatus;
}
	// -----------------------------------------------

int FileLibRetrieveFh (FILE *hFile, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	FileLibHeader  MyFileHeader;
	long           nRecordBlockNumber;
	int            iStatus = FileLibStatus_FileNotFound;

	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 == FileLibFileSignature && MyFileHeader.ulSignature2 == FileLibFileSignature) {
		long iBlock = FileLibSearch (hFile, pIndex, &nRecordBlockNumber, pFunc);
		if (iBlock == FileLibSearchInvalidFile)
			return FileLibStatus_FileInvalid;

		iStatus = 0;
		if (iBlock > 0) {
			FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;
			FileLibIndex   tempbufIndex;
			FileLibRecordOffset nOffsetDelta = (iBlock - 1) * MyFileHeader.nStoredIndexSize;

			fseek (hFile, nOffsetIndex + nOffsetDelta, SEEK_SET);
			fread (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
			memmove (pIndex, tempbufIndex.uchIndex, MyFileHeader.nIndexSize);

			if (pRecord && MyFileHeader.nRecordSize > 0) {
				nOffsetDelta = (tempbufIndex.nRecordBlockNumber - 1) * MyFileHeader.nRecordSize;
				fseek (hFile, MyFileHeader.ulRecordAreaOffset + nOffsetDelta, SEEK_SET);
				fread (pRecord, MyFileHeader.nRecordSize, 1, hFile);
			}
			iStatus = 1;
		} else {
			iStatus = FileLibStatus_RecordNotFound;
		}
	} else {
		iStatus = FileLibStatus_FileInvalid;
	}

	return iStatus;
}

int FileLibRetrieve (char *aszFileName, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	int   iStatus = FileLibStatus_FileNotFound;

	FILE  *hFile = fopen (aszFileName, "r+b");

	if (hFile) {
		iStatus = FileLibRetrieveFh (hFile, pIndex, pRecord, pFunc);
		fclose (hFile);
	}

	return iStatus;
}
	// -----------------------------------------------

int FileLibIterateFh (FILE *hFile, FileLibIterator *pIterator, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	FileLibHeader  MyFileHeader;
	int            iStatus = FileLibStatus_FileNotFound;

	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 == FileLibFileSignature && MyFileHeader.ulSignature2 == FileLibFileSignature) {
		pIterator->nIndexNumber++;

		iStatus = FileLibStatus_RecordNotFound;
		while (pIterator->nIndexNumber > 0 && pIterator->nIndexNumber <= MyFileHeader.nRecordCount) {
			FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;
			FileLibIndex   tempbufIndex;
			FileLibRecordOffset nOffsetDelta = (pIterator->nIndexNumber - 1) * MyFileHeader.nStoredIndexSize;

			fseek (hFile, nOffsetIndex + nOffsetDelta, SEEK_SET);
			fread (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
			if (pFunc (tempbufIndex.uchIndex, pIndex) == 0) {
				memmove (pIndex, tempbufIndex.uchIndex, MyFileHeader.nIndexSize);

				if (pRecord && MyFileHeader.nRecordSize > 0) {
					nOffsetDelta = (tempbufIndex.nRecordBlockNumber - 1) * MyFileHeader.nRecordSize;
					fseek (hFile, MyFileHeader.ulRecordAreaOffset + nOffsetDelta, SEEK_SET);
					fread (pRecord, MyFileHeader.nRecordSize, 1, hFile);
				}
				iStatus = 1;
				break;
			}
			pIterator->nIndexNumber++;
		}
	} else {
		iStatus = FileLibStatus_FileInvalid;
	}

	return iStatus;
}

int FileLibIterate (char *aszFileName, FileLibIterator *pIterator, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	int   iStatus = FileLibStatus_FileNotFound;

	FILE  *hFile = fopen (aszFileName, "r+b");

	if (hFile) {
		iStatus = FileLibIterateFh (hFile, pIterator, pIndex, pRecord, pFunc);
		fclose (hFile);
	}

	return iStatus;
}

int FileLibDeleteFh (FILE *hFile, void *pIndex, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	FileLibHeader  MyFileHeader;
	int            iStatus = 0;

	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 != FileLibFileSignature || MyFileHeader.ulSignature2 != FileLibFileSignature) {
		return FileLibStatus_FileInvalid;
	}

	if (MyFileHeader.nRecordCount < 1) {
		iStatus = FileLibStatus_RecordNotFound;
	} else {
		FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;

		int nBlockNumber = FileLibSearch (hFile, pIndex, 0, pFunc);
		if (nBlockNumber == FileLibSearchInvalidFile)
			return FileLibStatus_FileInvalid;

		bool bNewRecord = (nBlockNumber < 0);

		if (bNewRecord) {
			iStatus = FileLibStatus_RecordNotFound;
		} else {
			FileLibIndex   tempbufIndexDeleted;

			FileLibRecordOffset nOffsetDelete = nOffsetIndex + (nBlockNumber - 1) * MyFileHeader.nStoredIndexSize;

			// we need to delete this index from the list of indexes so we will move
			// the index records after with this index and above down one position over writing
			// this index position.  However since the stored index has the block number for the
			// associated record which is file content management data we want to keep intact, we
			// will just make a copy of this index and then put it at the end of the index area
			// of the file once we have moved the remaining indices down.
			fseek (hFile, nOffsetDelete, SEEK_SET);
			fread (&tempbufIndexDeleted, MyFileHeader.nStoredIndexSize, 1, hFile);
			memset (tempbufIndexDeleted.uchIndex, 0, sizeof(tempbufIndexDeleted.uchIndex));

#if defined(RECORD_AT_TIME)
			FileLibRecordOffset nOffsetLast = nOffsetIndex + (MyFileHeader.nRecordCount - 1) * MyFileHeader.nStoredIndexSize;

			while (nOffsetLast > nOffsetDelete) {
				fseek (hFile, nOffsetDelete+MyFileHeader.nStoredIndexSize, SEEK_SET);
				fread (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
				fseek (hFile, nOffsetDelete, SEEK_SET);
				fwrite (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
				nOffsetDelete += MyFileHeader.nStoredIndexSize;
			}
#else
			// create a buffer to be used for moving the indices in large groups rather than
			// one at a time and figure out the proper number of bytes to move at each time.
			// We want the window size to be a multiple of the stored index size to make the
			// file offset calculations easier and we want to make sure that the number of
			// indices will fit into the buffer we use to move data in the file.
			unsigned char uchDeleteBuffer[FileLibWindowBufferSize];
			int nDeleteWindowSize = sizeof(uchDeleteBuffer);
			nDeleteWindowSize = (nDeleteWindowSize / MyFileHeader.nStoredIndexSize) * MyFileHeader.nStoredIndexSize;

			FileLibRecordOffset nOffsetLast = nOffsetIndex + (MyFileHeader.nRecordCount - 1) * MyFileHeader.nStoredIndexSize;
			while (nOffsetLast >= nOffsetDelete + nDeleteWindowSize) {
				fseek (hFile, nOffsetDelete + MyFileHeader.nStoredIndexSize, SEEK_SET);
				fread (uchDeleteBuffer, nDeleteWindowSize, 1, hFile);
				fseek (hFile, nOffsetDelete, SEEK_SET);
				fwrite (uchDeleteBuffer, nDeleteWindowSize, 1, hFile);
				nOffsetDelete += nDeleteWindowSize;
			}

			if (nOffsetLast > nOffsetDelete) {
				nDeleteWindowSize = nOffsetLast - nOffsetDelete + MyFileHeader.nStoredIndexSize;
				fseek (hFile, nOffsetDelete + MyFileHeader.nStoredIndexSize, SEEK_SET);
				fread (uchDeleteBuffer, nDeleteWindowSize, 1, hFile);
				fseek (hFile, nOffsetDelete, SEEK_SET);
				fwrite (uchDeleteBuffer, nDeleteWindowSize, 1, hFile);
			}
#endif
			fseek (hFile, nOffsetLast, SEEK_SET);
			fwrite (&tempbufIndexDeleted, MyFileHeader.nStoredIndexSize, 1, hFile);
			MyFileHeader.nRecordCount--;

			// Update the file header data and then lets flush the file changes to disk.
			fseek (hFile, 0, SEEK_SET);
			fwrite (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);
			fflush (hFile);
			iStatus = 1;    // indicate success
		}
	}

	return iStatus;
}
int FileLibDelete (char *aszFileName, void *pIndex, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	int   iStatus = FileLibStatus_FileNotFound;

	FILE  *hFile = fopen (aszFileName, "r+b");

	if (hFile) {
		iStatus = FileLibDeleteFh (hFile, pIndex, pFunc);
		fclose (hFile);
	}

	return iStatus;
}

int FileLibUpdateFh (FILE *hFile, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	FileLibHeader  MyFileHeader;
	long           nRecordBlockNumber;
	int            iStatus = 0;

	fseek (hFile, 0, SEEK_SET);
	fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

	// Check the signatures to determine if this file has been initialized properly.
	// If signatures do not check out then we can not depend on file having proper management data.
	if (MyFileHeader.ulSignature1 == FileLibFileSignature && MyFileHeader.ulSignature2 == FileLibFileSignature) {

		if (MyFileHeader.nRecordCount < 1)
			return FileLibStatus_RecordNotFound;

		long iBlock = FileLibSearch (hFile, pIndex, &nRecordBlockNumber, pFunc);
		if (iBlock == FileLibSearchInvalidFile)
			return FileLibStatus_FileInvalid;

		iStatus = 0;
		if (iBlock < 1) {
			iStatus = FileLibStatus_RecordNotFound;
		} else {
			FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;
			FileLibIndex   tempbufIndex;
			FileLibRecordOffset nOffsetDelta = (iBlock - 1) * MyFileHeader.nStoredIndexSize;

			fseek (hFile, nOffsetIndex + nOffsetDelta, SEEK_SET);
			fread (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
			memmove (tempbufIndex.uchIndex, pIndex, MyFileHeader.nIndexSize);
			fseek (hFile, nOffsetIndex + nOffsetDelta, SEEK_SET);
			fwrite (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);

			if (pRecord && MyFileHeader.nRecordSize > 0) {
				nOffsetDelta = (tempbufIndex.nRecordBlockNumber - 1) * MyFileHeader.nRecordSize;
				fseek (hFile, MyFileHeader.ulRecordAreaOffset + nOffsetDelta, SEEK_SET);
				fwrite (pRecord, MyFileHeader.nRecordSize, 1, hFile);
			}
			fflush (hFile);
			iStatus = 1;      // indicate success
		}
	} else {
		iStatus = FileLibStatus_FileInvalid;
	}

	return iStatus;
}

int FileLibUpdate (char *aszFileName, void *pIndex, void *pRecord, int (*pFunc)(void *pIndex1, void *pIndex2))
{
	int  iStatus = FileLibStatus_FileNotFound;

	FILE  *hFile = fopen (aszFileName, "r+b");

	if (hFile) {
		iStatus = FileLibUpdateFh (hFile, pIndex, pRecord, pFunc);
		fclose (hFile);
	}
	return iStatus;
}

#if defined(PROVIDE_FILELIBCHECKFILE)
int FileLibCheckFile (char *aszFileName, FileLibFileCheckBuffer *aCheckBuf, int nCheckBufCount)
{
	FILE *hFile = fopen (aszFileName, "rb");
	int   iStatus = FileLibStatus_FileNotFound;

	if (hFile) {
		FileLibHeader  MyFileHeader;

		fseek (hFile, 0, SEEK_SET);
		fread (&MyFileHeader, sizeof(MyFileHeader), 1, hFile);

		// Check the signatures to determine if this file has been initialized properly.
		// If signatures do not check out then we can not depend on file having proper management data.
		if (MyFileHeader.ulSignature1 != FileLibFileSignature || MyFileHeader.ulSignature2 != FileLibFileSignature) {
			printf (" File Signature is invalid.\n");
			fclose (hFile);
			return -1;
		}

		FileLibRecordOffset nOffsetIndex = sizeof(MyFileHeader) + MyFileHeader.nHeaderSize;
		FileLibIndex   tempbufIndex;

		fseek (hFile, nOffsetIndex, SEEK_SET);

		for (int iCount = 0; iCount < MyFileHeader.nMaxRecords && iCount < nCheckBufCount; iCount++) {
			fread (&tempbufIndex, MyFileHeader.nStoredIndexSize, 1, hFile);
			if (tempbufIndex.nRecordBlockNumber > 0 && tempbufIndex.nRecordBlockNumber <= nCheckBufCount) {
				if (aCheckBuf[tempbufIndex.nRecordBlockNumber - 1].nIndexNumber != 0) {
					printf (" Index count %d record block number %d duplicated\n", iCount, tempbufIndex.nRecordBlockNumber);
				} else {
					aCheckBuf[tempbufIndex.nRecordBlockNumber - 1].nIndexNumber = iCount + 1;
					if (MyFileHeader.nRecordSize > 0) {
						aCheckBuf[tempbufIndex.nRecordBlockNumber - 1].nRecordOffset = MyFileHeader.ulRecordAreaOffset + (tempbufIndex.nRecordBlockNumber - 1) * MyFileHeader.nRecordSize;
					}
				}
			} else {
				printf (" Index count %d record block number %d out of range\n", iCount, tempbufIndex.nRecordBlockNumber);
			}
		}

		for (int iCount = 0; iCount < MyFileHeader.nMaxRecords && iCount < nCheckBufCount; iCount++) {
			if (aCheckBuf[iCount].nIndexNumber == 0) {
				printf (" Index count %d record block number missing\n", iCount);
			}
		}

		fclose (hFile);
	}

	return 0;
}
#else
int FileLibCheckFile (char *aszFileName, FileLibFileCheckBuffer *aCheckBuf, int nCheckBufCount)
{
	return 0;
}
#endif
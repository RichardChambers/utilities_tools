// FileAccessLib.cpp : Defines the entry point for the console application.
//

/*
	This file contains a test harness to be used when testing the FileLib function library.
	It is a console type of application.  The main entry point is at the bottom of the file.

	There are three different test harnesses that are used.

	The first one does various operations and then uses the FileLibCheckFile() function to do
	a file consistency check.  There are two versions of this function with one doing an actual
	consistency check and one that is a stub that does nothing.  To enable the actual consistency
	check, the defined constant PROVIDE_FILELIBCHECKFILE must be defined when compiling FileLib.cpp.

**/

#include "stdafx.h"

#include <string.h>

#include "FileLib.h"

//--------------   beginning of auto testing test harness mainAutoTest()    ------------------
//--------------------------------------------------------------------------------------
typedef struct {
	int jj1;
	int jj2;
	int jj3;
} HeaderThing;

typedef struct {
	int iValue;
	char aszKey[12];
} IndexThing;

typedef struct {
	int iValue;
	char aszKey[12];
	char aszName[24];
	char aszName2[16];
	int xValue;
	int yValue;
} RecordThing;

// Collation function to determine the order of the two arguments.
//  - returns -1 if pIndex1 should go before pIndex2
//  - returns 0 if pIndex1 and pIndex2 have the same key
//  - returns +1 if Pindex1 should go after pIndex2
static int CollateFunc (void *pIndex1, void *pIndex2)
{
	IndexThing *pLookFor = (IndexThing *)pIndex1;
	IndexThing *pFileItem = (IndexThing *)pIndex2;

	// do a string compare on the two keys to determine if pLookFor is lower
	// in the collating order than pFileItem (strcmp returns -1), if the two
	// keys are equal (strcmp returns 0), or if pLookFor is higher in the
	// collating order than pFileItem.
	return (strcmp (pLookFor->aszKey, pFileItem->aszKey));
}

static void MakeIndexAndRecord (int iLoop, IndexThing &MyIndex, RecordThing &MyRecord)
{
	// ensure that the index and record memory areas are
	// initialized allowing for memcmp() to work properly when
	// doing comparisons as a part of testing.
	memset (&MyIndex, 0, sizeof(IndexThing));
	memset (&MyRecord, 0, sizeof(RecordThing));

	// Create the key that we will use to find this record.
	sprintf (MyIndex.aszKey, "JJ%4.4d", iLoop);
	strcpy (MyRecord.aszKey, MyIndex.aszKey);

	// now initialize the various data areas with what
	// should be reasonably unique values so that we
	// can do a check that data storage is reliable.
	MyIndex.iValue = iLoop;
	sprintf (MyRecord.aszName, "Geo %4.4d", iLoop);
	MyRecord.iValue = iLoop;
	MyRecord.xValue = 100 + iLoop;
	MyRecord.yValue = 200000 + iLoop;
}

int mainAutoTest(void)
{
	IndexThing MyIndex;
	RecordThing MyRecord;

	printf ("\nAuto Test harnes - mainAutoTest()\n");

	FileLibFileCheckBuffer  MyCheckBuffer[5000];
	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));

	// Create our test file and then run the file check to see
	// that everything was created to a proper initial state.
	FileLibCreate ("testname1", 5000, sizeof(IndexThing), sizeof(RecordThing), sizeof(HeaderThing));

	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check - After Create\n");
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	// Now begin our test cases
	int iLoop;

	// If we insert the first 50 records in numerical order, can
	// we then retrieve the same set of 50 records?
	for (iLoop = 1; iLoop < 50; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}
	for (iLoop = 1; iLoop < 50; iLoop++) {
		IndexThing  MyComparisonIndex;
		RecordThing MyComparisonRecord;

		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		MakeIndexAndRecord (iLoop, MyComparisonIndex, MyComparisonRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
		if (memcmp (&MyIndex, &MyComparisonIndex, sizeof(IndexThing)) != 0) {
			printf (" Index data mismatch %d\n", iLoop);
		}
		if (memcmp (&MyRecord, &MyComparisonRecord, sizeof(RecordThing)) != 0) {
			printf (" Record data mismatch %d\n", iLoop);
		}
	}
	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check 1\n");
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	// If we delete some of the records from the file, is the
	// remaining data still correct?
	for (iLoop = 1; iLoop < 25; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibDelete ("testname1", &MyIndex, CollateFunc);
	}
	for (iLoop = 25; iLoop < 50; iLoop++) {
		IndexThing  MyComparisonIndex;
		RecordThing MyComparisonRecord;

		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		MakeIndexAndRecord (iLoop, MyComparisonIndex, MyComparisonRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
		if (memcmp (&MyIndex, &MyComparisonIndex, sizeof(IndexThing)) != 0) {
			printf (" Index data mismatch %d\n", iLoop);
		}
		if (memcmp (&MyRecord, &MyComparisonRecord, sizeof(RecordThing)) != 0) {
			printf (" Record data mismatch %d\n", iLoop);
		}
	}
	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check 2\n");
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	for (iLoop = 50; iLoop < 100; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}
	for (iLoop = 50; iLoop < 100; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}

	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check 3\n");
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	for (iLoop = 1; iLoop < 25; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}

	for (iLoop = 1; iLoop < 100; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}

	printf ("\n  Test insert in middle, two sections 100 - 199\n");
	for (iLoop = 100; iLoop < 200; iLoop+=2) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}
	for (iLoop = 100; iLoop < 200; iLoop+=2) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}
	for (iLoop = 101; iLoop < 200; iLoop+=2) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}
	for (iLoop = 101; iLoop < 200; iLoop+=2) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}
	for (iLoop = 100; iLoop < 200; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}
	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check - after insert 100-199\n");
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));


	for (iLoop = 400; iLoop < 2000; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}
	for (iLoop = 400; iLoop < 2000; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}


	for (iLoop = 200; iLoop < 400; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}
	for (iLoop = 200; iLoop < 400; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}

	for (iLoop = 1; iLoop < 2000; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);
		if (strcmp (MyIndex.aszKey, MyRecord.aszKey) != 0) {
			printf (" Mismatch %d\n", iLoop);
		}
	}

	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check line %d\n", __LINE__);
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	for (iLoop = 1999; iLoop > 0; iLoop-=2) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		if (0 != FileLibDelete ("testname1", &MyIndex, CollateFunc)) {
			printf ("    iLoop %d Error with delete %s\n", iLoop, MyIndex.aszKey);
		}
	}
	for (iLoop = 1998; iLoop > 0; iLoop-=2) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		memset (&MyRecord, 0, sizeof(MyRecord));
		if (0 != FileLibDelete ("testname1", &MyIndex, CollateFunc)) {
			printf ("    iLoop %d Error with delete %s\n", iLoop, MyIndex.aszKey);
		}
	}

	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check - after delete all\n");
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));


		MakeIndexAndRecord (50, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
		MakeIndexAndRecord (5001, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);

	FileLibFileInfo MyFileInfo;
	FileLibReadHeader ("testname1", 0, &MyFileInfo);

	MakeIndexAndRecord (99, MyIndex, MyRecord);
	FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);

	memset (&MyRecord, 0, sizeof(MyRecord));
	FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);

	MakeIndexAndRecord (88, MyIndex, MyRecord);
	FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);

	memset (&MyRecord, 0, sizeof(MyRecord));
	FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);

	strcpy (MyRecord.aszName, "Rep 88");
	MyRecord.yValue = 888;
	FileLibUpdate ("testname1", &MyIndex, &MyRecord, CollateFunc);

	memset (&MyRecord, 0, sizeof(MyRecord));
	FileLibRetrieve ("testname1", &MyIndex, &MyRecord, CollateFunc);

	for (iLoop = 1; iLoop < 10; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		if (0 != FileLibDelete ("testname1", &MyIndex, CollateFunc)) {
			printf ("    Error with delete %s\n", MyIndex.aszKey);
		}
	}

	for (iLoop = 1; iLoop < 40; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname1", &MyIndex, &MyRecord, CollateFunc);
	}

	for (iLoop = 1; iLoop < 15; iLoop +=2) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		if (0 != FileLibDelete ("testname1", &MyIndex, CollateFunc)) {
			printf ("    Error with delete %s\n", MyIndex.aszKey);
		}
	}

	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check - Final\n");
	FileLibCheckFile ("testname1", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	memset (&MyRecord, 0, sizeof(MyRecord));
	char junkbuf[24];

	printf ("\nPause - press return key to continue.\n");
	fgets (junkbuf, sizeof(junkbuf), stdin);

	return 0;
}
//--------------   end of auto testing test harness mainAutoTest()    ------------------
//--------------------------------------------------------------------------------------

//==============================================================================================

//--------------   begin of auto testing test harness mainAutoTest2()    ------------------
//--------------------------------------------------------------------------------------
// This auto test harness uses an index only use of the functions and does not store
// an associated record data.
int mainAutoTest2 (void)
{
	IndexThing MyIndex;
	RecordThing MyRecord;

	printf ("\nAuto Test harnes - mainAutoTest2()\n");

	FileLibFileCheckBuffer  MyCheckBuffer[5000];
	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));

	// Create our test file and then run the file check to see
	// that everything was created to a proper initial state.
	FileLibCreate ("testname2", 5000, sizeof(IndexThing), 0, sizeof(HeaderThing));

	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check - After Create\n");
	FileLibCheckFile ("testname2", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	// Now begin our test cases
	int iLoop;

	// If we insert the first 50 records in numerical order, can
	// we then retrieve the same set of 50 records?
	for (iLoop = 1; iLoop < 50; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibInsert ("testname2", &MyIndex, 0, CollateFunc);
	}
	for (iLoop = 1; iLoop < 50; iLoop++) {
		IndexThing  MyComparisonIndex;
		RecordThing MyComparisonRecord;

		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		MakeIndexAndRecord (iLoop, MyComparisonIndex, MyComparisonRecord);
		FileLibRetrieve ("testname2", &MyIndex, 0, CollateFunc);
		if (memcmp (&MyIndex, &MyComparisonIndex, sizeof(IndexThing)) != 0) {
			printf (" Index data mismatch %d\n", iLoop);
		}
	}
	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check 1\n");
	FileLibCheckFile ("testname2", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	// If we delete some of the records from the file, is the
	// remaining data still correct?
	for (iLoop = 1; iLoop < 25; iLoop++) {
		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		FileLibDelete ("testname2", &MyIndex, CollateFunc);
	}
	for (iLoop = 25; iLoop < 50; iLoop++) {
		IndexThing  MyComparisonIndex;
		RecordThing MyComparisonRecord;

		MakeIndexAndRecord (iLoop, MyIndex, MyRecord);
		MakeIndexAndRecord (iLoop, MyComparisonIndex, MyComparisonRecord);
		FileLibRetrieve ("testname2", &MyIndex, 0, CollateFunc);
		if (memcmp (&MyIndex, &MyComparisonIndex, sizeof(IndexThing)) != 0) {
			printf (" Index data mismatch %d\n", iLoop);
		}
	}
	memset (MyCheckBuffer, 0, sizeof(MyCheckBuffer));
	printf ("\nPerform File Check 2\n");
	FileLibCheckFile ("testname2", MyCheckBuffer, sizeof(MyCheckBuffer)/sizeof(MyCheckBuffer[0]));

	return 0;
}
//--------------   end of auto testing test harness mainAutoTest2()    ------------------
//--------------------------------------------------------------------------------------

//=========================================================================================

//--------------   begin example mainAutoTest3()    ------------------
//--------------------------------------------------------------------------------------
typedef struct {
	char  aszKeyEmployeeNumber[8];   // primary key for employee file, the employee id number
	char  aszKeyDepartment[4];       // foreign key to department file
} IndexEmployee;
typedef struct {
	char  aszLastName[32];           // last name of the employee
	char  aszFirstName[32];          // first name of the employee
	char  aszEmployeeNumber[8];      // the employee number of the employee
	char  aszDepartment[4];          // the department identifier for the employee's department
} RecordEmployee;

static int EmployeeCollateFunc (void *pIndex1, void *pIndex2)
{
	IndexEmployee *pLookFor = (IndexEmployee *)pIndex1;
	IndexEmployee *pInFile = (IndexEmployee *)pIndex2;

	return (strcmp (pLookFor->aszKeyEmployeeNumber, pInFile->aszKeyEmployeeNumber));
}

typedef struct {
	char  aszKeyDepartment[4];      // primary key for the department file, the department number
} IndexDepartment;
typedef struct {
	char  aszDepartmentName[32];    // the name of the department identifed by the department number
}RecordDepartment;

static int DepartmentCollateFunc (void *pIndex1, void *pIndex2)
{
	IndexDepartment *pLookFor = (IndexDepartment *)pIndex1;
	IndexDepartment *pInFile = (IndexDepartment *)pIndex2;

	return (strcmp (pLookFor->aszKeyDepartment, pInFile->aszKeyDepartment));
}

typedef struct {
	char           *filename;
	IndexEmployee   Index;
	RecordEmployee  Record;
	int            (*CollateFunc) (void *pIndex1, void *pIndex2);
} FileEmployee;
typedef struct {
	char              *filename;
	IndexDepartment   Index;
	RecordDepartment  Record;
	int               (*CollateFunc) (void *pIndex1, void *pIndex2);
} FileDepartment;


static int CreateInsertDepartmentData (int iNumber, FileDepartment &DepartmentFile)
{
	// create the index and record for a specific department
	sprintf (DepartmentFile.Index.aszKeyDepartment, "%3.3d", iNumber);
	sprintf (DepartmentFile.Record.aszDepartmentName, "Dept %3.3d", iNumber);
	return FileLibInsert (DepartmentFile.filename, &DepartmentFile.Index, &DepartmentFile.Record, DepartmentFile.CollateFunc);
}

static int CreateInsertEmployeeData (int iNumber, FileEmployee &EmployeeFile)
{
	// generate an employee record with information that identifies the employee number
	// and the generated department for this employee.
	int iDepartmentNumber = (iNumber % 20) + 101;  // generate a department id from 101 to 119

	sprintf (EmployeeFile.Index.aszKeyEmployeeNumber, "%6.6d", iNumber);
	sprintf (EmployeeFile.Index.aszKeyDepartment, "%3.3d", iDepartmentNumber);
	sprintf (EmployeeFile.Record.aszDepartment, "%3.3d", iDepartmentNumber);

	sprintf (EmployeeFile.Record.aszFirstName, "John %6.6d ne %3.3d", iNumber, iDepartmentNumber);
	sprintf (EmployeeFile.Record.aszLastName, "Boyd %6.6d ne %3.3d", iNumber, iDepartmentNumber);
	return FileLibInsert (EmployeeFile.filename, &EmployeeFile.Index, &EmployeeFile.Record, EmployeeFile.CollateFunc);
}

static void CreateEmployeeIndex (int iNumber, IndexEmployee &EmployeeIndex)
{
	// create the index to be used to locate an employee based on the employee number
	sprintf (EmployeeIndex.aszKeyEmployeeNumber, "%6.6d", iNumber);
}

static int FilterEmployeeByDepartment (void *pIndex1, void *pIndex2)
{
	IndexEmployee *pLookFor = (IndexEmployee *)pIndex1;
	IndexEmployee *pInFile = (IndexEmployee *)pIndex2;

	return (strcmp (pLookFor->aszKeyDepartment, pInFile->aszKeyDepartment));
}


int mainAutoTest3 (void)
{
	FileEmployee    EmployeeFile;
	FileDepartment  DepartmentFile;

	printf ("\nAuto Test harness - mainAutoTest3()\n");

	EmployeeFile.filename = "employees";
	EmployeeFile.CollateFunc = EmployeeCollateFunc;
	DepartmentFile.filename = "departments";
	DepartmentFile.CollateFunc = DepartmentCollateFunc;

	FileLibCreate (EmployeeFile.filename, 1000, sizeof(EmployeeFile.Index), sizeof(EmployeeFile.Record), 0);
	FileLibCreate (DepartmentFile.filename, 50, sizeof(DepartmentFile.Index), sizeof(DepartmentFile.Record), 0);

	// create the various departments
	for (int iNumber = 101; iNumber < 120; iNumber++) {
		CreateInsertDepartmentData (iNumber, DepartmentFile);
	}

	// create some employees which will be using the above departments
	for (int iEmployee = 10457; iEmployee < 11400; iEmployee += 12) {
		CreateInsertEmployeeData (iEmployee, EmployeeFile);
	}

	// look up first couple employees and print their data
	for (int iEmployee = 10457; iEmployee < 10500; iEmployee += 12) {
		IndexEmployee   MyEmployeeIndex;
		RecordEmployee  MyEmployeeRecord;

		CreateEmployeeIndex (iEmployee, MyEmployeeIndex);
		if (1 == FileLibRetrieve (EmployeeFile.filename, &MyEmployeeIndex, &MyEmployeeRecord, EmployeeFile.CollateFunc)) {
			IndexDepartment   MyDepartmentIndex;
			RecordDepartment  MyDepartmentRecord;
			strcpy (MyDepartmentIndex.aszKeyDepartment, MyEmployeeRecord.aszDepartment);
			memset (&MyDepartmentRecord, 0, sizeof(MyDepartmentRecord));
			FileLibRetrieve (DepartmentFile.filename, &MyDepartmentIndex, &MyDepartmentRecord, DepartmentFile.CollateFunc);
			printf (" iEmployee %d Last: %s, First: %s, Dept: %s\n", iEmployee, MyEmployeeRecord.aszLastName, MyEmployeeRecord.aszFirstName, MyDepartmentRecord.aszDepartmentName);
		}
	}

	FileLibIterator MyIterator;

	FileLibIteratorInit (MyIterator);

	{
		IndexEmployee   MyEmployeeIndex;
		RecordEmployee  MyEmployeeRecord;
		sprintf (MyEmployeeIndex.aszKeyDepartment, "%3.3d", 118);
		// Iterate through the employee file looking for all employees that are in the specified department.
		while (1 == FileLibIterate (EmployeeFile.filename, &MyIterator, &MyEmployeeIndex, &MyEmployeeRecord, FilterEmployeeByDepartment)) {
			IndexDepartment   MyDepartmentIndex;
			RecordDepartment  MyDepartmentRecord;
			strcpy (MyDepartmentIndex.aszKeyDepartment, MyEmployeeRecord.aszDepartment);
			memset (&MyDepartmentRecord, 0, sizeof(MyDepartmentRecord));
			FileLibRetrieve (DepartmentFile.filename, &MyDepartmentIndex, &MyDepartmentRecord, DepartmentFile.CollateFunc);
			printf (" Last: %s, First: %s, Dept: %s\n", MyEmployeeRecord.aszLastName, MyEmployeeRecord.aszFirstName, MyDepartmentRecord.aszDepartmentName);
		}
	}

	return 0;
}

//=========  Main Entry point for console application    ===========
int _tmain(int argc, _TCHAR* argv[])
{
	return mainAutoTest3();
}
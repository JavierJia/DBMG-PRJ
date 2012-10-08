#include <iostream>
#include <string>
#include <cassert>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctime>

#include "pf.h"
#include "../util/Logger.h"
#include "../util/IO.h"

using namespace std;

const int success = 0;

clock_t clock_begin = clock();
#define SECOND_PASSED ((float)(clock() - clock_begin) / CLOCKS_PER_SEC)
#define	TIMELOG(p) {printf("%.3f: %s\n", (float)(clock() - clock_begin) / CLOCKS_PER_SEC, p);}

// Check if a file exists
bool FileExists(string fileName)
{
    struct stat stFileInfo;

    if(stat(fileName.c_str(), &stFileInfo) == 0) return true;
    else return false;
}

int TestStatInformation( const char* fileName){
	struct stat sb;
	if ( stat( fileName, &sb ) ){
		perror ("stat error");
		return -1;
	}
	printf("%.3f File Stat Over", SECOND_PASSED);
	printf(" FILE MODE:\n");
	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}
	
	printf("I-node number:            %ld\n", (long) sb.st_ino);
	printf("Mode:                     %lo (octal)\n",
	(unsigned long) sb.st_mode);
	printf("Link count:               %ld\n", (long) sb.st_nlink);
	//printf("Ownership:                UID=%ld   GID=%ld\n",	(long) sb.st_uid, (long) sb.st_gid);
	//printf("Preferred I/O block size: %ld bytes\n",(long) sb.st_blksize);
	printf("File size:                %lld bytes\n",(long long) sb.st_size);
	printf("Blocks allocated:         %lld\n",(long long) sb.st_blocks);
	//printf("Last status change:       %s", ctime(&sb.st_ctime));
	//printf("Last file access:         %s", ctime(&sb.st_atime));
	//printf("Last file modification:   %s", ctime(&sb.st_mtime));
	return 0;
}

int PF_Manager_Test_CreateFile( PF_Manager *pf){
	printf("%.3f: PF_Manager Test CreateFile\n", SECOND_PASSED);
	RC rc;
	const char* fileName = "test.dat";
	rc = pf->CreateFile(fileName);
	assert(rc == success);
	printf("%.3f: PF_Manager CreateFile New\n", SECOND_PASSED);

	rc = pf->CreateFile(fileName);
	assert(rc != success);
	printf("%.3f: PF_Manager CreateFile Exist\n", SECOND_PASSED);

	rc = pf->CreateFile(NULL);
	assert(rc != success);
	printf("%.3f: PF_Manager CreateFile Null named \n", SECOND_PASSED);
	return 0;
}

int PF_Manager_Test_OpenFile( PF_Manager *pf){
	TIMELOG("PF_Manager Test OpenFile");

	const char* fileName = "test.dat";
	PF_FileHandle* pf_handle = new PF_FileHandle();
	RC rc = pf->OpenFile( fileName, *pf_handle);
	TIMELOG("PF_Manager OpenFile exist Over");
	assert(rc == success);

	fileName = "testX.dat";
	rc = pf->OpenFile( fileName, *pf_handle);
	TIMELOG("PF_Manager OpenFile not exsit Over");
	assert(rc != success);
	delete pf_handle;
	return 0;
}

int PF_FileHandle_Test_AppendPage(PF_Manager *pf){

	const char* fileName = "test.dat";
	PF_FileHandle *pf_handle = new PF_FileHandle();
	RC rc = pf->OpenFile( fileName,*pf_handle);
	
	void* data = malloc (PF_PAGE_SIZE);
    for(unsigned i = 0; i < PF_PAGE_SIZE; i++)
    {
        *((char *)data+i) = i % 94 + 32;
    }

	TIMELOG("PF_FileHandle_Test_PageNumber");
	int pagenum = pf_handle->GetNumberOfPages();
	assert(pagenum == 0);

	// Read or Write to the empty file, should fail
	TIMELOG("PF_FileHandle_Test Read empty file ");
	rc = pf_handle->ReadPage(0,data);
	TIMELOG("PF_FileHandle_Test Read empty file done");
	assert(rc != success);

	TIMELOG("PF_FileHandle_Test Write empty file ");
	rc = pf_handle->WritePage(0,data);
	TIMELOG("PF_FileHandle_Test Write empty file done");
	assert(rc != success);
	////////////////////////////////////////////////////////////	
	
	// Append to first block
	TIMELOG("PF_FileHandle_Test_Append Begin");
	rc = pf_handle->AppendPage(data);
	TIMELOG("PF_FileHandle_Test_Append Start");
	assert( rc == success);

	TIMELOG("PF_FileHandle_Test_PageNumber");
	pagenum = pf_handle->GetNumberOfPages();
	assert(pagenum == 1);

	TestStatInformation("test.dat");

	// Read the regular page
	void* readdata = malloc (PF_PAGE_SIZE);
	TIMELOG("PF_FileHandle_Test_Read Regular file Begin");
	rc = pf_handle->ReadPage(0,readdata);
	TIMELOG("PF_FileHandle_Test_Read Regular file End");
	assert( rc == success);
	for( int i = 0; i < PF_PAGE_SIZE; i++){
		assert (*((char*)data+i) == *((char*)readdata+i));
	}
	// Write the regular page
	void* writedata = malloc( PF_PAGE_SIZE);
	for (int i = 0; i < PF_PAGE_SIZE; i++){
		*((char*)writedata+i) = 'w';
	}
	TIMELOG("PF_FileHandle_Test_Write Regular file Begin");
	rc = pf_handle->WritePage(0, writedata);
	TIMELOG("PF_FileHandle_Test_Write Regular file End");
	assert(rc == success);
	// Verify the data
	rc = pf_handle->ReadPage(0,readdata);
	assert( rc == success);
	for( int i = 0; i < PF_PAGE_SIZE; i++){
		assert (*((char*)writedata+i) == *((char*)readdata+i));
	}
	////////////////////////////////////////////////////////////////	
	
	// Test unregular datablock
	TIMELOG("PF_FileHandle_Test_Append append less than a page");
	void* halfdata = malloc (PF_PAGE_SIZE/2);
    for(unsigned i = 0; i < PF_PAGE_SIZE/2; i++)
    {
        *((char *)halfdata+i) = 'h';
    }
	TIMELOG("PF_FileHandle_Test_Append Begin");
	rc = pf_handle->AppendPage(halfdata);
	TIMELOG("PF_FileHandle_Test_Append Start");
	assert( rc == success);

	TIMELOG("PF_FileHandle_Test_PageNumber");
	pagenum = pf_handle->GetNumberOfPages();
	assert(pagenum == 2);

	TestStatInformation("test.dat");

	void* doubledata= malloc (PF_PAGE_SIZE*4);
	/// Read and write to half block data
	rc = pf_handle->ReadPage(1, readdata);
	for(int i= 0; i< PF_PAGE_SIZE/2; i++){
		assert ( *((char*) halfdata +i ) == *((char*) readdata +i));
	}
	
	TIMELOG("PF_FileHandle_Test_Append append more than a page");
    for(unsigned i = 0; i < PF_PAGE_SIZE*2; i++)
    {
        *((char *)doubledata+i) = '!';
    }
	TIMELOG("PF_FileHandle_Test_Append Begin");
	rc = pf_handle->AppendPage(doubledata);
	TIMELOG("PF_FileHandle_Test_Append Start");
	assert( rc == success);

	TIMELOG("PF_FileHandle_Test_PageNumber");
	pagenum = pf_handle->GetNumberOfPages();
	assert(pagenum == 3);
	
	TestStatInformation("test.dat");

	rc = pf_handle->ReadPage(2, readdata);
	for(int i= 0; i< PF_PAGE_SIZE; i++){
		assert ( *((char*) doubledata+i ) == *((char*) readdata +i));
	}
    // Close the file "test"
    rc = pf->CloseFile(*pf_handle);
    assert(rc == success);

    // DestroyFile
    rc = pf->DestroyFile(fileName);
    assert(rc == success);
    
	//if(!FileExists(fileName))
	if (!util::FileExists(fileName))
    {
        cout << "File " << fileName << " has been destroyed." << endl;
        cout << "Test Case 2 Passed!" << endl << endl;
        return 0;
    }
    else
    {
        cout << "Failed to destroy file!" << endl;
        return -1;
    }

	free(data);
	free(readdata);
	free(writedata);
	free(halfdata);
	free(doubledata);
	delete pf_handle;
	return 0;
}

int main()
{
    PF_Manager *pf = PF_Manager::Instance();
    remove("test.dat");
   
	PF_Manager_Test_CreateFile(pf);
	PF_Manager_Test_OpenFile(pf);
	PF_FileHandle_Test_AppendPage(pf);
    return 0;
}

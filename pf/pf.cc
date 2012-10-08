#include <sys/stat.h>
#include "pf.h"

#define CHECK_NULL(pt)	{ \
	if ( pt == NULL){	\
		fprintf (stderr, "null value\n"); \
		return -1;}}

PF_Manager* PF_Manager::_pf_manager = NULL;

PF_Manager* PF_Manager::Instance()
{
    if(!_pf_manager)
        _pf_manager = new PF_Manager();
    
    return _pf_manager;    
}

PF_Manager::PF_Manager()
{
}

PF_Manager::~PF_Manager()
{
}
    
RC PF_Manager::CreateFile(const char *fileName)
{
	struct stat buffer;
	if ( stat( fileName, &buffer) == 0){
		fprintf ( stderr, "file %s already existed\n",fileName);
		return -1;
	}

	FILE *fp = fopen ( fileName, "wb");
	if ( fp == NULL ){
		perror ( "creating error");
		return -1;
	}
	fclose( fp );

    return 0;
}


RC PF_Manager::DestroyFile(const char *fileName)
{
	CHECK_NULL(fileName);

	if ( remove ( fileName ) != 0){
		perror ( "deleting error");
		return -1;
	}
	return 0;
}


RC PF_Manager::OpenFile(const char *fileName, PF_FileHandle &fileHandle)
{
	CHECK_NULL(fileName)

	FILE	*fp;					

	fp	= fopen( fileName, "rb+" );
	if ( fp == NULL ) {
		perror ( "opening error ");
		return -1;
	}

	RC rc = fileHandle.AttachFILE(fp);
	return rc;
}


RC PF_Manager::CloseFile(PF_FileHandle &fileHandle)
{
	FILE * fp = fileHandle.GetFILE();
	
	CHECK_NULL(fp);
	if ( fclose(fp) == EOF){
		perror( "closing error ");
		return -1;
	}
    return 0;
}


PF_FileHandle::PF_FileHandle(): _fp(NULL), _total_pages(0)
{
}
 

PF_FileHandle::~PF_FileHandle()
{
}

FILE* PF_FileHandle::GetFILE(){
	return _fp;
}

RC PF_FileHandle::AttachFILE(FILE * fp){
	CHECK_NULL(fp);

	if ( fseek( fp, 0, SEEK_END) ){
		perror( "attaching seek error");
		return -1;
	}
	long file_size = ftell(fp);
	if ( file_size < 0){
		perror( "attaching tell error");
		return -1;
	}
	_fp = fp;
	_total_pages = file_size / PF_PAGE_SIZE;
	rewind(_fp);
	return 0;
}

RC PF_FileHandle::ReadPage(PageNum pageNum, void *data)
{
	CHECK_NULL(data);
	CHECK_NULL(_fp);

	if ( pageNum >= _total_pages ) {
		fprintf (stderr, "read error, out of bound\n");
		return -1;
	}

	fseek( _fp, pageNum * PF_PAGE_SIZE, SEEK_SET);
	if ( 1 != fread( data, PF_PAGE_SIZE, 1, _fp)){
		perror ( "read error, fread failed");
		return -1;
	}
    return 0;
}


RC PF_FileHandle::WritePage(PageNum pageNum, const void *data)
{
	CHECK_NULL (data);
	CHECK_NULL (_fp);

	if ( pageNum >= _total_pages ){
		fprintf (stderr, "write error, out of bound\n");
		return -1;
	}

	fseek( _fp, pageNum * PF_PAGE_SIZE, SEEK_SET);
	if ( 1 != fwrite( data, PF_PAGE_SIZE, 1, _fp)){
		perror( "write error, fwrite failed");
		return -1;
	}
	fflush(_fp);
    return 0;
}


RC PF_FileHandle::AppendPage(const void *data)
{
	CHECK_NULL (data);
	CHECK_NULL (_fp);

	fseek( _fp, 0, SEEK_END);
	if ( 1 != fwrite( data, PF_PAGE_SIZE, 1, _fp)){
		perror ( "append error, fwrite failed");
		return -1;
	}
	fflush(_fp);
	_total_pages += 1;
    return 0;
}


unsigned PF_FileHandle::GetNumberOfPages()
{
	return _total_pages ;
}



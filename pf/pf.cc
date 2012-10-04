#include "pf.h"

#define CHECK_NULL(pt)	{ \
	if ( pt == NULL){	\
		fprintf (stderr, "null value\n"); \
		return -1;}}



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
	CHECK_NULL(fileName);

	FILE	*fp;
	fp	= fopen( fileName, "r" );
	if ( fp != NULL ) {
		fprintf ( stderr, "file %s already existed\n",fileName);
		fclose(fp);
		return -1;
	}

	fp = fopen ( fileName, "wb");
	if ( fp == NULL ){
		perror ( "creating error:\n");
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

	fp	= fopen( fileName, "rb" );
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


PF_FileHandle::PF_FileHandle(): _fp(NULL), _file_size(0)
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
	_file_size = ftell(fp);
	if ( _file_size < 0){
		perror( "attaching tell error");
		_file_size = 0;
		return -1;
	}
	_fp = fp;
	rewind(_fp);
	return 0;
}

RC PF_FileHandle::ReadPage(PageNum pageNum, void *data)
{
	CHECK_NULL(data);
	CHECK_NULL(_fp);

	if ( (pageNum) * PF_PAGE_SIZE > _file_size){
		fprintf (stderr, "read error, out of bound\n");
		return -1;
	}

	fseek( _fp, pageNum * PF_PAGE_SIZE, SEEK_SET);
	if ( 1 != fread( data, PF_PAGE_SIZE, 1, _fp)){
		fprintf (stderr, "read error, fread failed\n");
		return -1;
	}
    return 0;
}


RC PF_FileHandle::WritePage(PageNum pageNum, const void *data)
{
	CHECK_NULL (data);
	CHECK_NULL (_fp);

	if ( (pageNum) * PF_PAGE_SIZE > _file_size){
		fprintf (stderr, "write error, out of bound\n");
		return -1;
	}

	fseek( _fp, pageNum * PF_PAGE_SIZE, SEEK_SET);
	if ( 1 != fwrite( data, PF_PAGE_SIZE, 1, _fp)){
		fprintf (stderr, "write error, fwrite failed\n");
		return -1;
	}
    return 0;
}


RC PF_FileHandle::AppendPage(const void *data)
{
	CHECK_NULL (data);
	CHECK_NULL (_fp);

	fseek( _fp, 0, SEEK_END);
	if ( 1 != fwrite( data, PF_PAGE_SIZE, 1, _fp)){
		fprintf (stderr, "write error, fwrite failed\n");
		return -1;
	}
	_file_size += PF_PAGE_SIZE;
    return 0;
}


unsigned PF_FileHandle::GetNumberOfPages()
{
    return (_file_size > 0 ? (unsigned) _file_size / PF_PAGE_SIZE: 0);
}



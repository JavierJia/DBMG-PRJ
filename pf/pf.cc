#include <sys/stat.h>
#include "pf.h"
#include "pf_filenode.h"
#include "util/logger.h"
#include "util/io.h"

using namespace util;

#define CHECK_NULL(pt)    { \
    if ( pt == NULL){    \
        fprintf (stderr, "null value\n"); \
        return -1;}}

PF_Manager* PF_Manager::_pf_manager = NULL;

PF_Manager* PF_Manager::Instance()
{
    if(!_pf_manager)
        _pf_manager = new PF_Manager();
    
    return _pf_manager;    
}

PF_Manager::PF_Manager():_pf_log( ".storedfilelist.dat")
{
    _pf_filelist = new PF_FileList();
}

PF_Manager::~PF_Manager()
{
    if (_pf_filelist){
        delete _pf_filelist;
        _pf_filelist = NULL;
    }
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

    // add the FILE description into filelist
    PF_FileNode filenode(fileName, PF_FileStatus(1));
    RC rc = _pf_filelist->AppendNode(filenode);

    // store on disk in case of failure
    if ( rc == 0){
        fp = fopen( _pf_log, "wb");
        if (!fp){
            Logger::Warn(1, "could write on the log file:%s ", _pf_log);
            perror("");
        }
        _pf_filelist->StoreInto(fp);
        fclose(fp);
    }
    return rc;
}


RC PF_Manager::DestroyFile(const char *fileName)
{
    CHECK_NULL(fileName);
    struct stat buffer;
    if ( stat( fileName, &buffer) != 0){
        perror ("file may not exist");
        return -1;
    }

    // delete the file record from pf_filelist;
    if (_pf_filelist->DeleteNode(PF_FileNode(fileName, PF_FileStatus())) != 0){
        Logger::Error(1, "%s is not under control\n", fileName);
        return -1;
    }

    if ( remove ( fileName ) != 0){
        perror ( "deleting error");
            return -1;
    }
    
    // store on disk in case of failure
    {
        FILE* fp = fopen( _pf_log, "wb");
        if (!fp){
            Logger::Warn(1, "could write on the log file:%s ", _pf_log);
            perror("");
        }
        _pf_filelist->StoreInto(fp);
        fclose(fp);
    }
 
    return 0;
}


RC PF_Manager::OpenFile(const char *fileName, PF_FileHandle &fileHandle)
{
    //TODO: check out the no. of opened files to under the limit of system ulimit
    // http://pubs.opengroup.org/onlinepubs/7908799/xsh/getrlimit.html
    CHECK_NULL(fileName);
    
    const PF_FileNode* node = _pf_filelist->Contains(PF_FileNode(fileName, PF_FileStatus()));
    if (node == NULL){
        Logger::Error(1, "%s is not under control\n", fileName);
        return -1;
    }

    FILE    *fp;                    

    fp    = fopen( fileName, "rb+" );
    if ( fp == NULL ) {
        perror ( "opening error ");
        return -1;
    }

    RC rc = fileHandle.AttachFILE(fp, node);
    if (rc == 0){
        node->OpenOnce();
    }
    return rc;
}


RC PF_Manager::CloseFile(PF_FileHandle &fileHandle)
{
    //FILE * fp = fileHandle.GetFILE();
    const PF_FileNode* node = NULL;
    FILE * fp = fileHandle.GetAndResetFile(&node);
    
    CHECK_NULL(fp);
    fflush(fp);
    if ( fclose(fp) == EOF){
        perror( "closing error ");
        return -1;
    }
    int opened = node->CloseOnce();
    if (opened < 0){
        Logger::Warn(1, "openned time less than 0!\n");
    }
    return 0;
}

RC PF_Manager::RecoverfromLog(){                                                // Recover from the log which stored the filelist 
    if (!FileExists( _pf_log)){
        Logger::Warn(1,"log file %s does not exist\n", _pf_log);
        return -1;
    }
    FILE* fp = fopen( _pf_log, "rb");
    if (!fp){
        Logger::Error(1,"log file %s open error:", _pf_log);
        perror("");
        return -1;
    }
    fclose(fp);
    return _pf_filelist->ReadFrom(fp);
}

////////////////////////////////////////////////////////////////
PF_FileHandle::PF_FileHandle(): _fp(NULL),_filenode(NULL), _total_pages(0)
{
}
 

PF_FileHandle::~PF_FileHandle()
{
}

FILE* PF_FileHandle::GetAndResetFile(const PF_FileNode ** ptr_ptr_node){
    FILE* tfp = _fp;
    _fp = NULL;
    *ptr_ptr_node = _filenode;
    return tfp;
}

RC PF_FileHandle::AttachFILE(FILE * fp, const PF_FileNode* fnode){
    if ( _fp){
        fprintf(stderr, "file handle attaching error: filehandle already assigned");
        return -1;
    }
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
    _filenode = fnode;
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



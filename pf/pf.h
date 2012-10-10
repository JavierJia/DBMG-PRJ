#ifndef _pf_h_
#define _pf_h_

#include <cstdio>
#include <cstdlib>

#include "pf_filenode.h"
#include "pf_filelist.h"

typedef int RC;
typedef unsigned PageNum;

#define PF_PAGE_SIZE 4096

class PF_FileHandle;


class PF_Manager
{
public:
    static PF_Manager* Instance();                                      // Access to the _pf_manager instance
       
    RC CreateFile    (const char *fileName);                            // Create a new file
    RC DestroyFile   (const char *fileName);                            // Destroy a file
    RC OpenFile      (const char *fileName, PF_FileHandle &fileHandle); // Open a file
    RC CloseFile     (PF_FileHandle &fileHandle);                       // Close a file 

    RC RecoverfromLog();                                                // Recover from the log which stored the filelist 
                                                                        //      contained all the files that created
protected:    
    PF_Manager();                                                       // Constructor
    ~PF_Manager   ();                                                   // Destructor

private:
    static PF_Manager *_pf_manager;
    PF_FileList *_pf_filelist;                                          // file list created by CreateFile
    const char* _pf_log ;                        // file to store the list
};


class PF_FileHandle
{
    friend class PF_Manager;
public:
    PF_FileHandle();                                                    // Default constructor
    ~PF_FileHandle();                                                   // Destructor

    RC ReadPage(PageNum pageNum, void *data);                           // Get a specific page
    RC WritePage(PageNum pageNum, const void *data);                    // Write a specific page
    RC AppendPage(const void *data);                                    // Append a specific page
    unsigned GetNumberOfPages();                                        // Get the number of pages in the file

protected:
    RC AttachFILE (FILE * fp, const PF_FileNode * fnode);               // Attach the opened FILE to handle
    FILE* GetAndResetFile(const PF_FileNode** );                              // Give the FILE to other handler, and nolonger attatch to that any more
private:
    FILE    *_fp;                                                       // FILE pointer 
    const PF_FileNode *_filenode;                                       // FILE properties 
    PageNum _total_pages;                                               // Total pages of _fp
};
 
#endif

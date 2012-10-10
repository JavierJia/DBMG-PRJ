/*
 * =====================================================================================
 *
 *       Filename:  pf_filelist.h
 *
 *    Description:  implement a filelist using linkedlist. Can store and recover from 
 *                  a file
 *
 *        Version:  1.0
 *        Created:  10/09/2012 18:08:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfeng Jia (Javier), jianfeng.jia@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _PF_FILELIST_H_
#define _PF_FILELIST_H_

#include "util/linked_list.h"
#include "pf_filenode.h"

typedef util::LinkedList<PF_FileNode> t_linklist;

class PF_FileList: public t_linklist{

public:
    PF_FileList();
    PF_FileList(FILE* fp);
    
    ~PF_FileList();

    int StoreInto(FILE* fp)const;
    int ReadFrom(FILE* fp);
};

#endif

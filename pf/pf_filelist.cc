/*
 * =====================================================================================
 *
 *       Filename:  pf_filelist.cc
 *
 *    Description:  implement of pf_filelist
 *
 *        Version:  1.0
 *        Created:  10/09/2012 18:13:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfeng Jia (Javier), jianfeng.jia@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include "pf_filelist.h"

PF_FileList::PF_FileList():t_linklist(){
}

PF_FileList::PF_FileList(FILE* fp):t_linklist(){
    ReadFrom(fp);
}
    
PF_FileList::~PF_FileList(){
    Delete();
}

int PF_FileList::StoreInto(FILE* fp)const{
    if ( IsEmpty()){
        return 0;
    }
    t_linklist::Node* pre = head;
    t_linklist::Node* cur = pre->next;
    while(cur){
        if ( 1 != fwrite( cur, sizeof(PF_FileNode), 1, fp)){
            perror("pf_filelist store file error");
            return -1;
        }
        pre = cur;
        cur = pre-> next;
    }
    return 0;
}

int PF_FileList::ReadFrom(FILE* fp){
    PF_FileNode* node = new PF_FileNode();
    while ( 1 == fread( node, sizeof(PF_FileNode), 1, fp)){
        if( AppendNode(*node) != 0){
            delete node;
            return -1;
        }
    }
    delete node;
    return 0;
}


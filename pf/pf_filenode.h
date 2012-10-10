/*
 * =====================================================================================
 *
 *       Filename:  pf_filenode.h
 *
 *    Description:  a file list storing each of Created file by PF_Manager
 *
 *        Version:  1.0
 *        Created:  10/08/2012 22:45:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfeng Jia (Javier), jianfeng.jia@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _pf_filenode_h_
#define _pf_filenode_h_

#include <cstring>
#include <sys/param.h>

struct PF_FileStatus{
    int Open;

    PF_FileStatus(int open=1)
        :Open(open)
    {}

    PF_FileStatus(const PF_FileStatus &status)
        :Open(status.Open)
    {}
};

class PF_FileNode {                             
public:
    PF_FileNode();
    PF_FileNode(const char* fpath, const PF_FileStatus &status);
    PF_FileNode(const PF_FileNode &node);
    ~PF_FileNode();
public:
    bool operator == (const PF_FileNode &node)const; /* meet the requirement of util linked_list */

    int OpenOnce()const;                        /* keep record on opened times */
    int CloseOnce()const;                       /* keep record on closed times */
private:
    char m_path[ MAXPATHLEN];                   /* fileName */
    mutable PF_FileStatus m_status;             /* some status, could keep on change with to meet some special log need*/
};
#endif

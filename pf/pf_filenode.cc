/*
 * =====================================================================================
 *
 *       Filename:  pf_filenode.cc
 *
 *    Description:  implement of filenode
 *
 *        Version:  1.0
 *        Created:  10/09/2012 00:44:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfeng Jia (Javier), jianfeng.jia@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include <cassert>
#include "pf_filenode.h"

PF_FileNode::PF_FileNode()
:m_status(){
    m_path[0] = 0;
}

PF_FileNode::~PF_FileNode(){
}

PF_FileNode::PF_FileNode(const PF_FileNode &node)
    :m_status(node.m_status)
{
    strcpy( m_path, node.m_path);
}

PF_FileNode::PF_FileNode(const char* fpath, const PF_FileStatus &status)
    :m_status(status)
{
    assert( strlen(fpath) < MAXPATHLEN);
    strcpy( m_path, fpath);
}

bool PF_FileNode::operator == (const PF_FileNode &node)const{
    return strcmp( m_path, node.m_path) == 0;
}

int PF_FileNode::OpenOnce()const{
    m_status.Open +=1;
    return m_status.Open;
}

int PF_FileNode::CloseOnce()const{
    if (m_status.Open > 0){
        m_status.Open -= 1;
    }
    return m_status.Open;
}


/*
 * =====================================================================================
 *
 *       Filename:  IO.h
 *
 *    Description:  Write some IO oriented functions
 *
 *        Version:  1.0
 *        Created:  10/08/2012 10:58:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfeng Jia (Javier), jianfeng.jia@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __UTIL_IO_H__
#define __UTIL_IO_H__
#include <cstdio>
#include <sys/stat.h>

namespace util{
    
bool FileExists ( const char* fileName )        /* test if file exist */
{
    struct stat stFileInfo;
    if ( stat(fileName, &stFileInfo)){
        return false;
    }
    return true;
}        /* -----  end of function FileExist  ----- */


};
#endif

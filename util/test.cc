/*
 * =====================================================================================
 *
 *       Filename:  test.cc
 *
 *    Description:  Simple test for utils
 *
 *        Version:  1.0
 *        Created:  10/08/2012 14:44:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfeng Jia (Javier), jianfeng.jia@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cassert>

#include "logger.h"
#include "io.h"
#include "linked_list.h"

using namespace util;

int test_FILE_ptr(){
    const char* filename = "testifequal.txt";
    FILE* fp = fopen( filename, "w");
    Logger::Log(0,"file ptr open write mode is:%ld fileno:%d\n" ,fp, fileno(fp));
    FILE* fp2 = fopen( filename, "r");
    Logger::Log(0,"file ptr 2 open write mode is:%ld fileno:%d\n" ,fp2, fileno(fp2));
    fclose(fp);
    Logger::Log(0,"file ptr close write mode is:%ld fileno:%d\n" ,fp, fileno(fp));
    filename = "/Users/jiajianfeng/workplace/code/github/DBMG-PRJ/util/testifequal.txt";
    fp = fopen( filename, "r");
    Logger::Log(0,"file ptr open read mode is:%ld fileno:%d\n" ,fp, fileno(fp));
    fclose(fp);
    Logger::Log(0,"file ptr close read mode is:%ld fileno:%d\n" ,fp, fileno(fp));
    remove(filename);
    return 0;
}

int test_linked_list(){
    typedef LinkedList<int> int_linklist;
    int_linklist  list;

    // test if empty
    assert(list.IsEmpty());
    Logger::Log(0,"empty passed\n");

    list.AppendNode(4);
    assert(!list.IsEmpty());
    Logger::Log(0,"add passed\n");

    assert(list.Contains(4));
    Logger::Log(0,"constains passed\n");

    assert(!list.Contains(0));
    Logger::Log(0,"not constains passed\n");

    list.DeleteNode(4);
    assert(list.IsEmpty());
    Logger::Log(0,"delete passed\n");

    Logger::Log(0,"all passed\n");
    return 0;
}

int main (int argc, const char* argv[]){
    Logger::Log(0, "test normal log\n");
    char filename[256] = "test.io";

    if (argc > 1 && argv[1]){
        memcpy(filename, argv[1], 256 );
    }
    if (FileExists( filename)){
        Logger::Log(0, "%s exists\n", filename);
    }else{
        Logger::Warn(0, "%s not exists\n", filename);
    }

    for(int i=0;i < 2;i++){
        test_FILE_ptr();
    }

    test_linked_list();
    return 0;
}    /* end of main */

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

#include "logger.h"
#include "io.h"

using namespace util;

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
	return 0;
}	/* end of main */

#include <cstdarg>
#include <cstdio>
#include "logger.h"

using namespace util;

////////////////////////////////////////////////////////////////////////////////

#ifndef DEBUG
    int  Logger::_verbosity = 0;
    bool Logger::_timestamp = false;
#else
    int  Logger::_verbosity = 1;
    bool Logger::_timestamp = true;
#endif
clock_t Logger::_startTime = clock();

void Logger::Log(int level, const char *fmt, ...) {
    if (_verbosity >= level) {
        va_list args;
        va_start(args, fmt);
        if (_timestamp) 
            printf("%.3f\t", (float)(clock() - _startTime) / CLOCKS_PER_SEC);
        vprintf(fmt, args);
        va_end(args);
    }
}

void Logger::Warn(int level, const char *fmt, ...) {
    if (_verbosity >= level) {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "\e[0;33m");
        if (_timestamp) 
            fprintf(stderr,"%.3f\t", (float)(clock() - _startTime) / CLOCKS_PER_SEC);
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\e[m");
        va_end(args);
    }
}

void Logger::Error(int level, const char *fmt, ...) {
    if (_verbosity >= level) {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "\e[1;31m");
        if (_timestamp) 
            fprintf(stderr,"%.3f\t", (float)(clock() - _startTime) / CLOCKS_PER_SEC);
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\e[m");
        va_end(args);
    }
}

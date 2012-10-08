#ifndef __UTIL_LOGGER_H__
#define __UTIL_LOGGER_H__

#include <cstdio>
#include <ctime>

namespace util{
class Logger {
private:
    static int     _verbosity;
    static bool    _timestamp;
    static clock_t _startTime;

public:
    static inline void SetVerbosity(int verbosity=1) { _verbosity = verbosity; }
    static inline void ShowTimestamp(bool timestamp) { _timestamp = timestamp; }
    static inline int  GetVerbosity() { return _verbosity; }
    static void        Log(int level, const char *fmt, ...);
    static void        Warn(int level, const char *fmt, ...);
    static void        Error(int level, const char *fmt, ...);
};

};
#endif // LOGGER_H

#include "util.hpp"
#include <cstdlib>
#include <cstdarg>
#include <cstdio>

namespace util
{
    void die(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\n");
        va_end(args);
        exit(EXIT_FAILURE);
    }

    void err(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

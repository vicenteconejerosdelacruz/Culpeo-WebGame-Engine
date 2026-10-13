#pragma once

#if !defined(_WIN32) && !defined(WIN32)
typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef int BOOL;
typedef int INT;
typedef unsigned char BYTE;
typedef float FLOAT;
typedef bool BOOLEAN;

typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;
#endif
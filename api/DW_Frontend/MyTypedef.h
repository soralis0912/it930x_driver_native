#ifndef __MYTYPEDEF_H__
#define __MYTYPEDEF_H__

typedef unsigned char UINT8;

typedef unsigned short UINT16;

typedef unsigned int UINT32;

typedef signed char INT8;

typedef signed short INT16;

typedef signed int INT32;

typedef unsigned int ULONG;
typedef unsigned short UWORD;
typedef unsigned char UBYTE;
typedef unsigned char* PUBYTE;

#ifndef NULL
#define NULL	0
#endif

#ifndef BOOL
#define BOOL	int
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#endif // __MYTYPEDEF_H__
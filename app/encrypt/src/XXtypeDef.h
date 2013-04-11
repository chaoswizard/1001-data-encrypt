#ifndef __XTYPEDEF_H__
#define  __XTYPEDEF_H__

typedef unsigned char  BYTE;//8 bit
typedef unsigned short WORD;//16 bit
typedef unsigned long  DWORD;//32 bit
typedef unsigned int   UINT;

typedef DWORD          COLORREF;
typedef unsigned int   HANDLE;
typedef void*          HRGN;

#ifndef BOOL
#define	BOOL UINT
#endif

#ifndef TRUE
#define	TRUE  1
#endif

#ifndef FALSE
#define	FALSE  0
#endif


#ifndef NULL
#define NULL (void *)0
#endif


#endif


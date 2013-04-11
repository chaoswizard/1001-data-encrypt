#ifndef __MACRO_PUB_DEFINE_H__
#define  __MACRO_PUB_DEFINE_H__

#include "malloc.h"
#define   MALLOC(size)  malloc(size)
#define   FREE(ptr)     free(ptr)

//==========================================================
#include "XXByteStrm.h"
#define   FILE_READ(fp, buffer, len)	((fp)->Read(fp, buffer , 1, len))
#define   FILE_WRITE(fp, buffer, len)	((fp)->Write(fp, buffer, 1, len))
#define   FILE_SEEK_SET(fp, pos)        ((fp)->Seek(fp, pos, SEEK_SET))
#define   FILE_SEEK_END(fp, pos)        ((fp)->Seek(fp, pos, SEEK_END))
#define   FILE_TELL(fp)                 ((fp)->Tell(fp))
#define   FILE_CLOSE(fp)                ((fp)->Close(&(fp), TRUE))

#include "string.h"
#define   STRLEN(str)                    strlen(str)
#define   MEMCPY(d, s, l)                memcpy(d, s, l)

#endif


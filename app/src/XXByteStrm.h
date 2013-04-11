/*
*Copyright (c) 2009,(>WangXi<)
*All rights reserved.
*File Name:XXByteStrm.h/XXByteStrm.c
*Description:This file define a SimClass for ByteStream Operation
*version:1.0
*Author:chaoswizard@163.com(>WX<)
*Create Date:Dec.14th.2009
*/
#ifndef __XBYTESTRM_H__
#define  __XBYTESTRM_H__


#include "XXtypeDef.h"
#include <stdio.h>

#define   STRM_BUF_S   0x0 /*Static Alloc Array b000*/
#define   STRM_BUF_D  0x4/*Dynamic Alloc Array b100*/
#define   STRM_FILE_R  0x5/*Read existed File  and Append b101*/
#define   STRM_FILE_W  0x7/*Create new File and Append b111*/

struct BYTESTREAM {//
	union {
		BYTE  *Array;
		FILE   *fp;
	}handle;
	BYTE  m_Type:1;// 1 is  file, 0 isArray
	BYTE  m_Ew:1;// 1 is create new,0 is use existed
	BYTE  m_Einc:1;// 1 is  Append More,0 is can not
//-------member variable-----------
	DWORD	m_Size;
	long	m_Position;	//current position
	
	FILE *dumpFile;

//-------member interface-----------	
	void (*ShowAttri)(struct BYTESTREAM *,int line);
	DWORD (*GetSize)(struct BYTESTREAM *);
	long (*Tell)(struct BYTESTREAM *);
	BOOL (*Eof)(struct BYTESTREAM *);
	void  (*Seek)(struct BYTESTREAM *,long offset, int origin);

	int (*ReadByte)(struct BYTESTREAM *);
	int (*Read)(struct BYTESTREAM *,void *buffer, int size, int count);

	BOOL  (*PutC)(struct BYTESTREAM *stream,int c);
	int (*Write)(struct BYTESTREAM *,const void *buffer, int size, int count);
	
	void  (*Close)(struct BYTESTREAM **,BOOL destoryHandle);

	void (*Dump)(struct BYTESTREAM  *,int length);
	void  (*InitDumpFile)(struct BYTESTREAM  *,BYTE * name);
	void  (*CloseDumpFile)(struct BYTESTREAM  *);

};
////////////////////////////////////////////////////////////

//create one bytestream,MUST judge the return value,0 means failed else means successful.
BOOL  Create_ByteStrm(struct BYTESTREAM **stream,void *SrcData,DWORD Size,BYTE mode);
//void Print_HexStrm(FILE *dumpFile,BYTE *SrcData,int length);
//////////////////////////////////////////////////////////

#endif


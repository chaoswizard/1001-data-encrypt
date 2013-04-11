/*
*Copyright (c) 2009,(>WangXi<)
*All rights reserved.
*File Name:XXByteStrm.h/XXByteStrm.c
*Description:This file define a SimClass for ByteStream Operation
*version:1.0
*Author:chaoswizard@163.com(>WX<)
*Create Date:Dec.14th.2009
*/
#include <string.h>
#include <malloc.h>

#include "XXByteStrm.h"

//#define STRM_DEBUG




#define  STRM_ERROR  printf

#define STRM_DEBUG_IO  stderr

#define  BIT_STRM_TYPE  0
#define  BIT_STRM_EWT  1
#define  BIT_STRM_INC  2
//#define   STRM_BUF_S  /*Static Alloc Array b000*/
//#define   STRM_BUF_D  (0x1<<BIT_STRM_INC)/*Dynamic Alloc Array b100*/
//#define   STRM_FILE_R  ((0x1<<BIT_STRM_TYPE)|(0x1<<BIT_STRM_INC))/*Exist File read and append b101*/
//#define   STRM_FILE_W  ((0x1<<BIT_STRM_TYPE)|(0x1<<BIT_STRM_EWT)|(0x1<<BIT_STRM_INC))/*Not Exist File read and append b111*/


#define   MASK_STRM_TYPE   (0x1<<BIT_STRM_TYPE)
#define   MASK_STRM_EWT  (0x1<<BIT_STRM_EWT)
#define   MASK_STRM_INC  (0x1<<BIT_STRM_INC)

//============= Public  Interface Define =============
DWORD Stream_GetSize(struct st_byte_stream  *stream)
{
	//if (NULL == stream){return 0;}
	return  stream->m_Size;
}

long Stream_Tell(struct st_byte_stream *stream)
{
	//if (NULL == stream){return 0;}
	return stream->m_Position;//(stream->m_Position = ftell(stream->handle.fp));
}

BOOL Stream_Eof(struct st_byte_stream *stream)
{
	//if (NULL == stream){return TRUE;}
	return (stream->m_Position >= (long)stream->m_Size);
}

void  Stream_ShowAttri(struct st_byte_stream  *stream,int Line)
{
	//if (NULL == stream){return;}
#ifdef STRM_DEBUG
	printf(">>>%-d: (T=%X.W=%X.I=%X)[%d Byte]\tCurPos = %d\n",\
		Line,stream->m_Type,stream->m_Ew,stream->m_Einc,\
		stream->m_Size,stream->m_Position);
	if (stream->Eof(stream)) printf("\t----End----\n");
#endif
}

void  Stream_InitDumpFile(struct st_byte_stream  *stream,BYTE * name)
{
	char DumpFileName[32];

	//if ((NULL == stream)){return;}

	if(NULL == name) {
		stream->dumpFile = STRM_DEBUG_IO;
	} else {
	sprintf(DumpFileName,"Debug.%s",name);
	
#ifdef STRM_DEBUG
	stream->dumpFile = fopen(DumpFileName,"wt+");
#else
	stream->dumpFile = fopen(DumpFileName,"wb+");
#endif
	}
}

void  Stream_CloseDumpFile(struct st_byte_stream  *stream)
{
	//if ((NULL == stream)){return;}
	if(NULL==stream->dumpFile) {
		return;
	}
	if(STRM_DEBUG_IO != stream->dumpFile) {
		fclose(stream->dumpFile);
	}
}

void Stream_Dump(struct st_byte_stream  *stream,int length)
{
	BYTE  ch;
	long curPos = 0,savePos=0;
#ifdef STRM_DEBUG
	int Idx = 0;
#endif	
	
	//if ((NULL == stream)){return;}
	
	if ((0 == length)||(NULL==stream->dumpFile)) {return ;}

	savePos=curPos = stream->Tell(stream);

#ifdef STRM_DEBUG
	Idx=17;
	fprintf(stream->dumpFile,"\n");
	do{fprintf(stream->dumpFile,"===");}while(Idx--);
	Idx=0;
	fprintf(stream->dumpFile,"\n%6c",' ');
	do{fprintf(stream->dumpFile," %2x",Idx);}while((Idx++)<15);
	Idx=17;
	fprintf(stream->dumpFile,"\n");
	do{fprintf(stream->dumpFile,"---");}while(Idx--);
	Idx=0;
	if ((stream->GetSize(stream))<=0) {
		fprintf(stream->dumpFile,"\n\t\t\t无数据");
		Idx=17;
		fprintf(stream->dumpFile,"\n");
		do{fprintf(stream->dumpFile,"===");}while(Idx--);
		fprintf(stream->dumpFile,"\n");
		return;
	}
#endif
	if (-1 == length) {//print all
		while(!(stream->Eof(stream))) {
#ifdef STRM_DEBUG
			curPos = stream->Tell(stream);
			if ((0==Idx)&&(curPos%16!= 0) ){
				fprintf(stream->dumpFile,"\n%05X:",curPos-(curPos%16));
				Idx = curPos%16;
				while(Idx--){fprintf(stream->dumpFile,"%3c",' ');}
			} 

			if(curPos%16== 0){
				fprintf(stream->dumpFile,"\n%05X:",curPos);
			}
			Idx=16;
#endif
			ch=stream->ReadByte(stream);
#ifdef STRM_DEBUG
			fprintf(stream->dumpFile," %02X",ch);
#else
			fwrite(&ch,1,1,stream->dumpFile);
#endif
		}
	} else  {//print length cell
		while((!(stream->Eof(stream)))&&(length>0)){
#ifdef STRM_DEBUG
			curPos = stream->Tell(stream);
			if ((0==Idx)&&(curPos%16!= 0) ){
				fprintf(stream->dumpFile,"\n%05X:",curPos-(curPos%16));
				Idx = curPos%16;
				while(Idx--){fprintf(stream->dumpFile,"%3c",' ');}
			} 

			if(curPos%16== 0){
				fprintf(stream->dumpFile,"\n%05X:",curPos);
			}
			Idx=16;
#endif

			ch=stream->ReadByte(stream);
#ifdef STRM_DEBUG
			fprintf(stream->dumpFile," %02X",ch);
#else
			fwrite(&ch,1,1,stream->dumpFile);
#endif
			length--;
		}
	} 

#ifdef STRM_DEBUG
	Idx=17;
	fprintf(stream->dumpFile,"\n");
	do{fprintf(stream->dumpFile,"===");}while(Idx--);
	fprintf(stream->dumpFile,"\n");
#endif
	stream->Seek(stream,savePos,SEEK_SET);
}
//-----------------------End-----------------------


//========== Private  Interface Define For File ==========
void  File_Close(struct st_byte_stream **  stream,BOOL destoryHandle)
{
	//if (NULL == (*stream)) {return;}
	
	if (destoryHandle) {
		if(NULL!=((*stream)->handle.fp)) {
			fclose((*stream)->handle.fp);
		}
	}

	free((*stream));
	(*stream) = NULL;
}


void File_Seek(struct st_byte_stream *stream,long offset, int origin)
{
	long lNewPos = 0;
	
	//if (NULL == stream){return ;}

	if ( NULL== stream->handle.fp) {
		return ;
	}
	
	lNewPos = stream->m_Position;
	if (SEEK_SET == origin) {
		lNewPos = offset;
	} else if (SEEK_CUR == origin) {
		lNewPos += offset;
	} else if (SEEK_END == origin) {
		lNewPos = stream->m_Size+ offset;
	} 

	if (lNewPos < 0) {
		 lNewPos = 0;
	} else if (lNewPos >= (long)stream->m_Size) {
		lNewPos = stream->m_Size;
	}

	stream->m_Position= lNewPos;
	
	fseek(stream->handle.fp,stream->m_Position,SEEK_SET);

}


int File_ReadByte(struct st_byte_stream *stream)
{
	long  Pos = 0;
	
	//if (NULL == stream){return EOF;}

	if ( NULL== stream->handle.fp) {
		return EOF;
	}
	
	Pos = stream->m_Position;
	if ((Pos < 0)) {
		Pos = 0;
	} else if (Pos >= (long)stream->m_Size) {
		stream->m_Position = (long)stream->m_Size;
		return EOF;
	}

	stream->m_Position += 1; //= ftell(stream->handle.fp);
	return  fgetc(stream->handle.fp);
}



int File_Read(struct st_byte_stream *stream,void *buffer, int size, int count)
{
	long nCount = 0;
	long nRead = 0;

	//if (NULL == stream){return 0;}
	
	nCount = (long)(count*size);
	
	if ((nCount == 0)||( NULL== stream->handle.fp)||(NULL== buffer)) {
		return 0;
	}

	if (stream->m_Position >= (long)stream->m_Size) {
		return 0;
	}


	if ((stream->m_Position + nCount) > (long)(stream->m_Size))
		nRead = (stream->m_Size - stream->m_Position);
	else
		nRead = nCount;

	 
	stream->m_Position += nRead;

	return fread(buffer,  1, nRead, stream->handle.fp);//
}

BOOL  File_PutC(struct st_byte_stream *stream,int c)
{
	long  Pos = 0;

	//if (NULL == stream){return FALSE;}

	if ( NULL== stream->handle.fp) {
		return FALSE;
	}

	Pos = stream->m_Position;
	if (EOF!=fputc(c,stream->handle.fp)) {
		stream->m_Position += 1; 
		if (stream->m_Position > (long)stream->m_Size) {
			stream->m_Size = stream->m_Position;
		} 
		return TRUE;
	} else {
		return FALSE;
	}
}


int File_Write(struct st_byte_stream *stream,const void *buffer, int size, int count)
{
	long nCount = 0;
	long nWrite = 0;

	//if (NULL == stream){return 0;}

	nCount = (long)(count*size);
	
	if ((nCount == 0)||( NULL== stream->handle.fp)||(NULL== buffer)) {
		return 0;
	}

	nWrite = fwrite(buffer,  1, nCount, stream->handle.fp);
	
	stream->m_Position += nWrite;
	
	if (stream->m_Position > (long)stream->m_Size) {
		stream->m_Size = stream->m_Position;
	} 

	return nWrite;
}

//-----------------------End-----------------------


//========== Private  Interface Define For Buffer ==========

void  Buf_Close(struct st_byte_stream **  stream,BOOL destoryHandle)
{
	//if (NULL == (*stream)) {return;}
	
	free((*stream));
	(*stream) = NULL;

}
void  Buf_Destory(struct st_byte_stream **  stream,BOOL destoryHandle)
{
	//if (NULL == (*stream)) {return;}

	if (destoryHandle) {
		if (NULL != ((*stream)->handle.Array)) {
			free((*stream)->handle.Array);
			(*stream)->handle.Array = NULL;
		}
	}
	
	free((*stream));
	(*stream) = NULL;
}


void  Buf_Seek(struct st_byte_stream *stream,long offset, int origin)
{
	long lNewPos = 0;

	//if (NULL  == stream) {return;}
	
	lNewPos = stream->m_Position;

	if (SEEK_SET == origin) {
		lNewPos = offset;
	} else if (SEEK_CUR == origin) {
		lNewPos += offset;
	} else if (SEEK_END == origin) {
		lNewPos = stream->m_Size+ offset;
	} 
	

	if (lNewPos < 0){
		lNewPos = 0;
	}else if (lNewPos >= (long)stream->m_Size) {
		lNewPos = stream->m_Size;
	}
	
	stream->m_Position= lNewPos;
}


int Buf_ReadByte(struct st_byte_stream *stream)
{
	long  Pos = 0;

	//if (NULL  == stream) {return EOF;}
	
	if ( NULL== stream->handle.Array) {
		return EOF;
	}

	Pos = stream->m_Position;
	
	if ((Pos < 0)) {
		Pos = 0;
	} else if (Pos >= (long)stream->m_Size) {
		stream->m_Position = (long)stream->m_Size;
		return EOF;
	}

	stream->m_Position += 1;
	
	return (int)(stream->handle.Array[Pos]);
}

int Buf_Read(struct st_byte_stream *stream,void *buffer, int size, int count)
{
	long nCount = 0;
	long nRead = 0;
	
	//if (NULL  == stream) {return 0;}

	nCount = (long)(count*size);
	
	if ((nCount == 0)||( NULL== stream->handle.Array)||(NULL== buffer)) {
		return 0;
	}

	if (stream->m_Position >= (long)stream->m_Size) {
		return 0;
	}


	if ((stream->m_Position + nCount) > (long)(stream->m_Size))
		nRead = (stream->m_Size - stream->m_Position);
	else
		nRead = nCount;

	memcpy(buffer, stream->handle.Array + stream->m_Position, nRead);
	 
	stream->m_Position += nRead;

	return (int)(nRead/size);
}



BOOL  Buf_PutC(struct st_byte_stream *stream,int c)
{
	long  Pos =  0;

	//if (NULL  == stream) {return FALSE;}
	
	Pos = stream->m_Position;
	if ((Pos +1) > (long)stream->m_Size) {
		if (!(stream->m_Einc)) {
			return FALSE;
		}
		if( NULL== stream->handle.Array) {
			stream->handle.Array=(BYTE*)malloc(Pos +1);
		} else {
			stream->handle.Array = (BYTE*)realloc(stream->handle.Array, Pos +1);
		}

		if (stream->handle.Array == NULL) {
			return FALSE;
		}
	}
	
	c &= 0xFF;
	memcpy(stream->handle.Array + Pos, &c, 1);
	
	stream->m_Position += 1;
	
	if (stream->m_Position > (long)stream->m_Size) {
		stream->m_Size = stream->m_Position;
	} 
	
	return TRUE;
}


int Buf_Write(struct st_byte_stream *stream,const void *buffer, int size, int count)
{
	long nCount = 0;
	long  Pos = 0;
	
	//if (NULL  == stream) {return 0;}

	nCount = (long)(count*size);
	
	if ((nCount == 0)||(NULL== buffer)) {
		return 0;
	}

	Pos = stream->m_Position;
	if ((Pos + nCount) > (long)stream->m_Size){
		
		if (!(stream->m_Einc)) {
			return 0;
		}

		if ( NULL== stream->handle.Array) {
			stream->handle.Array = (BYTE*)malloc(Pos +nCount);
		} else {
			stream->handle.Array = (BYTE*)realloc(stream->handle.Array, Pos +nCount);
		}

		if (stream->handle.Array == NULL) {
			return 0;
		}
	}

	memcpy(stream->handle.Array + Pos, buffer, nCount);

	stream->m_Position += nCount;
	
	if (stream->m_Position > (long)stream->m_Size) {
		stream->m_Size = stream->m_Position;
	} 
	
	return count;
}

//-----------------------End-----------------------


void Print_HexStrm(FILE *dumpFile,BYTE *SrcData,int length)
{
	int Idx = 0;
	
	if(NULL == dumpFile){
		dumpFile = STRM_DEBUG_IO;
	}
	
	if ((NULL == SrcData)){
		fprintf(dumpFile,"\nParameter Error!\n");
		return;
	}

	
	if (0 == length) {return ;}


#ifdef STRM_DEBUG
	Idx=17;
	fprintf(dumpFile,"\n");
	do{fprintf(dumpFile,"---");}while(Idx--);
	Idx=0;
	fprintf(dumpFile,"\n%6c",' ');
	do{fprintf(dumpFile," %2x",Idx);}while((Idx++)<15);
	Idx=17;
	fprintf(dumpFile,"\n");
	do{fprintf(dumpFile,"---");}while(Idx--);
	Idx=0;
	if (length<=0) {
		fprintf(dumpFile,"\n\t\t\t无数据");
		Idx=17;
		fprintf(dumpFile,"\n");
		do{fprintf(dumpFile,"---");}while(Idx--);
		fprintf(dumpFile,"\n");
		return;
	}
#endif

	 for (Idx=0;Idx<length;Idx++) {
#ifdef STRM_DEBUG
		if(Idx%16== 0){
			fprintf(dumpFile,"\n%05X:",Idx);
		}
		fprintf(dumpFile," %02X",SrcData[Idx]);
#else
		fwrite(&SrcData[Idx],1,1,dumpFile);
#endif
	}

#ifdef STRM_DEBUG
	Idx=17;
	fprintf(dumpFile,"\n");
	do{fprintf(dumpFile,"---");}while(Idx--);
	fprintf(dumpFile,"\n");
#endif
}

BOOL  open_bytestrm(struct st_byte_stream **stream,void *src_data_or_path,DWORD capacity, BYTE mode)
{
	if (NULL != (*stream)) {
		STRM_ERROR("Had created\n");
		return FALSE;
	}

	(*stream) = (struct st_byte_stream *)malloc(sizeof (struct st_byte_stream));

	if (NULL == (*stream) ) {
		STRM_ERROR("Create Error\n");
		return FALSE;
	}
//public member
	(*stream)->m_Type =(BYTE)(( mode & MASK_STRM_TYPE)>>BIT_STRM_TYPE);
	(*stream)->m_Ew =(BYTE)(( mode & MASK_STRM_EWT)>>BIT_STRM_EWT);
	(*stream)->m_Einc =(BYTE)(( mode & MASK_STRM_INC)>>BIT_STRM_INC);
	(*stream)->m_Position = 0;

//public interface
	(*stream)->GetSize = Stream_GetSize;
	(*stream)->Tell = Stream_Tell;
	(*stream)->Eof = Stream_Eof;
	(*stream)->ShowAttri = Stream_ShowAttri;
	(*stream)->Dump = Stream_Dump;
	(*stream)->InitDumpFile = Stream_InitDumpFile;
	(*stream)->CloseDumpFile = Stream_CloseDumpFile;
	
//private interface and member	
	if (((*stream)->m_Type)) {

		if ((*stream)->m_Ew) {
			(*stream)->handle.fp = fopen((BYTE *)src_data_or_path,"wb+");//delete old file and create new one
		} else {
			(*stream)->handle.fp = fopen((BYTE *)src_data_or_path,"rb+");//file must exist 
		}
		
		if (NULL == (*stream)->handle.fp) {
			STRM_ERROR("Cannot open File  %s\n",(BYTE *)src_data_or_path);
			free((*stream));//>WX<add
			(*stream) = NULL;//>WX<add
			return FALSE;
		} 
#ifdef STRM_DEBUG
		if(src_data_or_path) {
			STRM_ERROR("Create File  %s\n",(BYTE *)src_data_or_path);
		} 
#endif
		//stream->m_Size = filelength(stream->handle.fp) ;
		fseek((*stream)->handle.fp,0L,SEEK_END);
		(*stream)->m_Size = ftell((*stream)->handle.fp) ;
		rewind((*stream)->handle.fp);
		(*stream)->Close = File_Close;
		(*stream)->ReadByte = File_ReadByte;
		(*stream)->Read = File_Read;
		
		(*stream)->PutC = File_PutC;
		(*stream)->Write = File_Write;
		
		(*stream)->Seek = File_Seek;
	}  else   {
	
		(*stream)->m_Size = capacity;
		if ((*stream)->m_Einc) {
			if ((*stream)->m_Size > 0) {
				(*stream)->handle.Array =  (BYTE*)malloc(capacity);
				if (NULL != ((*stream)->handle.Array)) {
					memset(((*stream)->handle.Array),0,capacity);
				}
			} else {
				(*stream)->handle.Array = NULL;
			}
			(*stream)->Close =Buf_Destory;
		} else {
			(*stream)->handle.Array =  (BYTE *)(src_data_or_path);
			(*stream)->Close =Buf_Close;
		}


		if ((*stream)->m_Size > 0) {
			if (NULL == ((*stream)->handle.Array)) {
				STRM_ERROR("Cannot fetch Array \n");
				free((*stream));//>WX<add
				(*stream) = NULL;//>WX<add
				return FALSE;
			} 
		}

#ifdef STRM_DEBUG
		if (src_data_or_path) {
			STRM_ERROR("Create Array  %02X\n",((BYTE *)src_data_or_path)[0]);
		} else {
			STRM_ERROR("Create Array_Empty\n");
		}
#endif
		(*stream)->ReadByte = Buf_ReadByte;
		(*stream)->Read = Buf_Read;
		
		(*stream)->PutC = Buf_PutC;
		(*stream)->Write = Buf_Write;
		
		(*stream)->Seek = Buf_Seek;

	}
	
	return TRUE;
}



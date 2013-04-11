#include "XXByteStrm.h"
#include "string.h"
#include "malloc.h"

#define MY_DEBUG  printf

#if 0
#define FILE_NAME  "test"
#define KEY_CODE   "123485"
#define SHOW_FILE_CMD    ""
#else
#define FILE_NAME        argv[1]
#define KEY_CODE         argv[2]
#define SHOW_FILE_CMD    argv[3]
#endif

#define TMP_FILE_NAME "JM_RTX.txt"

#define MALLOC(size)  malloc(size)
#define FREE(ptr)     free(ptr)

#define   FILE_READ(fp,buffer,len)	(fp->Read(fp,buffer , 1,len ))
#define   FILE_WRITE(fp,buffer,len)	(fp->Write(fp,buffer, 1,len ))
#define   FILE_SEEK_SET(fp,pos)     (fp->Seek(fp,pos,SEEK_SET))
#define   FILE_SEEK_END(fp,pos)     (fp->Seek(fp,pos,SEEK_END))
#define   FILE_TELL(fp)             (fp->Tell(fp))
#define   FILE_CLOSE(fp)            (fp->Close(&fp,TRUE))


BOOL Src_Stream_Init(struct BYTESTREAM **srcStrm,char *srcName)
{
	struct BYTESTREAM    *srcStream = NULL;
	int strlength=0;
	
	//尝试以文件方式初始化
	if (Create_ByteStrm(&srcStream,srcName , 0, STRM_FILE_R)) 
	{
		*srcStrm = srcStream;
		return TRUE;
	} 
	
	strlength = strlen(srcName);
	//尝试以写方式打开
	if(!Create_ByteStrm(&srcStream,TMP_FILE_NAME, strlength, STRM_FILE_W))
	{
		return FALSE;
	}
	else
	{
		FILE_SEEK_SET(srcStream,0);
		if(strlength != FILE_WRITE(srcStream,srcName,strlength))
		{
			FILE_CLOSE(srcStream);
			return FALSE;
		}
		else
		{
			FILE_SEEK_SET(srcStream,0);
			*srcStrm = srcStream;
			return TRUE;
		}
	}
}


//============================
int main(int argc,char **argv)
{
	struct BYTESTREAM    *fp=NULL;
	char *srcName = NULL,*strKey = NULL;
	unsigned char *keybuffer,*filebuffer; 
	unsigned int keylen=0,tmp=0,bufferlen=0;
	long int pos = 0,fileLen = 0;
	
	if (FILE_NAME)
	{
		if (KEY_CODE)
		{
			strKey  = KEY_CODE;
		}
		else
		{
			MY_DEBUG("Param Er @%d\n",__LINE__);
			return 0;
		}
	}
	else
	{
		MY_DEBUG("Param Er @%d\n",__LINE__);
		return 0;
	}
	
	if (!Src_Stream_Init(&fp,FILE_NAME))
	{
		MY_DEBUG("Init Er @%d\n",__LINE__);
		return 0;
	}
	if (fp->m_Ew)
	{
		srcName = TMP_FILE_NAME;
	}
	else
	{
		srcName = FILE_NAME;
	}
	
	MY_DEBUG("@%s%s @Key: %s\n",
		fp->m_Ew?"Create":"Open",
		fp->m_Type?"File:":"String:\n\t",
		srcName,
		strKey);
	//MY_DEBUG("begin...\n");
	
	
	keylen = strlen(KEY_CODE);
	keybuffer = MALLOC(keylen+1);
	if (NULL == keybuffer)
	{
		MY_DEBUG("Malloc Er @%d\n",__LINE__);
		FILE_CLOSE(fp); 
		return 0;	
	}
    memcpy(keybuffer,strKey,keylen);
	
	filebuffer = MALLOC(keylen+1);
    if (NULL == filebuffer)
	{
		MY_DEBUG("Malloc Er @%d\n",__LINE__);
		FREE(keybuffer);
		keybuffer = NULL;
		FILE_CLOSE(fp); 
		return 0;	
	}
	
	FILE_SEEK_END(fp,0);
	fileLen = FILE_TELL(fp);
	
	pos = 0;
	while(pos < fileLen) {
		FILE_SEEK_SET(fp,pos);
		tmp = FILE_READ(fp,filebuffer,keylen);
		//MY_DEBUG("R.(%d %d),[%d %d],%d  ",pos, FILE_TELL(fp),keylen,tmp,fileLen);
		if (0 == tmp)
		{
			bufferlen = 0;
			break;
		}
		else 
		{
			bufferlen = tmp;
		}
		
		tmp=0;
		for (tmp=0;tmp<bufferlen;tmp++)
		{
			filebuffer[tmp] ^=keybuffer[tmp];
		} 
		FILE_SEEK_SET(fp,pos);
		tmp = FILE_WRITE(fp,filebuffer,bufferlen);
		
		//MY_DEBUG("W.(%d %d),[%d %d],%d\n",pos, FILE_TELL(fp),bufferlen,tmp,fileLen);
		if (bufferlen != tmp)
		{
			break;
		}
		pos += tmp;
	}

	FILE_CLOSE(fp); 
	//MY_DEBUG("\nOk\n");
	
	FREE(keybuffer);
	keybuffer = NULL;
	FREE(filebuffer);
	filebuffer = NULL;

	if (SHOW_FILE_CMD)
	{
		char cmdbuffer[256];
		MY_DEBUG("\nShow %s:\n",srcName);
		sprintf(cmdbuffer,"%s %s",SHOW_FILE_CMD,srcName);
		system(cmdbuffer);
		//MY_DEBUG("^_^\n");
	}
	
	return 0;
}

#include "Encrypt.h"

// if test
#if 0
#define SRC_FILE_PATH  "test"
#define KEY_CODE   "123485"
#define SHOW_FILE_CMD    ""
#else
#define SRC_FILE_PATH    argv[1]
#define DST_FILE_PATH    argv[1]
#define KEY_CODE         argv[2]
#define SHOW_FILE_CMD    argv[3]
#endif

#define MY_DEBUG  printf


int main(int argc,char **argv)
{
	struct st_byte_stream *src = NULL;
	char *src_path = NULL, *encrypt_key_str = NULL;
	unsigned char *key_buffer; 
	unsigned int key_len = 0;
	long int file_len = 0;
	
	if (SRC_FILE_PATH)
	{
		if (KEY_CODE)
		{
			encrypt_key_str  = KEY_CODE;
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

    src = encrypt_open(SRC_FILE_PATH, &src_path);
	if (!src)
	{
		MY_DEBUG("Init Er @%d\n",__LINE__);
		return 0;
	}
    
	MY_DEBUG("@%s%s @Key: %s\n",
		src->m_Ew?"Create":"Open",
		src->m_Type?"File:":"String:\n\t",
		src_path,
		encrypt_key_str);
	//MY_DEBUG("begin...\n");
	
	
	key_len = STRLEN(KEY_CODE);
	key_buffer = MALLOC(key_len+1);
	if (NULL == key_buffer)
	{
		MY_DEBUG("Malloc Er @%d\n",__LINE__);
		encrypt_close(&src); 
		return 0;	
	}
    
    MEMCPY(key_buffer,encrypt_key_str,key_len);
	
	FILE_SEEK_END(src,0);
	file_len = FILE_TELL(src);
    FILE_SEEK_SET(src, 0);
    
    encrypt_stream(src, src, file_len, key_buffer, key_len);

	encrypt_close(&src); 
	//MY_DEBUG("\nOk\n");
	
	FREE(key_buffer);
	key_buffer = NULL;

	if (SHOW_FILE_CMD)
	{
		char cmdbuffer[256];
		MY_DEBUG("\nShow %s:\n",src_path);
		sprintf(cmdbuffer,"%s %s",SHOW_FILE_CMD,src_path);
		system(cmdbuffer);
		//MY_DEBUG("^_^\n");
	}
	
	return 0;
}


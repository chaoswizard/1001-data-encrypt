#include "Encrypt.h"

#define ENCRPT_DEBUG  printf

#define TMP_FILE_NAME "JM_RTX.txt"

struct st_byte_stream *encrypt_open(const char *path, char **real_path)
{
	struct st_byte_stream    *strm = NULL;
	int len = 0;
	
	//尝试以文件方式初始化
	if (!open_bytestrm(&strm, path, 0, STRM_FILE_R)) 
	{
        len = strlen(path);
        //尝试以写方式打开
        if(!open_bytestrm(&strm, TMP_FILE_NAME, len, STRM_FILE_W))
        {
            return NULL;
        }
        else
        {
            FILE_SEEK_SET(strm,0);
            if(len != FILE_WRITE(strm, path, len))
            {
                encrypt_close(&strm);
                return NULL;
            }
            else
            {
                FILE_SEEK_SET(strm,0);
            }
        }
	} 

	if (strm->m_Ew)// create
	{
		*real_path = TMP_FILE_NAME;
	}
	else
	{
		*real_path = path;
	}


    return strm;
}




int encrypt_close(struct st_byte_stream **src_stream)
{
    if (src_stream && *src_stream)
        FILE_CLOSE(*src_stream);

    return 0;
}



int encrypt_stream(struct st_byte_stream *dest,
                        struct st_byte_stream *src, long int len,
                        const char *key_buffer, int key_len)
{
	char *src_path = NULL, *encrypt_key_str = NULL;
	unsigned char *data_buffer; 
	unsigned int tmp = 0, buffer_len = 0, total_len;
	long int src_pos = 0, dest_pos = 0;

    total_len = 0;
	data_buffer = MALLOC(key_len+1);
    if (NULL == data_buffer)
	{
		ENCRPT_DEBUG("Malloc Er @%d\n",__LINE__);
		return -1;	
	}

    src_pos  = FILE_TELL(src);
    dest_pos = FILE_TELL(dest);
    while(src_pos < len) {
        FILE_SEEK_SET(src, src_pos);
        tmp = FILE_READ(src, data_buffer, key_len);
        //ENCRPT_DEBUG("R.(%d %d),[%d %d],%d  ",src_pos, FILE_TELL(src),key_len,tmp,len);
        if (0 == tmp)
        {
            buffer_len = 0;
            break;
        }
        else 
        {
            buffer_len = tmp;
        }
        //------------- Encrypt -------------------------------------------
        tmp=0;
        for (tmp=0; tmp<buffer_len; tmp++)
        {
            data_buffer[tmp] ^= key_buffer[tmp];
        } 
        //---------------------------------------------------------------
        FILE_SEEK_SET(dest, dest_pos);
        tmp = FILE_WRITE(dest, data_buffer, buffer_len);
        total_len += tmp;
        //ENCRPT_DEBUG("W.(%d %d),[%d %d],%d\n",src_pos, FILE_TELL(src),buffer_len,tmp,len);
        if (buffer_len != tmp)
        {
            break;
        }
        src_pos  += tmp;
        dest_pos += tmp;
    }

	FREE(data_buffer);

    return total_len;
}



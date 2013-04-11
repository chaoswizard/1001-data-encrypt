#ifndef __MYENCRYPT_H__
#define  __MYENCRYPT_H__
#include "macro_pub_define.h"


struct st_byte_stream *encrypt_open(const char *path, char **real_path);
int encrypt_close(struct st_byte_stream **src_stream);
int encrypt_stream(struct st_byte_stream *dest,
                        struct st_byte_stream *src, long int len,
                        const char *key_data, int key_len);

#endif


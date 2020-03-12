#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include <stdio.h>
#include <errno.h>

extern int encrypt_data(const unsigned char *key, unsigned char *iv, const unsigned char *plain, unsigned char **base64);
extern int decrypt_data(const unsigned char *key, unsigned char *iv, const unsigned char *b64, unsigned char **plain);

#endif /* __ENCRYPT_H__ */

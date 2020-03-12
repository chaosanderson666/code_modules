#include <stdio.h>
#include <stdlib.h>

#include "encrypt.h"

/* iv buffer must be readable and writable. */
static unsigned char en_key[KEY_LEN] = {0};
static unsigned char en_iv[KEY_LEN] = {0};
static unsigned char de_key[KEY_LEN] = {0};
static unsigned char de_iv[KEY_LEN] = {0};
	
int main(void)
{
	int ret;
	
	char *plain = "this is a test for aes encrypt...";
	unsigned char *base64 = NULL;
	unsigned char *decrypt = NULL;

	printf("plain: %s\n", plain);
	
	/* !Note: iv will be changed after encrypt. */
	ret = encrypt_data(en_key, en_iv, (unsigned char *)plain, &base64);
	if (ret != 0) {
		printf("encrypt failed.\n");
		return -1;
	} else {
		printf("encrypt: %s\n", base64);
	}
	
	ret = decrypt_data(de_key, de_iv, base64, &decrypt);
	if (ret != 0) {
		printf("decrypt failed.\n");
		free(base64);
		return -1;
	} else {
		printf("decrypt: %s\n", decrypt);
		free(base64);
		free(decrypt);
	}
}

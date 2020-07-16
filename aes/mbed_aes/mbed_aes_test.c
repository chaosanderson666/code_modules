#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "mbed_aes.h"

void print_buffer(const uint8_t *buff, int len)
{
	int i;
	
	printf("buff: ");

	for (i = 0; i < len; i++) {
		printf("%02x", buff[i]);
	}
	
	printf("\n");
}

int main(void)
{
	size_t cipher_len;
	
	uint8_t cipher[64] = {0};
	uint8_t de_plain[64] = {0};
	
	uint8_t en_iv[16] = {0x11,};
	uint8_t de_iv[16] = {0x11,};

	const uint8_t plain_hex[16] = {
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
	};

	const uint8_t key[16] = {
			0x11, 0x22, 0x33, 0x44,
			0x55, 0x66, 0x77, 0x88,
			0x11, 0x22, 0x33, 0x44,
			0x55, 0x66, 0x77, 0x88,
	};

	mbedtls_aes_context aes_ctx;
	
	print_buffer(plain_hex, sizeof(plain_hex));
	
	/* cipher_len should be a multiple of the block size (16 bytes) */
	if (sizeof(plain_hex) % 16 == 0) {
		cipher_len = sizeof(plain_hex);
	} else {
		cipher_len = (sizeof(plain_hex) / 16 + 1) * 16;
	}

	/* encryption */
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, key, 128);
	memset(cipher, 0, sizeof(cipher));
	mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, cipher_len, en_iv, plain_hex, cipher);
	print_buffer(cipher, sizeof(cipher));
	
	/* decryption */
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_dec(&aes_ctx, key, 128);
	memset(de_plain, 0, sizeof(de_plain));
	mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, cipher_len, de_iv, cipher, de_plain);
	print_buffer(de_plain,  sizeof(de_plain));
}


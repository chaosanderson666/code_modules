/*
 * https://tls.mbed.org/api/aes_8h.html#a321834eafbf0dacb36dac343bfd6b35d
 * https://tls.mbed.org/kb
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <mbedtls/aes.h>
#include <mbedtls/base64.h>

#include "encrypt.h"

#define KEY_LEN   16

int encrypt_data(const unsigned char *key, unsigned char *iv, const unsigned char *plain, unsigned char **base64)
{
	int ret;
	mbedtls_aes_context aes_ctx;

	size_t plain_len = strlen((char *)plain) + 1;

	/* cipher_len must be a multiple of the block size (key bytes) */
	size_t cipher_len = (plain_len/KEY_LEN + 1) * KEY_LEN;

    unsigned char *cipher_buff = (unsigned char *)calloc(cipher_len, sizeof(unsigned char));
	if (cipher_buff == NULL) {
	    return -1;
	}

	mbedtls_aes_init(&aes_ctx);
	ret = mbedtls_aes_setkey_enc(&aes_ctx, key, KEY_LEN * 8);
	if (ret != 0) {
		free(cipher_buff);
        mbedtls_aes_free(&aes_ctx);
		return -1;
	}

	ret = mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, cipher_len, iv, plain, cipher_buff);
	if (ret != 0) {
        free(cipher_buff);
        mbedtls_aes_free(&aes_ctx);
		return -1;
	}

	size_t base64_len = cipher_len * 2;

    unsigned char *base64_buff = (unsigned char *)calloc(base64_len, sizeof(unsigned char));
	if (base64_buff == NULL) {
        free(cipher_buff);
        mbedtls_aes_free(&aes_ctx);
	    return -1;
	}

    size_t write_len = 0;
    if (mbedtls_base64_encode(base64_buff, base64_len, &write_len, cipher_buff, cipher_len) != 0) {
        free(cipher_buff);
        free(base64_buff);
        mbedtls_aes_free(&aes_ctx);
        return -1;
    }

	*base64 = base64_buff;

    free(cipher_buff);
    mbedtls_aes_free(&aes_ctx);

	return 0;
}

int decrypt_data(const unsigned char *key, unsigned char *iv, const unsigned char *base64, unsigned char **plain)
{
	int ret;
	mbedtls_aes_context aes_ctx;

	size_t b64_len = strlen((char *)base64);
    size_t cipher_buf_len = b64_len;
    unsigned char *cipher = (unsigned char *)calloc(cipher_buf_len, sizeof(unsigned char));
	if (cipher == NULL) {
	    return -1;
	}

    size_t write_len = 0;
    if (mbedtls_base64_decode(cipher, cipher_buf_len, &write_len, base64, b64_len) != 0) {
        free(cipher);
		return -1;
    }

	mbedtls_aes_init(&aes_ctx);
	ret = mbedtls_aes_setkey_dec(&aes_ctx, key, KEY_LEN * 8);
	if (ret != 0) {
		free(cipher);
		mbedtls_aes_free(&aes_ctx);
        return -1;
	}

    size_t plain_buf_len = b64_len;
    unsigned char *plain_buf = (unsigned char *)calloc(plain_buf_len, sizeof(unsigned char));
	if (plain_buf == NULL) {
		free(cipher);
		mbedtls_aes_free(&aes_ctx);
		return -1;
	}

	ret = mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, write_len, iv, cipher, plain_buf);
	if (ret != 0) {
		free(cipher);
		mbedtls_aes_free(&aes_ctx);
		return -1;
	}

	*plain = plain_buf;

    free(cipher);
    mbedtls_aes_free(&aes_ctx);

	return 0;
}


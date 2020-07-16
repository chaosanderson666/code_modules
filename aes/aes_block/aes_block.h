#ifndef __AES_BLOCK_H__
#define __AES_BLOCK_H__

#include <stdint.h>

#define BLOCK_SIZE  16

typedef struct {
        uint32_t nr;    /* rounds */
        uint32_t *rk;   /* round_key */
        uint32_t buf[(BLOCK_SIZE + 1) << 2]; /* store round_keys, each block is 4 bytes */
} aes_context;

uint8_t aes_set_key(aes_context *ctx, const uint8_t *key, uint32_t key_bit);

uint8_t aes_encrypt_block(aes_context *ctx, uint8_t *cipher_text, const uint8_t *text);
uint8_t aes_decrypt_block(aes_context *ctx, uint8_t *text, const uint8_t *cipher_text);

#endif

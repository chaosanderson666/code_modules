#ifndef __AES_BLOCK_H__
#define __AES_BLOCK_H__

#include <stdint.h>

#define BLOCK_SIZE  16

typedef struct {
        uint32_t nr;    /* rounds */
        uint32_t *rk;   /* round_key */
        uint32_t buf[(BLOCK_SIZE + 1) << 2]; /* store round_keys, each block is 4 bytes */
} aes_context;

void aes_ctx_init(aes_context *ctx);
int aes_set_key(aes_context *ctx, const uint8_t *key, uint32_t key_bit);

int aes_block_encrypt(aes_context *ctx, const uint8_t *input, uint8_t *output);
int aes_block_decrypt(aes_context *ctx, const uint8_t *input, uint8_t *output);

int aes_cbc_encrypt(aes_context *ctx, uint32_t length, uint8_t iv[16],
                    const uint8_t *input, uint8_t *output);
int aes_cbc_decrypt(aes_context *ctx, uint32_t length, uint8_t iv[16],
                    const uint8_t *input, uint8_t *output);

#endif

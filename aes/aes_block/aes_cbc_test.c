#include <stdio.h>
#include <stdint.h>

#include "aes_block.h"

void print_hex(const uint8_t *buff, int len)
{
        int i;

        printf("buff: ");

        for (i = 0; i < len; i++) {
                printf("%02x", buff[i]);
        }

        printf("\r\n");
}

int main(void)
{
        uint32_t cipher_len;

        uint8_t cipher[64] = {0};
        uint8_t de_plain[64] = {0};

        uint8_t en_iv[16] = {0x11, };
        uint8_t de_iv[16] = {0x11, };

        const uint8_t plain_hex[] = {
                0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        };

        const uint8_t key[16] = {
                0x11, 0x22, 0x33, 0x44,
                0x55, 0x66, 0x77, 0x88,
                0x11, 0x22, 0x33, 0x44,
                0x55, 0x66, 0x77, 0x88,
        };

        aes_context ctx;

        print_hex(plain_hex, sizeof(plain_hex));

        /* cipher_len should be a multiple of the block size (16 bytes) */
        if (sizeof(plain_hex) % 16 == 0) {
                cipher_len = sizeof(plain_hex);
        } else {
                cipher_len = (sizeof(plain_hex) / 16 + 1) * 16;
        }

        /* encryption */
        aes_ctx_init(&ctx);
        aes_set_key(&ctx, key, 128);
        aes_cbc_encrypt(&ctx, cipher_len, en_iv, plain_hex, cipher);
        print_hex(cipher, sizeof(cipher));

        /* decryption */
        aes_ctx_init(&ctx);
        aes_set_key(&ctx, key, 128);
        aes_cbc_decrypt(&ctx, cipher_len, de_iv, cipher, de_plain);
        print_hex(de_plain, sizeof(de_plain));
}

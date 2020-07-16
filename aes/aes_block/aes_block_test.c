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
        aes_context ctx;

        uint8_t de_hex[32] = {0};
        uint8_t cipher[32] = {0};

        const uint8_t plain_hex[] = {
                0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x77,
        };

        const uint8_t key[16] = {
                0x11, 0x22, 0x33, 0x44,
                0x55, 0x66, 0x77, 0x88,
                0x11, 0x22, 0x33, 0x44,
                0x55, 0x66, 0x77, 0x88,
        };

        aes_set_key(&ctx, key, 128);

        print_hex(plain_hex, sizeof(plain_hex));

        /* encryption */
        aes_encrypt_block(&ctx, cipher, plain_hex);
        print_hex(cipher, sizeof(cipher));

        /* decryption */
        aes_decrypt_block(&ctx, de_hex, cipher);
        print_hex(de_hex, sizeof(de_hex));
}

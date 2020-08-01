#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(unsigned char *a, unsigned char *b) {
        unsigned char t = *a;
        *a = *b;
        *b = t;
}

void rc4KSA(unsigned char *s, const unsigned char *key, int len) {
        int i;

        for (i = 0; i < 256; i++) s[i] = i;
        int j = 0;

        for (i = 0; i < 256; i++ ) {
                j = (j + s[i] + *(key + i % len)) % 256;
                swap(s + i, s + j);
        }
}

void rc4PRGA(unsigned char *s, char *data, size_t len) {
        int i = 0;
        int j = 0;
        size_t idx;

        for ( idx = 0; idx < len; idx++ ) {
                i = (i + 1) % 256;
                j = (j + s[i]) % 256;
                swap(s + i, s + j);
                unsigned char k = s[(s[i] + s[j]) % 256];
                data[idx] ^= k;  // k是伪随机数，和明文进行XOR进行加密
        }
}

int main(int argc, const char * argv[]) {
        unsigned char s[256]; // S-box
        unsigned char key[128]; // key的长度1到256字节，本例子使用128字节
        //arc4random_buf((unsigned char *)key, sizeof(key)); // 用伪随机数算法生成key
        char data[1000] = "这里是要机密的数据";

        // 加密
        rc4KSA(s, key, sizeof(key));
        rc4PRGA(s, data, strlen(data));

        // 解密
        rc4KSA(s, key, sizeof(key));
        rc4PRGA(s, data, strlen(data));
        return 0;
}


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int str2hex(char *out, const char *in)
{
    int i, j;
    int len = (int)strlen(in);
    char high, low;
    
    if (len % 2 != 0) {
        return -1;
    }
    
    for (i = 0, j = 0; i < len; i += 2, j++) {
        if (in[i] >= '0' && in[i] <= '9') {
            high = in[i] - '0';
        } else if (in[i] >= 'a' && in[i] <= 'f') {
            high = in[i] - 'a' + 10; 
        } else if (in[i] >= 'A' && in[i] <= 'F') {
            high = in[i] - 'A' + 10; 
        } else {
            return -1;
        }
        
        if (in[i + 1] >= '0' && in[i + 1] <= '9') {
            low = in[i + 1] - '0';
        } else if (in[i + 1] >= 'a' && in[i + 1] <= 'f') {
            low = in[i + 1] - 'a' + 10; 
        } else if (in[i + 1] >= 'A' && in[i + 1] <= 'F') {
            low = in[i + 1] - 'A' + 10;
        } else {
            return -1; 
        }

        out[j] = ((high & 0x0f) << 4 | (low & 0x0f));
    }
    return 0;
}

int main(void)
{
    char str[] = "0123456789abcdefABCDEF0102030a0f";
    unsigned char hex[sizeof(str)] = {0};
    int i;

    printf("%s\n", str);
    int ret = str2hex(hex, str);
    for (i = 0; i < sizeof(hex)/2; i++) {
        printf("%02x", hex[i]);
    }
    printf("\n");
}

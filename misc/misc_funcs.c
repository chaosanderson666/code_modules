//string to upper
#include <ctype.h>
char *strupr(char *str){
    char *orign = str;
    for (; *str != '\0'; str++)
        *str = toupper(*str);
    return orign;
}

//string to lower
char *strlwr(char *str){
    char *orign = str;
    for (; *str != '\0'; str++)
        *str = tolower(*str);
    return orign;
}

//注意strncpy会在dest后面所有剩余的空间赋'\0'。
//一般用法：strncpy(buff, src, sizeof(buff)-1);
//如果不减一，那么dest最后一个空位可能就没有设置成'\0'，那就不是字符串了。
char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';
    return dest;
}

static void __memcpy(void *des, const void *src, uint32_t len)
{
        if (!des || !src || len <= 0) {
                return;
        }

        uint8_t *tmp_des = (uint8_t *)des;
        uint8_t *tmp_src = (uint8_t *)src;

        while (len-- > 0) {
                *tmp_des++ = *tmp_src++;
        }
}

static void __memset(void *s, int c, uint32_t len)
{
        if (!s || len <= 0) {
                return;
        }

        uint8_t *tmp = (uint8_t *)s;

        while (len-- > 0) {
                *tmp++ = c;
        }
}

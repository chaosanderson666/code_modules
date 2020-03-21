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

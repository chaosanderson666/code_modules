#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: %s <str1> <str2>\n", argv[0]);
        return 0;
    }

    if (strstr(argv[1], argv[2]) != NULL) {
        printf("'%s' exist in '%s'\n", argv[2], argv[1]);
    } else {
        printf("'%s' is not exist in '%s'\n", argv[2], argv[1]);
    }

    return 0;
}

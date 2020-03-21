#include <stdio.h>
#include <stdlib.h>

/* do not have pointer in the struct, sub struct */
typedef struct sample_t {
    int magic;
    char magic_str[40];
} sample_t;

int main(void)
{
    sample_t *sample1 = (sample_t *)calloc(sizeof(sample_t), 1);
    sample1->magic = 0x5a5a;
    sprintf(sample1->magic_str, "%s", "magic string...");
    
    printf("Writing magic to a file...\n");
    FILE *fp;
    /* if the dir does not exist, creating will be failed. */
    //fp = fopen("./dir/magic", "wb+");
    fp = fopen("./magic", "wb+");
    if (fp != NULL) {
        fwrite((char *)sample1 , sizeof(sample_t), 1, fp);
        fclose(fp);
    }
    
    sample_t *sample2 = (sample_t *)calloc(sizeof(sample_t), 1);
    printf("Reading magic from a file...\n");

    fp = fopen("./magic", "rb");
    if (fp != NULL) {
        fread((char *)sample2 , sizeof(sample_t), 1, fp);
        fclose(fp);
    }
    printf("magic: 0x%x, magic_str: %s\n", sample2->magic, sample2->magic_str);
    
    return 0;
}
/*
http://man7.org/linux/man-pages/man3/fopen.3.html
http://man7.org/linux/man-pages/man3/fread.3.html
*/

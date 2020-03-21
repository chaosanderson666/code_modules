#include <stdio.h>
#include <time.h>
#include<stdlib.h>

int main(void)
{
    unsigned long  num;
    unsigned long  iter;
    time_t timeb, timea;

    iter = 0;
    time(&timeb);
    srand((unsigned long)time(0));

    while(1){
        num = rand()%10000000;

        ++iter;

        fprintf(stderr, "%ld ", num);
        if(num == 3141592){
            break;
        }
    }

    fprintf(stderr, "\nHit num: %ld\nIter:%ld\n", num, iter);
    time(&timea);
    printf("Start at: %s", ctime(&timeb));
    printf("Hit at: %s", ctime(&timea));

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

/*
    When calculating the size of a structure, its memory layout needs to be considered.
    Simply, each variable should be aligned with its own size, and the last position
    variable should be aligned with the cell size. The size of cell depends on the size
    of the largest type in the structure.
*/

/* 
    For example, The largest type in the following structure is int, so the cell size should be 
    4(32-bit machine).
    The type whose size that less than 4 can be added together, if their total size exceed the 
    cell size, a new cell will be allocated. If the last type size is less than cell size, its 
    size will also be considered to be cell size. 

    +---+---+---+---+
    | a | a | b |   |
    +---+---+---+---+
    | c | c | c | c |
    +---+---+---+---+
    | d |   |   |   |
    +---+---+---+---+

    largest cell size: 4, the total size: 12.
*/
typedef struct {
    short a;
    char b;
    int c;
    char d;
}st1;

/*
    Each variable will also be aligned with its size. For example, the char variable 
    will be aligned with length 1, short will be aligned with length 2, etc. If a char 
    is allocated first and then a short is allocated, the short will not be next to 
    the char, it will skip a byte to align with length 2, so it will be placed from 
    the third position.
    Let's exchange the position of a and b to understand this.

    +---+---+---+---+
    | b |   | a | a |
    +---+---+---+---+
    | c | c | c | c |
    +---+---+---+---+
    | d |   |   |   |
    +---+---+---+---+

    largest cell size: 4, the total size: 12.
*/
typedef struct {
    char b;
    short a;
    int c;
    char d;
}st1_align;

/*
    Sometimes we can change the location of these variables to save storage space.
    For comparison, let's change the position of the these variables again.

    +---+---+---+---+
    | b | d | a | a |
    +---+---+---+---+
    | c | c | c | c |
    +---+---+---+---+

    Now it takes up less space, just 8 bytes, the minimum space.
    largest cell size: 4, the total size: 8.
*/
typedef struct {
    char b;
    char d;
    short a;
    int c;
}st1_best;

/*
    Just another example, the largest type in the following structure is 
    short, so the cell size should be 2.

    +---+---+
    | a |   |
    +---+---+
    | b | b |
    +---+---+
    | c |   |
    +---+---+

    Largest cell size: 2, the total size: 6.
*/
typedef struct {
    char a;
    short b;
    char c;
}st2;

/*
    The largest type in the following structure is short(short is in 
    the st2), so the cell size should be 2. And we should consider struct 
    variable e as a whole, so the a in the st2 should be in a new cell.

    +---+---+
    | d |   |
    +---+---+
    | a |   |   ---
    +---+---+      \
    | b | b |   ----- st2
    +---+---+      /
    | c |   |   ---
    +---+---+
    | f |   |
    +---+---+

    Largest cell size: 2, the total size: 10.
*/
typedef struct {
    char d;
    st2 e;
    char f;
}st3;

/*
    Now let's test these theories.
*/
int main(void)
{
    int i;

    printf("sizeof(char): %d\n", sizeof(char));
    printf("sizeof(short): %d\n", sizeof(short));
    printf("sizeof(int): %d\n", sizeof(int));
    printf("sizeof(long): %d\n", sizeof(long));
    printf("sizeof(long long): %d\n", sizeof(long long));
    printf("sizeof(double: %d\n", sizeof(double));
    printf("sizeof(struct {short char int char}): %d\n", sizeof(st1));
    printf("sizeof(struct {char short int char}): %d\n", sizeof(st1_align));
    printf("sizeof(struct {char char short int}): %d\n", sizeof(st1_best));
    printf("sizeof(struct {char short char}): %d\n", sizeof(st2));
    printf("sizeof(struct {char st2 char}): %d\n", sizeof(st3));

    /* show st1 layout */
    st1 *s1 = (st1 *)calloc(1, sizeof(st1));
    s1->a = 0xa1a2;
    s1->b = 0xb1;
    s1->c = 0xc1c2c3c4;
    s1->d = 0xd1;

    char *p = (char *)s1;
    printf("\nst1 layout, size: %d\n", sizeof(st1));
    for (i = 0; i < sizeof(st1); i++) {
        printf(" 0x%02x",  (unsigned char)p[i]);
    }

    /* show st1_align layout */
    st1_align *s1_align = (st1_align *)calloc(1, sizeof(st1_align));
    s1_align->a = 0xa1a2;
    s1_align->b = 0xb1;
    s1_align->c = 0xc1c2c3c4;
    s1_align->d = 0xd1;

    char *p_align = (char *)s1_align;
    printf("\nst1_align layout, size: %d \n", sizeof(st1_align));
    for (i = 0; i < sizeof(st1_align); i++) {
        printf(" 0x%02x", (unsigned char)p_align[i]);
    }

    /* show st1_best layout */
    st1_best *s1_best = (st1_best *)calloc(1, sizeof(st1_best));
    s1_best->a = 0xa1a2;
    s1_best->b = 0xb1;
    s1_best->c = 0xc1c2c3c4;
    s1_best->d = 0xd1;

    char *p_best = (char *)s1_best;
    printf("\nst1_best layout, size: %d \n", sizeof(st1_best));
    for (i = 0; i < sizeof(st1_best); i++) {
        printf(" 0x%02x", (unsigned char)p_best[i]);
    }
    printf("\n\n");
    
    free(s1);
    free(s1_align);
    free(s1_best);
    
    return 0;
}

/* 
$ ./struct_size 
sizeof(char): 1
sizeof(short): 2
sizeof(int): 4
sizeof(long): 4
sizeof(long long): 8
sizeof(double: 8
sizeof(struct {short char int char}): 12
sizeof(struct {char short int char}): 12
sizeof(struct {char char short int}): 8
sizeof(struct {char short char}): 6
sizeof(struct {char st2 char}): 10

st1 layout, size: 12
 0xa2 0xa1 0xb1 0x00 0xc4 0xc3 0xc2 0xc1 0xd1 0x00 0x00 0x00
st1_align layout, size: 12 
 0xb1 0x00 0xa2 0xa1 0xc4 0xc3 0xc2 0xc1 0xd1 0x00 0x00 0x00
st1_best layout, size: 8 
 0xb1 0xd1 0xa2 0xa1 0xc4 0xc3 0xc2 0xc1

*/

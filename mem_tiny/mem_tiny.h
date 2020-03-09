#ifndef __MEM_TINY_H__
#define __MEM_TINY_H__

void *malloc_tiny(unsigned int size);
int free_tiny(void *ptr);
void memset_tiny(void *m, unsigned char value, unsigned int size);

#endif  /* __MEM_TINY_H__ */


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "mem_tiny.h"

#define MEM_TINY_DEBUG_ENABLE      1

#if MEM_TINY_DEBUG_ENABLE
#define MEM_TINY_DEBUG(format, ...) \
do { \
    printf(""format"", ##__VA_ARGS__); \
    fflush(stdout); \
} while(0)
#else
#define MEM_TINY_DEBUG(format, ...)
#endif /* MEM_TINY_DEBUG_ENABLE */

/* the number of array do not larger than 'sizeof(flg) * 8 = 32' */
static uint8_t mb1[20][64] = {0};
static uint8_t mb2[10][128] = {0};
static uint8_t mb3[5][256] = {0};
static uint8_t mb4[5][256] = {0};

#define NUM_OF_ARRAY(array)  (sizeof(array)/sizeof(array[0]))

struct mem_block {
        uint8_t is_full;
        uint32_t bitmap;
        uint32_t num;
        uint32_t size;
        uint8_t **block;
};

#define NEW_MEM_BLOCK(mb)             \
	{                             \
	  .is_full = 0,               \
	  .bitmap = 0,                \
	  .num = (NUM_OF_ARRAY(mb)),  \
	  .size = sizeof((mb)[0]),    \
	  .block = (uint8_t **)(mb),  \
	}

static struct mem_block mbs[] = {
        NEW_MEM_BLOCK(mb1),
        NEW_MEM_BLOCK(mb2),
        NEW_MEM_BLOCK(mb3),
        NEW_MEM_BLOCK(mb4),
};

static void *__do_alloc(struct mem_block *mb)
{
        uint32_t i;

        for (i = 0; i < mb->num; i++) {
                if ((mb->bitmap & (1 << i)) == 0) {
                        mb->bitmap |= (1 << i);
                        MEM_TINY_DEBUG("alloc size: %d, num: %d, bitmap: 0x%08X, addr: 0x%08X\r\n",
                                       mb->size, i, mb->bitmap, (uint32_t)((uint8_t *)mb->block + i * mb->size));
                        return (void *)((uint8_t *)mb->block + i * mb->size);
                }
        }
        mb->is_full = 1;
        MEM_TINY_DEBUG("alloc size: %d is full, bitmap: 0x%08X\r\n", mb->size,
                       mb->bitmap);
        return NULL;
}

static void __do_free(uint8_t *ptr, struct mem_block *mb)
{
        uint32_t i;
        uint32_t offset = 0;

        for (i = 0; i < mb->num; i++) {
                offset += mb->size;
                if (ptr < (uint8_t *)mb->block + offset) {
                        mb->bitmap &= ~(1 << i);
                        mb->is_full = 0;
                        MEM_TINY_DEBUG("free size: %d, num: %d, bitmap: 0x%08X, addr: 0x%08X\r\n",
                                       mb->size, i, mb->bitmap, (uint32_t)((uint8_t *)mb->block + i * mb->size));
                        return;
                }
        }
}

void memset_tiny(void *mb, uint8_t value, uint32_t size)
{
        int i;

        for (i = 0; i < size; i++) {
                ((uint8_t *)mb)[i] = value;
        }
}

void *malloc_tiny(uint32_t size)
{
        uint32_t i = 0;
        struct mem_block *mb = NULL;
        void *alloc_mem = NULL;

        if (!size) {
                return NULL;
        }

        for (i = 0; i < NUM_OF_ARRAY(mbs); i++) {
                mb = &mbs[i];
                if (size <= mb->size && !mb->is_full) {
                        alloc_mem = __do_alloc(mb);
                        if (alloc_mem) {
                                return alloc_mem;
                        }
                }
        }

        return NULL;
}

int free_tiny(void *ptr)
{
        uint32_t i = 0;
        struct mem_block *mb = NULL;

        if (!ptr) {
                return -1;
        }

        for (i = 0; i < NUM_OF_ARRAY(mbs); i++) {
                mb = &mbs[i];
                if (((uint8_t *)ptr >= (uint8_t *)mb->block)
                                && ((uint8_t *)ptr < (uint8_t *)mb->block + mb->num * mb->size)) {
                        __do_free((uint8_t *)ptr, mb);
                        return 0;
                }
        }

        return -1;
}


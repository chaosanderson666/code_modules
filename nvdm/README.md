### 主要特点
- 提供 group-key-value 的存储形式
- 适用范围？是否提供写平衡机制？
- nvdm_test.c 提供了一些如何使用的例子
### 如何移植？
- 如果是在 OS 环境下使用，则需要在 nvdm_port.c 文件中定义如下的变量和相关的接口，这里假设是 FreeRTOS。这些变量和接口提供了互斥访问和内存管理的功能，如果没有 OS，这些接口一般都是空函数，只有内存接口需要定义，具体如何定义这些接口可以参考 nvdm_port.c 文件中的例子。
```c
/* This macro defines NVDM using in OS context */
#define NVDM_PORT_USE_OS

#ifdef NVDM_PORT_USE_OS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdbool.h>

typedef SemaphoreHandle_t nvdm_port_mutex_t;
static nvdm_port_mutex_t g_nvdm_mutex;
static TaskHandle_t g_task_handler;
```
- Flash 配置，也在 nvdm_port.c 文件里。具体参考下面的代码块，其中最重要的是 FLASH_CONFIG_ADDRESS 和 ROM_NVDM_LENGTH，它们分别指定了 Flash 读写的起始地址和分配的大小，其他的一些宏请具体看注释，有些宏可能对存储的效率有一些影响。
```c
/* This macro defines max count of data items */
#define NVDM_PORT_DAT_ITEM_COUNT (500)
/* This macro defines size of PEB, normally it is size of flash block */
#define NVDM_PORT_PEB_SIZE (4096)
/* This macro defines max size of data item during all user defined data items.
 * 1. Must not define it greater than 2048 bytes.
 * 2. Define it as smaller as possible to enhance the utilization rate of NVDM region.
 * 2. Try your best to store small data less than 256 bytes.
 */
#define NVDM_PORT_MAX_DATA_ITEM_SIZE (2048)
/* This macro defines start address and PEB count of the NVDM region */
#define FLASH_CONFIG_ADDRESS   0x1F0000
#define ROM_NVDM_LENGTH  0x00010000   /* 64kB */
#define NVDM_PORT_REGION_ADDRESS(pnum, offset) (FLASH_CONFIG_ADDRESS + pnum * NVDM_PORT_PEB_SIZE + offset)
#define NVDM_PORT_REGION_PEB_COUNT    (ROM_NVDM_LENGTH / NVDM_PORT_PEB_SIZE)
/* This macro defines max length of group name of data item */
#define GROUP_NAME_MAX_LENGTH (16)
/* This macro defines max length of data item name of data item */
#define DATA_ITEM_NAME_MAX_LENGTH (32)
```
- Flash 读写接口配置，需要提供三个操作函数，读取，写入以及擦除。读取和写入都是基于字节流的，擦除是基于 sector 的，大小由 NVDM_PORT_PEB_SIZE 指定（PEB 是 physical eraseblock 的缩写），是该 Flash 的最小擦除单位，下面的代码块是一个例子。
```c
void nvdm_port_flash_read(uint32_t address, uint8_t *buffer, uint32_t length)
{
        int status = flash_stream_read(&flash, address, length, buffer);
        if (status != 1) {
                /* error print */
        }
}

void nvdm_port_flash_write(uint32_t address, const uint8_t *buffer, uint32_t length)
{
        int status = flash_stream_write(&flash, address, length, buffer);
        if (status != 1) {
                /* error print */
        }
}

/* erase unit is 4K large(which is size of PEB) */
void nvdm_port_flash_erase(uint32_t address)
{
        flash_erase_sector(&flash, address);
}
```
- 日志打印设置，需要将如下宏定义为真，并且设置好打印缓冲的大小，注意这个缓冲的大小要大于要打印的最长字符串的大小，最后设置好三个打印函数，在源码中有如何设置的例子，设置好后，在运行时会打印出很多关于存储的信息，比如执行过程，存储空间的情况等。
```c
#define DEBUG_NVMD_ENABLE   1
static char sprint_buf[128];
void nvdm_port_log_notice(const char *message, ...);
void nvdm_port_log_info(const char *message, ...);
void nvdm_port_log_error(const char *message, ...);
```

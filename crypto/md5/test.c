#include <stdio.h>
#include <string.h>
#include "md5_hash.h"
	
int main(uint32_t app_addr, uint32_t app_size, uint8_t *origin_hash)
{
        unsigned char temp[4];
        unsigned char app_hash[MD5_SIZE];
        md5_context ctx;

        /* The app size must be word-aligned */
        if (app_size == 0 || app_size % sizeof(uint32_t) != 0) {
                return -1;
        }

        memset(app_hash, 0, sizeof(app_hash));
	md5_starts(&ctx);
        for (int i = 0; i < app_size; i += sizeof(temp)) {
                sysflash_read(app_addr + i, (uint32_t *)temp, sizeof(temp));
		md5_update(&ctx, temp, sizeof(temp));
        }
	md5_finish(&ctx, app_hash);

	if (memcmp(app_hash, origin_hash, MD5_SIZE) == 0) {
		return 0;
	}
	
	return -1;
}


#include "nvdm.h"
#include <stdint.h>
#include <stdlib.h>

#define STA_IPADDR_DEFAULT_VALUE "192.168.0.1"

void nvdm_application(void)
{
        nvdm_status_t status;
        uint32_t size;
        uint8_t buffer[16];
        char group_name[64];
        char data_item_name[64];

        status = nvdm_init();
        if (status != NVDM_STATUS_OK) {
                goto error;
        }

        status = nvdm_write_data_item("STA", "IpAddr", NVDM_DATA_ITEM_TYPE_STRING,
                                      (uint8_t *)STA_IPADDR_DEFAULT_VALUE, sizeof(STA_IPADDR_DEFAULT_VALUE));
        if (status != NVDM_STATUS_OK) {
                goto error;
        }

        size = sizeof(buffer);
        memset(buffer, 0, sizeof(buffer));
        status = nvdm_read_data_item("STA", "IpAddr", buffer, &size);
        if (status != NVDM_STATUS_OK) {
                goto error;
        }
        printf("\n\r-------nvdm buffer: %s", buffer);

        status = nvdm_write_data_item("STA", "IpAddr2", NVDM_DATA_ITEM_TYPE_STRING,
                                      (uint8_t *)STA_IPADDR_DEFAULT_VALUE, sizeof(STA_IPADDR_DEFAULT_VALUE));
        if (status != NVDM_STATUS_OK) {
                goto error;
        }

        size = sizeof(buffer);
        memset(buffer, 0, sizeof(buffer));
        status = nvdm_read_data_item("STA", "IpAddr2", buffer, &size);
        if (status != NVDM_STATUS_OK) {
                goto error;
        }
        printf("\n\r-------nvdm buffer: %s", buffer);

        status = nvdm_query_begin();
        if (status != NVDM_STATUS_OK) {
                printf("-------nvdm error!!!!!");
                return;
        }
        status = nvdm_query_next_group_name(group_name);
        if (status != NVDM_STATUS_OK) {
                printf("-------nvdm error!!!!!");
                return;
        }
        status = nvdm_query_next_data_item_name(data_item_name);
        if (status != NVDM_STATUS_OK) {
                printf("-------nvdm error!!!!!");
                return;
        }
        status = nvdm_query_end();
        if (status != NVDM_STATUS_OK) {
                printf("-------nvdm error!!!!!");
                return;
        }

        status = nvdm_delete_data_item(group_name, data_item_name);
        if (status != NVDM_STATUS_OK) {
                printf("-------nvdm error!!!!!");
                return;
        }

error:
        printf("-------nvdm error!!!!!");
}

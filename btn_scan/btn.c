#include <string.h>
#include <stdio.h>
#include "bsp_btn.h"
#include "btn.h"

#define KEY_STATE_START       0
#define KEY_STATE_PRESS       1
#define KEY_STATE_LONG_PRESS  2

#define KEY_PRESS_TIME_LONG     80
#define KEY_DEBOUNCE_TIME       1

struct key_object;
typedef void (*handle_cb_t)(struct key_object *);
typedef int (*read_status_t)(void);

struct key_combi {
        const int key_id;
        const handle_cb_t combi_cb;  /* combination key callback func. */
};

struct key_object {
        const char *name;
        int state;
        int time_long;
        int time_debounce;
        const read_status_t state_read;
        const handle_cb_t click_handle;
        const handle_cb_t long_handle;
        const int combi_key_num;
        const struct key_combi combi_keys[5];
};

static void __up_click_handle(struct key_object *key)
{
        send_event_to_main_task(TASK_MAIN_EVT_TEMP_PLUS);
}

static void __up_long_handle(struct key_object *key)
{
        send_event_to_main_task(TASK_MAIN_EVT_MOTOR_DETECT);
}

static void __down_click_handle(struct key_object *key)
{
        send_event_to_main_task(TASK_MAIN_EVT_TEMP_MINUS);
}

static void __down_long_handle(struct key_object *key)
{
        send_event_to_main_task(TASK_MAIN_EVT_START_PAIR);
}

static void __mode_click_handle(struct key_object *key)
{
        send_event_to_main_task(TASK_MAIN_EVT_MODE_CHANGE);
}

static void __mode_long_handle(struct key_object *key)
{
        send_event_to_main_task(TASK_MAIN_EVT_DEVICE_TURN_OFF);
}

/* combination keys handle functions. */
static void __up_down_combi_handle(struct key_object *key)
{
        config_reset();
        HAL_NVIC_SystemReset();
}

static void __up_mode_combi_handle(struct key_object *key)
{
        send_event_to_main_task(TASK_MAIN_EVT_DEV_LOCK);
}

struct key_object keys[] = {
        [0] = {
                .name = "up",
                .state = KEY_STATE_START,
                .time_long = KEY_PRESS_TIME_LONG,
                .time_debounce = KEY_DEBOUNCE_TIME,
                .state_read = bsp_btn_up_read,
                .click_handle = __up_click_handle,
                .long_handle = __up_long_handle,
                .combi_key_num = 2,
                .combi_keys = {
                        [0] = { .key_id = 1, .combi_cb = __up_down_combi_handle, },
                        [1] = { .key_id = 2, .combi_cb = __up_mode_combi_handle, },
                }
        },
        [1] = {
                .name = "down",
                .state = KEY_STATE_START,
                .time_long = KEY_PRESS_TIME_LONG,
                .time_debounce = KEY_DEBOUNCE_TIME,
                .state_read = bsp_btn_down_read,
                .click_handle = __down_click_handle,
                .long_handle = __down_long_handle,
                .combi_key_num = 1,
                .combi_keys = {
                        [0] = { .key_id = 0, .combi_cb = __up_down_combi_handle, },
                }
        },
        [2] = {
                .name = "mode",
                .state = KEY_STATE_START,
                .time_long = KEY_PRESS_TIME_LONG,
                .time_debounce = KEY_DEBOUNCE_TIME,
                .state_read = bsp_btn_mode_read,
                .click_handle = __mode_click_handle,
                .long_handle = __mode_long_handle,
                .combi_key_num = 1,
                .combi_keys = {
                        [0] = { .key_id = 0, .combi_cb = __up_mode_combi_handle, },
                }
        },
};

static int __wait_all_key_release(struct key_object *key)
{
        while (bsp_btns_all_release()) {
                vTaskDelay(pdMS_TO_TICKS(10));
        }
}

static int __key_reset(struct key_object *key)
{
        key->state = KEY_STATE_START;
        key->time_long = KEY_PRESS_TIME_LONG;
        key->time_debounce = 0;

        return 0;
}

static int check_combi_keys(struct key_object *key)
{
        int i;
        int key_id;
        const struct key_combi *combi_keys = key->combi_keys;

        for (i = 0; i < key->combi_key_num; i++) {
                key_id = combi_keys[i].key_id;
                if (!keys[key_id].state_read()) {
                        combi_keys[i].combi_cb(key);
                }
        }
}

void btns_scan(void)
{
        static int index = 0;
        struct key_object *key = &keys[index];

        switch (key->state) {
        case KEY_STATE_START:
                if (!(key->state_read)()) {
                        key->state = KEY_STATE_PRESS;
                }
                break;
        case KEY_STATE_PRESS:
                if ((key->state_read)()) {
                        /* it is a bounce. */
                        __key_reset(key);
                        break;
                }

                if (key->time_debounce > 0) {
                        key->time_debounce--;
                } else {
                        /* The button was actually pressed, but maybe it is long press. */
                        key->state = KEY_STATE_LONG_PRESS;
                }
                break;
        case KEY_STATE_LONG_PRESS:
                /* if this key was released, and time_long is not become to 0, it is a single click. */
                if ((key->state_read)()) {
                        (key->click_handle)(keys);
                        __key_reset(key);
                        break;
                }

                if (key->time_long > 0) {
                        key->time_long--;
                        break;
                }

                /* here, this key's time_long became to 0. */

                /* if this key register combination keys, check them. */
                if (key->combi_key_num > 0) {
                        check_combi_keys(key);
                        __wait_all_key_release(keys);
                        __key_reset(key);
                        break;
                }

                (key->long_handle)(keys);
                __wait_all_key_release(keys);
                __key_reset(key);
                break;
        }

        if (index++ >= sizeof(keys) / sizeof(keys[0]) - 1) {
                /* start scan again! */
                index = 0;
        }
}

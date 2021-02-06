#include <string.h>
#include <stdio.h>
#include "bsp_btn.h"
#include "btn.h"
#include "user_tasks.h"

enum {
        KEY_TIME_LONG_PRESS = 80,
        KEY_TIME_DEBOUNCE = 1,
        KEY_TIME_BETWEEN_CLICKS = 5,
};

enum {
        KEY_STATE_START,
        KEY_STATE_DEBOUNCE,
        KEY_STATE_PRESSED,
        KEY_STATE_CLICKS,
        KEY_STATE_LONG_PRESS,
};

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
        int time_between_clicks;              /* the time between clicks.*/
        const int pressed_state;              /* the state of the button be pressed. */
        const read_status_t state_read;
        int click_cnt;
        const handle_cb_t clicks[2];
        const handle_cb_t long_handle;
        const int combi_key_num;
        const struct key_combi combi_keys[3]; /* the combination keys relate this key. */
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
                .time_long = 0,
                .time_debounce = 0,
                .time_between_click = 0,
                .pressed_state = 0,
                .state_read = bsp_btn_up_read,
                .click_cnt = 0,
                .clicks = {
                        [0] = __up_click_handle,
                },
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
                .time_long = 0,
                .time_debounce = 0,
                .time_between_click = 0,
                .pressed_state = 0,
                .state_read = bsp_btn_down_read,
                .click_cnt = 0,
                .clicks = {
                        [0] = __down_click_handle,
                },
                .long_handle = __down_long_handle,
                .combi_key_num = 1,
                .combi_keys = {
                        [0] = { .key_id = 0, .combi_cb = __up_down_combi_handle, },
                }
        },
        [2] = {
                .name = "mode",
                .state = KEY_STATE_START,
                .time_long = 0,
                .time_debounce = 0,
                .time_between_clicks = 0,
                .pressed_state = 0,
                .state_read = bsp_btn_mode_read,
                .click_cnt = 0,
                .clicks = {
                        [0] = __mode_click_handle,
                        [1] = __mode_click_handle,
                },
                .long_handle = __mode_long_handle,
                .combi_key_num = 1,
                .combi_keys = {
                        [0] = { .key_id = 0, .combi_cb = __up_mode_combi_handle, },
                }
        },
};

static int __wait_all_key_release(struct key_object *keys)
{
        int i;
        int pressed = 0;

        while (1) {
                for (i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
                        if (keys[i].state_read() == keys[i].pressed_state) {
                                pressed = 1;
                        }
                }
                if (!pressed) {
                        break;
                }
                pressed = 0;
                __btn_sleep_ms(10);
        }
}

static int __key_reset(struct key_object *key)
{
        key->state = KEY_STATE_START;
        key->time_long = 0;
        key->time_debounce = 0;
        key->click_cnt = 0;

        return 0;
}

static int check_combi_keys(struct key_object *key)
{
        int i;
        int key_id;
        const struct key_combi *combi_keys = key->combi_keys;

        for (i = 0; i < key->combi_key_num; i++) {
                key_id = combi_keys[i].key_id;
                if (keys[key_id].state_read() == keys[key_id].pressed_state) {
                        combi_keys[i].combi_cb(key);
                        return 1;
                }
        }

        return 0;
}

void btns_scan(void)
{
        static int index = 0;
        struct key_object *key = &keys[index];

        switch (key->state) {
        case KEY_STATE_START:
                if ((key->state_read)() == key->pressed_state) {
                        key->state = KEY_STATE_DEBOUNCE;
                        break;
                }
                break;
        case KEY_STATE_DEBOUNCE:
                if (key->time_debounce < KEY_TIME_DEBOUNCE) {
                        key->time_debounce++;
                        break;
                }

                if ((key->state_read)() == key->pressed_state) {
                        /* The button was actually pressed. */
                        key->state = KEY_STATE_PRESSED;
                } else {
                        /* it is a bounce. */
                        __key_reset(key);
                }
                break;
        case KEY_STATE_PRESSED:
                if ((key->state_read)() != key->pressed_state) {
                        /* it is a click. */
                        key->click_cnt++;
                        key->state = KEY_STATE_CLICKS;
                        break;
                }

                if (key->time_long < KEY_TIME_LONG_PRESS) {
                        key->time_long++;
                        break;
                }

                /* it is a long press. */
                key->state = KEY_STATE_LONG_PRESS;
                break;
        case KEY_STATE_CLICKS:
                if (key->time_between_clicks < KEY_TIME_BETWEEN_CLICKS) {
                        key->time_between_clicks++;
                        if ((key->state_read)() == key->pressed_state) {
                                key->state = KEY_STATE_DEBOUNCE;
                        }
                        break;
                }
                (key->clicks[key->click_cnt])(keys);
                __key_reset(key);
                break;
        case KEY_STATE_LONG_PRESS:
                /* if this key register combination keys, check them. */
                if (key->combi_key_num > 0) {
                        if (!check_combi_keys(key)) {
                                (key->long_handle)(keys);
                        }
                }

                __wait_all_key_release(keys);
                __key_reset(key);
                break;
        }

        if (index++ >= sizeof(keys) / sizeof(keys[0]) - 1) {
                /* start scan again! */
                index = 0;
        }
}

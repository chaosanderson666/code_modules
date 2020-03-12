/*
 * Copyright (C) 2011 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <sys/time.h>
#include <unistd.h>

#include <libubox/ustream.h>

#include "libubus.h"

static struct ubus_context *ctx;
static struct blob_buf b;
static char result[UBUS_MAX_MSGLEN];

static void client_subscribe_cb(struct ubus_context *ctx, struct ubus_object *obj)
{
	fprintf(stderr, "Subscribers active: %d\n", obj->has_subscribers);
}

static struct ubus_object client_object = {
	.subscribe_cb = client_subscribe_cb,
};

enum {
	RESULT,
	__RESULT_MAX,
};

static const struct blobmsg_policy return_policy[__RESULT_MAX] = {
	[RESULT] = { .name = "result", .type = BLOBMSG_TYPE_STRING},
};

static void invoke_cb(struct ubus_request *req,
				    int type, struct blob_attr *msg)
{
	struct blob_attr *tb[__RESULT_MAX];
	char *result;

	fprintf(stderr, "Enter invoke callback function\n");

	if (blobmsg_parse(return_policy, __RESULT_MAX, tb, 
					blob_data(msg), blob_len(msg)) != 0) {
		fprintf(stderr, "Failed to parse blofmsg\n");
		return;
	}
	

	if (!tb[RESULT]) {
		fprintf(stderr, "No return code received from server\n");
		return;
	}

	result = blobmsg_get_string(tb[RESULT]);
	if (!result) {
		fprintf(stderr, "Failed to get string\n");
        return;
	}
	strcpy(req->priv, result);
}

static int client_main(void)
{
	uint32_t id;
	int ret;
	
	//the max len of msg is controled by UBUS_MAX_MSGLEN, defined in CMakeLists.txt.
	char *str = "{\"params\":{\"content\":\"how you doing\"}}";

	//get the ubus connection context from ubusd.
	ret = ubus_add_object(ctx, &client_object);
	if (ret) {
		fprintf(stderr, "Failed to add_object object: %s\n", ubus_strerror(ret));
		return -1;
	}

	//look up the target object id by the object path in ubus context.
	ret = ubus_lookup_id(ctx, "test", &id);
	if (ret) {
		fprintf(stderr, "Failed to look up object: %s\n", ubus_strerror(ret));
		return -1;
	}

	//send the request to server.
	if (blob_buf_init(&b, 0) != 0) {
		fprintf(stderr, "Failed to init blob buf\n");
		return -1;
	}

	if (blobmsg_add_string(&b, "request", str) != 0) {
		fprintf(stderr, "Failed to add blob msg\n");
		return -1;
	}

	fprintf(stderr, "%s\n", str);
    
    memset(result, 0, sizeof(result));
	//when timeout hanpped, will not call the cb func, just goto finish.
	ret = ubus_invoke(ctx, id, "method", b.head, invoke_cb, result, 3000);
	if (ret) {
		fprintf(stderr, "invoke ret: %s\n", ubus_strerror(ret));
		return;
	}

    if (strlen(result) == 0) {
        return -1;
    }
    
	fprintf(stderr, "finish: %s\n", result);
}

int main(int argc, char **argv)
{
	const char *ubus_socket = NULL;
	int ch;

	while ((ch = getopt(argc, argv, "cs:")) != -1) {
		switch (ch) {
		case 's':
			ubus_socket = optarg;
			break;
		default:
			break;
		}
	}

	argc -= optind;
	argv += optind;

	uloop_init();

	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);

	client_main();

	ubus_free(ctx);
	uloop_done();

	return 0;
}

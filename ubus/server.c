/*
 * Copyright (C) 2011-2014 Felix Fietkau <nbd@openwrt.org>
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
#include <unistd.h>
#include <signal.h>

#include <libubox/blobmsg_json.h>
#include "libubus.h"
#include "count.h"

static struct ubus_context *ctx;
static struct blob_buf b;

enum {
    API,
    __MAX
};

static const struct blobmsg_policy policy[__MAX] = {
    [API] = { .name = "request", .type = BLOBMSG_TYPE_STRING },
};

static int method(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[__MAX];
	char *s1;

	blobmsg_parse(policy, __MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[API]) {
		return UBUS_STATUS_INVALID_ARGUMENT;
	}
	s1 = blobmsg_get_string(tb[API]);
	if (!s1) {
		free(s1);
		return UBUS_STATUS_UNKNOWN_ERROR;
	}
	fprintf(stderr, "%s\n", s1);

	blob_buf_init(&b, 0);
    char *str = "{\"result\":\"ok\"}";
	blobmsg_add_string(&b, "result", str); 

	ubus_send_reply(ctx, req, b.head);
	fprintf(stderr, "%s\n", str);

	return 0;
}

static const struct ubus_method test_methods[] = {
	UBUS_METHOD("method", method, policy),
};

static struct ubus_object_type test_object_type =
	UBUS_OBJECT_TYPE("test", test_methods);

static struct ubus_object test_object = {
	.name = "test",
	.type = &test_object_type,
	.methods = test_methods,
	.n_methods = ARRAY_SIZE(test_methods),
};

static void server_main(void)
{
	int ret;

	ret = ubus_add_object(ctx, &test_object);
	if (ret) {
		fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(ret));
		return;
	}

	uloop_run();
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
	signal(SIGPIPE, SIG_IGN);

	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);

	server_main();

	ubus_free(ctx);
	uloop_done();

	return 0;
}

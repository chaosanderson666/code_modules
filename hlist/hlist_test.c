#include <stdio.h>
#include <stdlib.h>
#include "hlist.h"

#define SIZE 100

typedef struct test_data {
	struct hlist_node hnode;
	int key;
	int value;
}test_t;

struct hlist_table {
	struct hlist_head *hhead;
};

int mapping_func(int key)
{
	return key % SIZE;
}

int hlist_table_init(struct hlist_table *htable)
{
	int index;

	htable = (struct hlist_table *)calloc(1, sizeof(struct hlist_table));
	if(!htable) return -1;
	htable->hhead = (struct hlist_head *)calloc(SIZE, sizeof(struct hlist_head));
	if(!htable->hhead) return -1;
	
	for(index = 0; index < SIZE; index++){
		INIT_HLIST_HEAD(&(htable->hhead[index]));
	}
	
	return 0;
}

test_t *hlist_find_node(struct hlist_table *htable, test_t *test)
{
	test_t *pos;
	struct hlist_head *head;
	
	head = &(htable->hhead[mapping_func(test->key)]);
	hlist_for_each_entry(pos, head, hnode){
		if(pos->key == test->key){
			return pos;
		}
	}
	return NULL;
}

int test_hinsert(struct hlist_table *htable, test_t *new)
{
	struct hlist_head *head;
	
	if(hlist_find_node(htable, new))
		return -1;
	
	INIT_HLIST_NODE(&new->hnode);
	
	head = &(htable->hhead[mapping_func(new->key)]);
	hlist_add_head(&new->hnode, head);
}

int main()
{
	int i;
	test_t *test;
	struct hlist_table *htable;

	hlist_table_init(htable);

	for(i = 0; i < 1000000; i++){
		test = (test_t *)calloc(1, sizeof(test_t));
		test->key = i;
		test->value= 10 + i;
		
		printf("insert: %d\n", test->key);
		test_hinsert(htable, test);
	}

	printf("insert done\n");

	for(i = 0; i < 100000; i++){
		test = (test_t *)calloc(1, sizeof(test_t));
		test->key = i;
		test = hlist_find_node(htable, test);
		printf("value: %d", test->value);
	}
//	while(1);
	return 0;
}

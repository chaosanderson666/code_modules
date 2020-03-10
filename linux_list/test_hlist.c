#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_list.h"

struct person {
        struct hlist_node hnode;
        int age;
        int weight;
};

#define HASH_NUMBER 7

struct hlist_head hashtbl[HASH_NUMBER];

static inline int hash_func(int k)
{
        return k % HASH_NUMBER;
}

void main(void)
{
        struct hlist_node *hn;
        struct person *temp_user_struct;

        int i;
        int index;
        for (i = 0; i < HASH_NUMBER; i++) {
                INIT_HLIST_HEAD(&hashtbl[i]);
        }

        for (i = 0; i < 17; i++) {
                temp_user_struct = (struct person *)malloc(sizeof(struct person));
                INIT_HLIST_NODE(&temp_user_struct->hnode);
                temp_user_struct->age = i;
                temp_user_struct->weight = i + 1;
                index = hash_func(temp_user_struct->age);
                hlist_add_head(&temp_user_struct->hnode, &hashtbl[index]);
        }
        printf("======== after add hlist ========\n");
        for (i = 0; i < HASH_NUMBER; i++) {
                hlist_for_each_entry(temp_user_struct, &hashtbl[i], hnode) {
                        printf("index = %d, age = %d, weight = %d\n", i, temp_user_struct->age,
                               temp_user_struct->weight);
                }
        }

        int j = 0;
        int age;
        srand(time(NULL));
        age = rand() % 17;
        index = hash_func(age);
        printf("======== search age = %d ========\n", age);
        hlist_for_each_entry_safe(temp_user_struct, hn, &hashtbl[index], hnode) {
                printf("hlist_for_each_entry = %d\n", j++);
                if (temp_user_struct->age == age) {
                        printf("found: age = %d, weight: %d, index = %d\n", temp_user_struct->age,
                               temp_user_struct->weight, index);
                        printf("======== delete age = %d ========\n", age);
                        hlist_del(&temp_user_struct->hnode);
                        break;
                }
        }

        printf("======== after delete age = %d ========\n", age);
        for (i = 0; i < HASH_NUMBER; i++) {
                hlist_for_each_entry(temp_user_struct, &hashtbl[i], hnode) {
                        printf("index = %d, age = %d, weight = %d\n", i, temp_user_struct->age,
                               temp_user_struct->weight);
                }
        }
}

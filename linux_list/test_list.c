#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_list.h"

struct person_head {
        struct list_head list;
        int total;
};
struct person {
        struct list_head list;
        int age;
        int weight;
};

int main(void)
{
        int age;
        int weight;
        struct list_head *pos;
        struct list_head *n;
        struct person_head phead;
        struct person *temp_user_struct;
        struct person *temp_user_struct_n;

        INIT_LIST_HEAD(&phead.list);
        phead.total = 0;

        for (age = 10, weight = 35; age < 15; age += 1, weight += 1) {
                temp_user_struct = (struct person *)malloc(sizeof(struct person));
                temp_user_struct->age = age;
                temp_user_struct->weight = weight;
                list_add(&temp_user_struct->list, &phead.list);
                phead.total++;
        }

        for (age = 50, weight = 60; age < 55; age += 1, weight += 1) {
                temp_user_struct = (struct person *)malloc(sizeof(struct person));
                temp_user_struct->age = age;
                temp_user_struct->weight = weight;
                list_add_tail(&temp_user_struct->list, &phead.list);
                phead.total++;
        }

        printf("======== list_for_each ========\n");
        list_for_each(pos, &phead.list) {
                temp_user_struct = list_entry(pos, struct person, list);
                printf("age:%d,      weight:    %d \n", temp_user_struct->age,
                       temp_user_struct->weight);
        }
        printf("total: %d\n", phead.total);

        printf("======== list_for_each_entry ========\n");
        list_for_each_entry(temp_user_struct, &phead.list, list) {
                printf("age:%d,      weight:    %d \n", temp_user_struct->age,
                       temp_user_struct->weight);
        }
        printf("total: %d\n", phead.total);

        printf("======== list_for_each_entry_reverse ========\n");
        list_for_each_entry_reverse(temp_user_struct, &phead.list, list) {
                printf("age:%d,      weight:    %d \n", temp_user_struct->age,
                       temp_user_struct->weight);
        }
        printf("total: %d\n", phead.total);

        list_for_each_safe(pos, n, &phead.list) {
                temp_user_struct = list_entry(pos, struct person, list);
                if (temp_user_struct->age == 12) {
                        printf("======== delete age = %d ========\n", temp_user_struct->age);
                        list_del(pos);
                        phead.total--;
                        free(temp_user_struct);
                }
        }

        printf("======== after delete node ========\n");
        list_for_each_entry_safe(temp_user_struct, temp_user_struct_n, &phead.list,
                                 list) {
                printf("age:%d,      weight:    %d \n", temp_user_struct->age,
                       temp_user_struct->weight);
        }
        printf("total: %d\n", phead.total);

        list_for_each_entry_safe_reverse(temp_user_struct, temp_user_struct_n,
                                         &phead.list, list) {
                if (temp_user_struct->age == 53) {
                        printf("======== delete age = %d ========\n", temp_user_struct->age);
                        list_del_init(&temp_user_struct->list);
                        phead.total--;
                        free(temp_user_struct);
                }
        }
        printf("======== after delete node ========\n");
        list_for_each_entry_safe(temp_user_struct, temp_user_struct_n, &phead.list,
                                 list) {
                printf("age:%d,      weight:    %d \n", temp_user_struct->age,
                       temp_user_struct->weight);
        }
        printf("total: %d\n", phead.total);

        return 0;
}

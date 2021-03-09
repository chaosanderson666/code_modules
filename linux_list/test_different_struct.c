#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_list.h"

struct person_head {
        struct list_head list;
        int total;
};

struct general {
       struct list_head list;
       int type;
       int id;
};

struct person {
        struct general gen;
        int age;
        int plan[5];
        int hight;
};

struct animal {
        struct general gen;
        int species;
};

struct person_head phead;

struct general *get_iterm_by_type_id(int type, int id)
{
        struct list_head *pos;
        struct general *temp_general;

        list_for_each(pos, &phead.list) {
                temp_general = list_entry(pos, struct general, list);
                printf("type: %d, id: %d\n", temp_general->type, temp_general->id);
                if (temp_general->type == type && temp_general->id == id) {
                        return temp_general;
                }
        }
        return NULL;
}

int add_iterm(struct general *gen)
{
	list_add(&gen->list, &phead.list);
	phead.total++;
	return 0;
}

int main(void)
{
        int i;
        struct general *temp_general;
        struct person *temp_person;
        struct animal *temp_animal;

        INIT_LIST_HEAD(&phead.list);
        phead.total = 0;

        for (i = 1; i < 5; i++) {
                temp_person = (struct person *)malloc(sizeof(struct person));
                temp_person->gen.type = 0; /* means person */
                temp_person->gen.id = i;
                temp_person->age = i + 1;
                temp_person->plan[0] = i + 2;
                temp_person->plan[1] = i + 3;
                temp_person->plan[2] = i + 4;
                temp_person->hight = i + 1;
                list_add(&temp_person->gen.list, &phead.list);
                phead.total++;
        }

        for (i = 1; i < 5; i++) {
                temp_animal = (struct animal *)malloc(sizeof(struct animal));
                temp_animal->gen.type = 1; /* means animal */
                temp_animal->gen.id = i;
                temp_animal->species = i + 1;
                list_add(&temp_animal->gen.list, &phead.list);
                phead.total++;
        }

        temp_general = get_iterm_by_type_id(2, 3);
        if (temp_general) {
                temp_person = (struct person *)temp_general;
                printf("type: %d, id: %d, age: %d, plan[0]: %d\n", temp_person->gen.type, temp_person->gen.id, temp_person->age,
                        temp_person->plan[0]);
        }

        temp_general = get_iterm_by_type_id(1, 2);
        if (temp_general) {
                temp_animal = (struct animal *)temp_general;
                printf("type: %d, id: %d, species: %d\n", temp_animal->gen.type, temp_animal->gen.id, temp_animal->species);
        }

        return 0;
}

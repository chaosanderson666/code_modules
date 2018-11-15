/***********************************************************************************************************                                             Command line**                            (c) Copyright 1999, John , All Rights Reserved**********************************************************************************************************/#define LIST_INIT(head_name) \         static list_head head_name); \		 init_list_head(&(head_name));		 #define offsetof(type, member)  (size_t)(&((type*)0)->member)#define container_of(ptr, type, member) ({          \        const typeof(((type *)0)->member)*__mptr = (ptr);    \    (type *)((char *)__mptr - offsetof(type, member)); })	#define list_entry(ptr, type, member)  container_of(ptr, type, member)#define list_first_entry(ptr, type, member)  list_entry((ptr)->next, type, member)#define list_for_each(pos, head) \    for (pos = (head)->next; pos != (head); pos = pos->next)	struct list_head{    struct list_head *next;};static inline void init_list_head(struct list_head *list){    list->next = NULL;}static inline void list_add_first(struct list_head *head, struct list_head *new){ 	new->next = head->next;	head->next = new;}struct list_head *list_tail(struct list_head *head){  if(head == NULL) {    return head;  }  for(struct list *l = head; l->next != NULL; l = l->next);    return l;}static inline void list_add_tail(struct list_head *head, struct list_head *new){ 	struct list_head *tail;		tail = list_tail(head);	list_add_first(new, tail);}static inline  void list_del(struct list_head *head, struct list_head *node){    head->next = node->next;    node->next = NULL;}
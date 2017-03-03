#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
/*single list element define*/
typedef struct listElement
{
	void *data;
	struct listElement *next;
}ListElem;

/*define a list structer*/

typedef struct _list
{
	int size;
	int (*match)(const void *key1,const void *key2);
	void (*destroy)(void *data);
	ListElem *head;
	ListElem *tail;
}List;

/*public interface*/
void list_init(List *list,void (*destroy)(void* data));
void list_destroy(List *list);
int list_insert_next(List *list,ListElem *element,const void *data);
int list_remove_next(List *list,ListElem *element,void **data);
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define is_list_head(list,elem) ((elem)==(list)->head?1:0)
#define is_list_tail(elem) ((elem)->next == NULL?1:0)
#define list_data(elem) ((elem)->data)
#define list_next(elem) ((elem)->next)


#endif

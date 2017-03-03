#include <stdlib.h>
#include <string.h>

#include "list.h"

void list_init(List *list,void (*destroy)(void* data))
{
	list->size	= 0;
	list->destroy	= destroy;
	list->head	= NULL;
	list->tail	= NULL;
	
	return ;
}

void list_destroy(List *list)
{
	void *data;
	
	while(list_size(list) > 0)
	{
		if(list_remove_next(list,NULL,(void** )&data) == 0 && list->destroy != NULL)
		{
			list->destroy(data);
		}
	}
	
	memset(list,0,sizeof(List));		
}

int list_insert_next(List *list,ListElem *element,const void *data)
{
	ListElem *new_elem;
	if((new_elem = (ListElem*)malloc(sizeof(ListElem) == NULL)))
		return -1;

	new_elem->data = (void*)data;
	if(element == NULL)
	{
		if(list_size(list) ==0 )
		{
			list->tail = new_elem;
		}
		new_elem->next 	= list->head;
		list->head		= new_elem;
	}
	else
	{
		if(element->next == NULL)
			list->tail = new_elem;
		new_elem->next	= element->next;
		element->next 	= new_elem;
	}

	list->size++;
	return 0;
}

int list_remove_next(List *list,ListElem *element,void **data)
{
	ListElem *old_elem;
	if(list_size(list) == 0)
		return -1;

	if(element == NULL)
	{
		*data 		= list->head->data;
		old_elem 	= list->head;
		list->head 	= list->head->next;
		if(list_size(list) == 1)
				list->tail = NULL;
	}	
	else
	{
		if(element->next == NULL)
			return -1;
		*data 		= element->next->data;
		old_elem	= element->next;
		element->next	= element->next->next;
		if(element->next == NULL)
			list->tail = element;
			
	}
	free(old_elem);
	list->size--;
	return 0;
}

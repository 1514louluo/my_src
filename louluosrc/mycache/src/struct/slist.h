/*
*	Copyright MIT
*	A single list!
*	author: louluo  2014
*/
#ifndef _SLIST_H
#define _SLIST_H

struct slist
{
	struct slist *next;
};

#define SLIST_HEAD_INIT {NULL}

#define SLIST_HEAD   SLIST_HEAD_INIT

#define slist_entry(ptr, type, member) \
	container_of(ptr, type, member)


void INIT_SLIST_HEAD(struct slist *list)
{
	list->next = NULL;
}

void slist_filo_push(struct slist *new, struct slist *head)
{
	new->next = head->next;
	head->next = new;
}

struct slist *slist_filo_pop(struct slist *head)
{
	struct slist *del = head->next;
	head->next = del->next;
	return del;
}

#endif //_SLIST_H




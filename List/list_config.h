#ifndef	_ADT_ARRAY_BASED_LIST_
#define _ADT_ARRAY_BASED_LIST_


/* Specify the type of the list entries */
typedef Plane_t ListEntry;

/* Declare the type node */
typedef struct listnode{
	ListEntry Item;
	struct listnode *next;
}ListNode;

/* Declare the List type */
typedef struct{
	int size;
	ListNode* head;
}List_t;

#endif
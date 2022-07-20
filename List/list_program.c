#include <stdio.h>
#include <stdlib.h>
#include "Error_States.h"
#include "..\Time\time_config.h"
#include "..\Plane\plane_config.h"
#include "list_config.h"



/*
	pre: none
	post: Initialize the list and returns enum(LIST_CREATED)
	complexity: O(1)
*/
ListErrorStates_t CreateList(List_t* pList){
	pList->head = NULL;
	pList->size = 0;
	return LIST_CREATED;
}

/*
	pre: list is initialized and not empty
	post: - deletes the whole list elements
	      - if the list is not empty, it returns (LIST_DESTROYED)
		  - if the list is empty, it returns (LIST_EMPTY)
	complexity: O(N)
*/
ListErrorStates_t DestroyList(List_t* pList){
	if(pList->head != NULL){ // list is not empty
		ListNode* node; 
		while(pList->head){
			node = pList->head->next;
			free(pList->head);
			pList->head = node;
		}
		pList->size = 0;
		return LIST_DESTROYED;
	}
	else{
		return LIST_EMPTY;
	}
}

/*
	pre: list is initialized
	post: - checks if the list is empty or not
	      - if the list is not empty, it returns (LIST_NOT_EMPTY)
		  - if the list is empty, it returns (LIST_EMPTY)
	complexity: O(1)
*/
ListErrorStates_t ListEmpty(List_t List){
	return List.head == NULL? LIST_EMPTY : LIST_NOT_EMPTY;
}

/*
	pre: list is initialized
	post: - checks if the list is full or not
	      - if the list is not full, it returns (LIST_NOT_FULL)
		  - if the list is full, it returns (LIST_FULL)
	complexity: O(1)
*/
ListErrorStates_t ListFull(List_t List){
	return LIST_NOT_FULL;
}

/*
	pre: list is initialized
	post: - gets the current size of the list
	      - if the list is not empty, it returns (LIST_NOT_EMPTY)
		  - if the list is empty, it returns (LIST_EMPTY)
	complexity: O(1)
*/
ListErrorStates_t ListSize(int* pSize, List_t List){
	*pSize = List.size;
	return List.head == NULL? LIST_EMPTY : LIST_NOT_EMPTY;
}

/*
	pre: - list is initialized and not full
	     - 0 <= index <= current size of the list
	post: - inserts an element to a specific index in the list
	      - if the list is not full, it returns (LIST_INSERTION_DONE)
		  - if the list is full, it returns (LIST_FULL)
		  - returns (LIST_NULL_POINTER) if it faild to create a node
	complexity: O(N)
*/
ListErrorStates_t ListInsert(int index, ListEntry Entry, List_t* pList){
	ListNode* NewNode = (ListNode*) malloc(sizeof(ListNode));
	if(NewNode != NULL){
		NewNode->Item = Entry;
		if(index != 0){
			ListNode* node;
			int i;
			for(node = pList->head, i = 0; i < index - 1; i++, node = node->next)
				;
			NewNode->next = node->next;
			node->next = NewNode;
		}
		else{
			NewNode->next = pList->head;
			pList->head = NewNode;
		}
		pList->size++;
		return LIST_INSERTION_DONE;
	}
	else{
		return LIST_NULL_POINTER;
	}
}

/*
	pre: - list is initialized and not empty
	     - 0 <= index <= (current size of the list) - 1
	post: - deletes an element from a specific index in the list
	      - if the list is not empty, it returns (LIST_DELETION_DONE)
		  - if the list is empty, it returns (LIST_EMPTY)
	complexity: O(N)
*/
ListErrorStates_t ListDelete(int index, ListEntry* pEntry, List_t* pList){
	if(pList->size != 0){
		ListNode* node = NULL, *prev = NULL;
		int i;
		for(node = pList->head, i = 0; i < index; i++, node = node->next)
			prev = node;
		
		*pEntry = node->Item;
		if(prev != NULL){ // not the first node
			prev->next = node->next;
		}
		else{
			pList->head = node->next;
		}
		free(node);
		node = NULL;
		pList->size--;
		return LIST_DELETION_DONE;
	}
	else{
		return LIST_EMPTY;
	}
}

/*
	pre: - list is initialized and not empty
	     - 0 <= index <= (current size of the list) - 1
	post: - getd an element from a specific index in the list
	      - if the list is not empty, it returns (LIST_RETRIVE_DONE)
		  - if the list is empty, it returns (LIST_EMPTY)
	complexity: O(N)
*/
ListErrorStates_t ListRetrieve(int index, ListEntry* pEntry, List_t List){
	if(List.size != 0){
		int i;
		ListNode* node = List.head;
		for(i = 0; i < index; i++)
			node = node->next;
		
		*pEntry = node->Item;
		return LIST_RETRIVE_DONE;
	}
	else{
		return LIST_EMPTY;
	}
}

/*
	pre: - list is initialized and not empty
	     - 0 <= index <= (current size of the list) - 1
	post: - replace an element value in a specific index in the list
	      - if the list is not empty, it returns (LIST_REPLACEMENT_DONE)
		  - if the list is empty, it returns (LIST_EMPTY)
	complexity: O(N)
*/
ListErrorStates_t ListReplace(int index, ListEntry Entry, List_t* pList){
	if(pList->size != 0){
		int i;
		ListNode* node = pList->head;
		for(i = 0; i < index; i++)
			node = node->next;
		
		node->Item = Entry;
		
		return LIST_REPLACEMENT_DONE;
	}
	else{
		return LIST_EMPTY;
	}
}

/*
	pre: - list is initialized and not empty
	post: - enables the user to visit the list elements
	      - if the list is not empty, it returns (LIST_TRAVERSED)
		  - if the list is empty, it returns (LIST_EMPTY)
	complexity: O(N)
*/
ListErrorStates_t ListTraverse(List_t List, void (*pf) (ListEntry)){
	if(List.size != 0){
		for(ListNode* node = List.head; node; node = node->next)
			(*pf)(node->Item);
		
		return LIST_TRAVERSED;
	}
	else{
		return LIST_EMPTY;
	}
}
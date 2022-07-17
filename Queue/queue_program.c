#include <stdio.h>
#include <stdlib.h>
#include "Error_States.h"
#include "..\Plane\Plane_config.h"
#include "queue_config.h"




ErrorStates_t	CreateQueue(Queue_t* pQueue){
	pQueue->front = NULL;
	pQueue->rear = NULL;
	pQueue->size = 0;
	return QUEUE_CREATED;
}


ErrorStates_t 	ClearQueue(Queue_t* pQueue){
	if(pQueue->size != 0){
		
		while(pQueue->front){
			pQueue->rear = pQueue->front->next;
			free(pQueue->front);
			pQueue->front = pQueue->rear;
		}
		pQueue->size = 0;
		return QUEUE_CLEARED;
	}else{
		return QUEUE_EMPTY;
	}
}


ErrorStates_t	Push(QueueEntry Entry, Queue_t* pQueue){
	QueueNode* NewNode = (QueueNode*) malloc(sizeof(QueueNode));
	if(NewNode != NULL){
		NewNode->Item = Entry;
		NewNode->next = NULL;
		
		if(pQueue->front) // not the first node
			pQueue->rear->next = NewNode;
		else{
			pQueue->front = NewNode;
		}
		pQueue->rear = NewNode;
		pQueue->size++;
		return QUEUE_PUSHED_SUCCESSFULLY;
	}
	else{
		return  NULL_POINTER;
	}
}

/*
	pre: queue is initialized and not empty
	post: - pops an entry from the queue if it's not empty and returns enum(QUEUE_POPPED_SUCCESSFULLY)
		  - if the queue is empty it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	Pop(QueueEntry* pEntry, Queue_t* pQueue){
	if(pQueue->size != 0){
		QueueNode* OldNode;
		*pEntry = pQueue->front->Item;
		OldNode = pQueue->front;
		pQueue->front = pQueue->front->next;
		
		if(pQueue->front) // One node only was in the queue
			pQueue->rear = pQueue->front;
		
		pQueue->size--;
		
		free(OldNode);
		OldNode = NULL;
		return QUEUE_POPPED_SUCCESSFULLY;
	}
	else{
		return QUEUE_EMPTY;
	}
}

/*
	pre: queue is initialized
	post: - gets the number of elements in the queue if it's not empty it returns enum(QUEUE_NOT_EMPTY)
		  - if the queue is empty it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueSize(int* Size, Queue_t Queue){
	
	return (*Size = Queue.size) == 0? QUEUE_EMPTY : QUEUE_NOT_EMPTY;
}

/*
	pre: queue is initialized and not empty
	post: - gets the element in the front of the queue 
		  - if it's not empty it returns enum(QUEUE_FRONT_REVEILED)
		  - if the queue is full it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueFront(QueueEntry* Front, Queue_t Queue){
	if(Queue.size != 0){
		*Front = Queue.front->Item;
		return QUEUE_FRONT_REVEILED;
	}
	else{
		return QUEUE_EMPTY;
	}
}

/*
	pre: queue is initialized and not empty
	post: - gets the element in the rear of the queue 
		  - if it's not empty it returns enum(QUEUE_REAR_REVEILED)
		  - if the queue is full it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueRear(QueueEntry* Rear, Queue_t Queue){
	if(Queue.size != 0){
		*Rear = Queue.rear->Item;
		return QUEUE_REAR_REVEILED;
	}
	else{
		return QUEUE_EMPTY;
	}
}

/*
	pre: queue is initialized
	post: - if it's not empty it returns enum(QUEUE_NOT_EMPTY)
		  - if the queue is full it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueEmpty(Queue_t Queue){
	return Queue.size == 0? QUEUE_EMPTY : QUEUE_NOT_EMPTY;
}

/*
	pre: queue is initialized
	post: - if it's not full it returns enum(QUEUE_NOT_FULL)
		  - if the queue is full it returns enum(QUEUE_FULL)
*/
ErrorStates_t	QueueFull(Queue_t Queue){
	return QUEUE_NOT_FULL;
}

/*
	pre: queue is initialized and not empty
	post: - executes a function built by the user to interact with the queue's entries 
		  - if it's not empty it returns enum(QUEUE_REAR_REVEILED)
		  - if the queue is full it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueTraverse(Queue_t Queue, void (*pf) (QueueEntry)){
	if(Queue.size != 0){
		for(QueueNode* node = Queue.front; node; node = node->next)
			pf(node->Item);
		
		return QUEUE_TRAVERSED;
	}
	else{
		return QUEUE_EMPTY;
	}
}
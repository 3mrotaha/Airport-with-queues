#ifndef _ADT_BASED_ARRAY_QUEUE_
#define _ADT_BASED_ARRAY_QUEUE_


/* Specify the type of the queue entries*/
typedef Plane_t QueueEntry;

/* Declaration of the type node */
typedef struct queuenode{
	QueueEntry Item;
	struct queuenode* next;
}QueueNode;

/* Declaration of the queue type */
typedef struct{
	QueueNode* front;
	QueueNode* rear;
	int size;
}Queue_t;

#endif
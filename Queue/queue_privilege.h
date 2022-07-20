#ifndef	_ADT_BASED_ARRAY_QUEUE_PRIVILEGE_
#define _ADT_BASED_ARRAY_QUEUE_PRIVILEGE_

/*
	pre: none
	post: Initializing the queue, returns enum (QUEUE_CREATED)
*/
ErrorStates_t	CreateQueue(Queue_t*);

/*
	pre: queue is initialized and not empty
	post: - clears the queue if it's not empty and returns enum(QUEUE_CLEARED)
		  - if the queue is empty it returns (QUEUE_EMPTY)
*/
ErrorStates_t 	ClearQueue(Queue_t*);

/*
	pre: queue is initialized and not full
	post: - pushes a new entry to the queue if it's not full and returns enum(QUEUE_PUSHED_SUCCESSFULLY)
		  - if the queue is full it returns (QUEUE_FULL)
*/
ErrorStates_t	Push(QueueEntry, Queue_t*);

/*
	pre: queue is initialized and not empty
	post: - pops an entry from the queue if it's not empty and returns enum(QUEUE_POPPED_SUCCESSFULLY)
		  - if the queue is empty it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	Pop(QueueEntry*, Queue_t*);

/*
	pre: queue is initialized
	post: - gets the number of elements in the queue if it's not empty it returns enum(QUEUE_NOT_EMPTY)
		  - if the queue is empty it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueSize(int*, Queue_t);

/*
	pre: queue is initialized and not empty
	post: - gets the element in the front of the queue 
		  - if it's not empty it returns enum(QUEUE_FRONT_REVEILED)
		  - if the queue is full it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueFront(QueueEntry*, Queue_t);

/*
	pre: queue is initialized and not empty
	post: - gets the element in the rear of the queue 
		  - if it's not empty it returns enum(QUEUE_REAR_REVEILED)
		  - if the queue is full it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueRear(QueueEntry*, Queue_t);

/*
	pre: queue is initialized
	post: - if it's not empty it returns enum(QUEUE_NOT_EMPTY)
		  - if the queue is full it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueEmpty(Queue_t);

/*
	pre: queue is initialized
	post: - if it's not full it returns enum(QUEUE_NOT_FULL)
		  - if the queue is full it returns enum(QUEUE_FULL)
*/
ErrorStates_t	QueueFull(Queue_t);

/*
	pre: queue is initialized and NotEmpty
	post: - if it's not Empty it returns enum(QUEUE_FRONT_REVEILED)
		  - if the queue is empty it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	ChangeFront(QueueEntry, Queue_t*);


/*
	pre: queue is initialized and not empty
	post: - executes a function built by the user to interact with the queue's entries 
		  - if it's not empty it returns enum(QUEUE_TRAVERSED)
		  - if the queue is empty it returns enum(QUEUE_EMPTY)
*/
ErrorStates_t	QueueTraverse(Queue_t, void (*) (QueueEntry));

#endif
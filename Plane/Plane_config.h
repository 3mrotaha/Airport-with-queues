#ifndef _PLANE_TYPE_
#define _PLANE_TYPE_

/* Declaration : the flight type for each plane contains the destination of the plane
                 and the time the plane will take to reach its destination. 
*/
typedef struct Flights{
	char* Destination;
	
	Time_t	FlyDuration;
}Flight_t;


/* Declaration : the status type for each plane refers to the current plane 
				 status.
*/
typedef enum PlaneStatus{
	PLANE_ON_AIR,
	
	PLANE_ON_GROUND,
	
	READY_TO_FLY,
	READY_TO_LAND,
	
	PLANE_FLYING,
	PLANE_LANDING,
	
	PLANE_ON_AIR_BUT_HAVE_TO_WAIT,
	PLANE_ON_GROUND_BUT_HAVE_TO_WAIT
}PlaneStatus_t;


/* Declaration: the plane type is used to create a plane and gives it all the 
                necessary flight information.
*/
typedef struct Plane{
	int id;
	
	PlaneStatus_t status;
	
	Flight_t Flight;
	
	Time_t TakeoffTime;
	Time_t ActualTakeoffTime;
	Time_t RunWayTime;
	Time_t LandTime;
	
}Plane_t;


#endif
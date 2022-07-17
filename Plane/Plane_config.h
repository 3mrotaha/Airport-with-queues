#ifndef _PLANE_TYPE_
#define _PLANE_TYPE_

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

typedef struct time{
	int hr;
	int min;
	int sec;
}Time_t;


typedef struct Plane{
	int id;
	
	PlaneStatus_t status;
	
	Time_t TakeoffTime;
	Time_t ActualTakeoffTime;
	Time_t FlightTime;
	
	Time_t RunWayTime;
	
	Time_t LandTime;
	
	
}Plane_t;


#endif
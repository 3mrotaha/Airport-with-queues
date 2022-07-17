#include <stdio.h>
#include "Plane_ErrorStates.h"
#include "Plane_config.h"
#include "plane_private.h"


static int planeID = 0;
static float planeSpeed = 142; // km/hr
static const int WayLength = 3500; // m

PlaneError_t	CreatePlane(Plane_t* pPlane){
	if(pPlane != NULL){
		pPlane->id = ++planeID;
		pPlane->status = PLANE_ON_GROUND;
		
		InitializeTime(&pPlane->TakeoffTime);
		InitializeTime(&pPlane->ActualTakeoffTime);
		InitializeTime(&pPlane->LandTime);
		InitializeTime(&pPlane->FlightTime);
		
		/* speed (km/hr) = distance (km) / time (hr) */
		float time = (WayLength / 1000.0) / planeSpeed;
		planeSpeed += 50.75;
		pPlane->RunWayTime.hr = (int) time;
		pPlane->RunWayTime.min = (int)((time - pPlane->RunWayTime.hr) * 60.0);
		pPlane->RunWayTime.sec = (((time - pPlane->RunWayTime.hr) * 60.0) - pPlane->RunWayTime.min) * 60;
		return PLANE_CREATED;
	}
	else{
		return PLANE_CREATION_ERROR;
	}
}

PlaneError_t	SetTakeOffTime(Time_t time, Plane_t* pPlane){
	if(!IsSameTime(pPlane->LandTime, time)){
		pPlane->TakeoffTime.hr = time.hr;
		pPlane->TakeoffTime.min = time.min;
		pPlane->TakeoffTime.sec = time.sec;
		return TIME_SET;
	}
	else{
		return NO_NEED;
	}
}

PlaneError_t	SetFlightTime(Time_t time, Plane_t* pPlane){
	if(!IsSameTime(pPlane->LandTime, time)){
		pPlane->LandTime.hr = time.hr;
		pPlane->LandTime.min = time.min;
		pPlane->LandTime.sec = time.sec;
		return TIME_SET;
	}
	else{
		return NO_NEED;
	}
}


PlaneError_t	PlaneFly(Time_t time, Plane_t** pPlane){
	if(*(pPlane)->status != PLANE_ON_AIR){
		*(pPlane)->ActualTakeoffTime.hr = time.hr;
		*(pPlane)->ActualTakeoffTime.min = time.min;
		*(pPlane)->ActualTakeoffTime.sec = time.sec;
		*(pPlane)->status = PLANE_FLYING;
		return PLANE_STATUS_CHANGED;
	}
	else{
		return PLANE_TAKEOFF_ERROR;
	}
}

PlaneError_t	PlaneLand(Time_t time, Plane_t** pPlane){
	if(*(pPlane)->status != PLANE_ON_GROUND){
		*(pPlane)->ActualLandTime.hr = *(pPlane)->ActualTakeoffTime.hr + time.hr;
		*(pPlane)->ActualLandTime.min = *(pPlane)->ActualTakeoffTime.min + time.min;
		
		if(*(pPlane)->ActualLandTime.min >= 60) *(pPlane)->ActualLandTime.min = 0, *(pPlane)->ActualLandTime.hr++;
		
		*(pPlane)->ActualLandTime.sec = *(pPlane)->ActualTakeoffTime.sec + time.sec;
		if(*(pPlane)->ActualLandTime.sec >= 60)
			*(pPlane)->ActualLandTime.sec = 0, *(pPlane)->ActualLandTime.min++;
		
		*(pPlane)->status = PLANE_FLYING;
		return PLANE_STATUS_CHANGED;
	}
	else{
		return PLANE_LAND_ERROR;
	}
}

PlaneError_t 	WaitForRunWay(Plane_t p1, Plane_t* p2){
	if(p2->status == READY_TO_FLY){
		p2->TakeoffTime.hr += p1.RunWayTime.hr;
		p2->TakeoffTime.min += p1.RunWayTime.min;
		p2->TakeoffTime.sec += p1.RunWayTime.sec;
	}
	else{
		p2->LandTime.hr += p1.RunWayTime.hr;
		p2->LandTime.min += p1.RunWayTime.min;
		p2->LandTime.sec += p1.RunWayTime.sec;
	}
	return TIME_SET;
}

PlaneError_t	ChangePlaneStatus(PlaneStatus_t stat, Plane_t** pPlane){
	if((*pPlane)->status != stat){
		(*pPlane)->status = stat;
		return PLANE_STATUS_CHANGED;
	}
	else{
		return NO_NEED;
	}
}

static inline void	InitializeTime(Time_t* time){
	time->hr = 0;
	time->min = 0;
	time->sec = 0;
}

static Boolean IsSameTime(Time_t time, Time_t T){
	if(time.hr != T.hr || time.min != T.min || time.sec != T.sec)
		return FALSE;
	
	return TRUE;
}
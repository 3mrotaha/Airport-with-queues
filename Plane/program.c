#include <stdio.h>

#include "..\Time\time_ErrorStates.h"
#include "..\Time\time_config.h"
#include "..\Time\time_privilege.h"

#include "plane_ErrorStates.h"
#include "plane_config.h"
#include "plane_private.h"


static int planeID = 0;
static float planeSpeed = 70; // km/hr
static const int WayLength = 2500; // m

PlaneError_t	CreatePlane(Plane_t* pPlane, Flight_t flight){
	if(pPlane != NULL){
		pPlane->id = ++planeID;
		pPlane->status = PLANE_ON_GROUND;
		
		InitializeTime(&pPlane->Flight.TakeoffTime);
		InitializeTime(&pPlane->ActualTakeoffTime);
		InitializeTime(&pPlane->LandTime);
		pPlane->Flight = flight;
		pPlane->RunWayTime = CalculateRunWay(planeSpeed, WayLength);
		planeSpeed += 10.75;
		return PLANE_CREATED;
	}
	else{
		return PLANE_CREATION_ERROR;
	}
}

PlaneError_t	PlaneFly(Time_t time, Plane_t* pPlane){
	if(pPlane->status != PLANE_ON_AIR){
		pPlane->ActualTakeoffTime.hr = time.hr;
		pPlane->ActualTakeoffTime.min = time.min;
		pPlane->ActualTakeoffTime.sec = time.sec;
		pPlane->status = PLANE_FLYING;
		return PLANE_STATUS_CHANGED;
	}
	else{
		return PLANE_TAKEOFF_ERROR;
	}
}

PlaneError_t	PlaneLand(Plane_t* pPlane){
	if(pPlane->status != PLANE_ON_AIR){	
	
		SumTimes(pPlane->Flight.FlyDuration, pPlane->ActualTakeoffTime, &pPlane->LandTime);		
		pPlane->status = PLANE_FLYING;
		
		return PLANE_STATUS_CHANGED;
	}
	else{
		return PLANE_LAND_ERROR;
	}
}

PlaneError_t 	WaitForRunWay(Plane_t P_OnRunWay, Plane_t* P_Wait){
	if(P_Wait->status == READY_TO_FLY){
		SumTimes(P_Wait->Flight.TakeoffTime, P_OnRunWay.RunWayTime, &P_Wait->Flight.TakeoffTime);
	}
	else{
		SumTimes(P_Wait->LandTime, P_OnRunWay.RunWayTime, &P_Wait->LandTime);
	}
	return TIME_SET;
}

PlaneError_t	ChangePlaneStatus(PlaneStatus_t stat, Plane_t* pPlane){
	if(pPlane->status != stat){
		pPlane->status = stat;
		return PLANE_STATUS_CHANGED;
	}
	else{
		return NO_NEED;
	}
}

static Time_t CalculateRunWay(float speed, int distance){
	Time_t RunWayTime;
	/* speed (km/hr) = distance (km) / time (hr) */
	float time = (WayLength / 1000.0) / planeSpeed;
	RunWayTime.hr = (int) time;
	RunWayTime.min = (int)((time - RunWayTime.hr) * 60.0);
	RunWayTime.sec = (((time - RunWayTime.hr) * 60.0) - RunWayTime.min) * 60;
	return RunWayTime;
}
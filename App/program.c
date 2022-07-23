#include<stdio.h>

#include "..\Plane\plane_ErrorStates.h"
#include "..\Time\time_ErrorStates.h"
#include "..\Queue\Error_States.h"
#include "..\List\Error_States.h"
#include "app_ErrorStates.h"

#include "..\Time\time_config.h"
#include "..\Plane\plane_config.h"
#include "..\Queue\queue_config.h"
#include "..\List\list_config.h"
#include "configurations.h"

#include "..\Time\time_privilege.h"
#include "..\Plane\plane_privilege.h"
#include "..\Queue\queue_privilege.h"
#include "..\List\list_privilege.h"

#include "private.h"

static Queue_t Takeoff, Runway, Land;
extern Flight_t flights[MAX_FLIGHTS];

static int flight_index = 0;
static List_t OnAir;
static Time_t TempRunWayTime;
int RunwayTimeSum = 0;
static Plane_t Plane;


AppError_t	AppInit(void){
	CreateQueue(&Takeoff);
	CreateQueue(&Runway);
	CreateQueue(&Land);
	CreateList(&OnAir);
	return APP_INITIALIZED;
}

AppError_t	LaunchPlane(Time_t time){
	if(IsSameTime(flights[flight_index].TakeoffTime, time) == TIME_COMPARE_SAME){
		CreatePlane(&Plane, flights[flight_index % MAX_FLIGHTS]);
		flight_index++;
		ChangePlaneStatus(READY_TO_FLY, &Plane);
		Push(Plane, &Takeoff);
		return PLANE_LAUNCHED;
	}
	else{
		return NOW_FLIGHTS_NOW;
	}
}

AppError_t	CheckTakeoff(Time_t time){
	if(QueueEmpty(Takeoff) != QUEUE_EMPTY){
		if(QueueEmpty(Runway) == QUEUE_EMPTY){
			Pop(&Plane, &Takeoff);
			ChangePlaneStatus(PLANE_FLYING, &Plane);
			TempRunWayTime = Plane.RunWayTime;
			Push(Plane, &Runway);
			AirPortStatus(time);
			printf("Plane(%d) is On the Runway for %d:%d going to %s\n", Plane.id, Plane.RunWayTime.min, Plane.RunWayTime.sec, Plane.Flight.Destination);
			RunwayTimeSum = 0;
			return PLANE_ON_RUN_WAY;
		}
		else if(QueueEmpty(Runway) != QUEUE_EMPTY){
			Plane_t temp;
			QueueFront(&temp, Runway);
			QueueFront(&Plane, Takeoff);
			if(!RunwayTimeSum){
				WaitForRunWay(temp, &Plane);
				ChangePlaneStatus(PLANE_ON_GROUND_BUT_HAVE_TO_WAIT, &Plane);
				ChangeFront(Plane, &Takeoff);
				AirPortStatus(time);
				printf("Plane(%d) has to wait Plane(%d) to %s for %d:%d\n", Plane.id, temp.id, temp.status == PLANE_LANDING?"LAND":"FLY", temp.RunWayTime.min, temp.RunWayTime.sec);
				RunwayTimeSum = 1;
			}
		}
			return PLANE_TAKEOFF_DELAYED;
	}
}

AppError_t	CheckLanding(Time_t time){
	if(QueueEmpty(Land) != QUEUE_EMPTY){
		if(QueueEmpty(Runway) == QUEUE_EMPTY){
			Pop(&Plane, &Land);
			ChangePlaneStatus(PLANE_LANDING, &Plane);
			TempRunWayTime = Plane.RunWayTime;
			Push(Plane, &Runway);
			AirPortStatus(time);
			printf("Plane(%d) is On the Runway for coming from %s\n", Plane.id, Plane.Flight.Destination);
			RunwayTimeSum = 0;
			return PLANE_ON_RUN_WAY;
		}
		else if(QueueEmpty(Runway) != QUEUE_EMPTY){
			Plane_t temp;
			QueueFront(&temp, Runway);
			QueueFront(&Plane, Land);
			if(!RunwayTimeSum){
				WaitForRunWay(temp, &Plane);
				ChangePlaneStatus(PLANE_ON_AIR_BUT_HAVE_TO_WAIT, &Plane);
				ChangeFront(Plane, &Land);
				AirPortStatus(time);
				printf("Plane(%d) has to wait Plane(%d) to %s for %d:%d\n", Plane.id, temp.id, temp.status == PLANE_LANDING?"LAND":"FLY", temp.RunWayTime.min, temp.RunWayTime.sec);
				RunwayTimeSum = 1;
			}
			return PLANE_LAND_DELAYED;
		}
	}
}

AppError_t PushToAir(Time_t time){
	if(QueueEmpty(Runway) != QUEUE_EMPTY){
		Plane_t temp;
		Time_t t;
		InitializeTime(&t);
		QueueFront(&temp, Runway);
		if(IsSameTime(TempRunWayTime, t) == TIME_COMPARE_SAME && temp.status == PLANE_FLYING){
			Pop(&temp, &Runway);
			PlaneFly(time, &temp);
			PlaneLand(&temp);
			ChangePlaneStatus(PLANE_ON_AIR, &temp);
			ListInsert(0, temp, &OnAir);
			AirPortStatus(time);
			printf("plane(%d) is On Air Towards %s for %d:%d:%d, ", temp.id, temp.	Flight.Destination, temp.Flight.FlyDuration.hr, temp.Flight.FlyDuration.min, temp.Flight.FlyDuration.sec);
			printf("lands in %d:%d:%d\n", temp.LandTime.hr, temp.LandTime.min, temp.LandTime.sec);
			return PLANE_FLYED;
		}
	}
	return RUNWAY_CHECKED;
}

AppError_t PopFromAir(Time_t time){
	if(ListEmpty(OnAir) != LIST_EMPTY){
		int s;
		ListSize(&s, OnAir);
		for(int i = 0; i < s; i++){
			ListRetrieve(i, &Plane, OnAir);
			if(IsSameTime(Plane.LandTime, time) == TIME_COMPARE_SAME){
				ListDelete(i, &Plane, &OnAir);
				ChangePlaneStatus(READY_TO_LAND, &Plane);
				Push(Plane, &Land);
				AirPortStatus(time);
				printf("plane(%d) from %s Requests to land\n", Plane.id, Plane.Flight.Destination);
				return PLANE_REQUEST_LANDING;
			}
		}
	}
	return ON_AIR_CHECKED;
}

AppError_t PopFromRunWay(Time_t time){
	if(QueueEmpty(Runway) != QUEUE_EMPTY){
		Plane_t temp;
		QueueFront(&temp, Runway);
		Time_t t;
		InitializeTime(&t);
		if(IsSameTime(TempRunWayTime, t) == TIME_COMPARE_SAME && temp.status == PLANE_LANDING){
			Pop(&temp, &Runway);
			ChangePlaneStatus(PLANE_ON_GROUND, &temp);
			AirPortStatus(time);
			printf("plane(%d) From %s is On Ground\n", temp.id, temp.Flight.Destination);
			RunwayTimeSum = 0;
			return PLANE_LANDED;
		}
		DecrementTimeBy1sec(&TempRunWayTime);
	}
	return AIRPORT_CHECKED;
}

AppError_t DestroyApp(void){
	ClearQueue(&Takeoff);
	ClearQueue(&Land);
	ClearQueue(&Runway);
	DestroyList(&OnAir);
	return AIRPORT_CHECKED;
}


static AppError_t AirPortStatus(Time_t time){
	printf("%d:%d:%d > ", time.hr, time.min, time.sec);
	return AIRPORT_CHECKED;
}
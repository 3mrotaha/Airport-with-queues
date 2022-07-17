#include <stdio.h>
#include "Plane\plane_ErrorStates.h"
#include "Plane\Plane_config.h"
#include "Plane\Plane_privilege.h"

#include "Queue\Error_States.h"
#include "Queue\queue_config.h"
#include "Queue\queue_privilege.h"

int IsSameTime(Time_t time, Time_t T);

void displayID(Plane_t p){
	printf("%d		,", p.id);
}

void displayLandTime(Plane_t p){
	printf("%d:%d:%d	,", p.LandTime.hr, p.LandTime.min, p.LandTime.sec);
}
int main(void){
	
	
	Queue_t WaitToFly, WaitToLand, RunWay, OnAir;
	Plane_t Plane;
	Time_t TimeNewPlaneFly = {.hr = 0, .min = 0, .sec = 0}, NowTime, CurrentRunWayTime;
	int flagRunWay = 0;
	
	CreateQueue(&WaitToFly);
	CreateQueue(&WaitToLand);
	CreateQueue(&RunWay);
	CreateQueue(&OnAir);
	
	int hrs, mins, secs;	
	for(NowTime.hr = 0; NowTime.hr < 24; NowTime.hr++){
		for(NowTime.min = 0; NowTime.min < 60; NowTime.min++){
			for(NowTime.sec = 0; NowTime.sec < 60; NowTime.sec++){
				/* Asumming 1 second == 10^8 Iterations */
				for(long long loopsSec = 0; loopsSec <= 100000000; loopsSec++);
				//printf("%d\n", hrs);
				
				if(NowTime.hr == TimeNewPlaneFly.hr && NowTime.min == TimeNewPlaneFly.min && NowTime.sec == TimeNewPlaneFly.sec){
					Plane_t Pl;
					CreatePlane(&Pl);
					SetTakeOffTime(NowTime, &Pl);					

					if(TimeNewPlaneFly.sec >= 60){
						TimeNewPlaneFly.sec = 0;
						TimeNewPlaneFly.min++;
						if(TimeNewPlaneFly.min >= 60){
							TimeNewPlaneFly.min = 0;
							TimeNewPlaneFly.hr++;
						}
					}
						/* HERE */
					
					SetFlightTime(TimeNewPlaneFly, &Pl);	
					Plane_t* p = &Pl;
										
					ChangePlaneStatus(READY_TO_FLY, &p);
					Push(Pl, &WaitToFly);
				}
				if(QueueEmpty(RunWay) == QUEUE_EMPTY && QueueEmpty(WaitToLand) == QUEUE_EMPTY){
					if(QueueEmpty(WaitToFly) != QUEUE_EMPTY){											
						Plane_t* Pl, p;
						
						Pop(&p, &WaitToFly);
						Pl = &p;
						PlaneFly(NowTime, &Pl);
						
						Push(p, &RunWay);
						printf("%d:%d:%d : ", NowTime.hr, NowTime.min, NowTime.sec);
						printf("Plane(%d) on the Run Way to FLY for %d mins %d secs", Pl->id, Pl->RunWayTime.min, Pl->RunWayTime.sec);
						printf(" Back at %d:%d:%d\n", Pl->LandTime.hr, Pl->LandTime.min, Pl->LandTime.sec);
					}
					else{
						if(QueueEmpty(OnAir) != QUEUE_EMPTY){
							QueueTraverse(OnAir, &displayID);
							printf("\n");
							QueueTraverse(OnAir, &displayLandTime);	
						}
						
					}
					flagRunWay = 0;
				}
				
				if(QueueEmpty(RunWay) == QUEUE_EMPTY && QueueEmpty(WaitToLand) == QUEUE_NOT_EMPTY){
					Plane_t* Pl, p;
					Pop(&p, &WaitToLand);
					Pl = &p;
					PlaneLand(NowTime, &Pl);
					Push(p, &RunWay);
					printf("%d:%d:%d : ", NowTime.hr, NowTime.min, NowTime.sec);
					printf("Plane(%d) on the Run Way to LAND for %d mins %d secs\n", Pl->id, Pl->RunWayTime.min, Pl->RunWayTime.sec);
					if(QueueEmpty(WaitToFly) != QUEUE_EMPTY){
						Plane_t* p;
						QueueFront(p, WaitToFly);
						ChangePlaneStatus(PLANE_ON_GROUND_BUT_HAVE_TO_WAIT, &p);
						printf("%d:%d:%d : ", NowTime.hr, NowTime.min, NowTime.sec);
						printf("RunWay Busy! Plane(%d) has to wait Plane(%d) to LAND in %d:%d mins\n", p->id, Pl->id, Pl->RunWayTime.min, Pl->RunWayTime.sec);
					}
					flagRunWay = 0;
				}
				 
				if(QueueEmpty(RunWay) == QUEUE_NOT_EMPTY && QueueEmpty(WaitToLand) == QUEUE_NOT_EMPTY){
					Plane_t pFly, *pLand, pRunWay, p;
					pLand = &p;
					QueueFront(pLand, WaitToLand);
					ChangePlaneStatus(PLANE_ON_AIR_BUT_HAVE_TO_WAIT, &pLand);
					
					QueueFront(&pRunWay, RunWay);				
					WaitForRunWay(pRunWay, pLand);
					
					if(QueueEmpty(WaitToFly) != QUEUE_EMPTY){
						Plane_t* pl = &pFly;
						QueueFront(pl, WaitToFly);
						ChangePlaneStatus(PLANE_ON_GROUND_BUT_HAVE_TO_WAIT, &pl);
						printf("%d:%d:%d : ", NowTime.hr, NowTime.min, NowTime.sec);
						if(pRunWay.status == PLANE_FLYING)
							printf("RunWay Busy! Plane(%d) & Plane(%d) has to wait Plane(%d) to FLY in %d:%d mins\n", pFly.id, pLand->id, pRunWay.id, pRunWay.RunWayTime.min, pRunWay.RunWayTime.sec);
						else if(pRunWay.status == PLANE_LANDING)
							printf("RunWay Busy! Plane(%d) & Plane(%d) has to wait Plane(%d) to LAND in %d:%d mins\n", pFly.id, pLand->id, pRunWay.id, pRunWay.RunWayTime.min, pRunWay.RunWayTime.sec);
					}
					else{
						printf("%d:%d:%d : ", NowTime.hr, NowTime.min, NowTime.sec);
						if(pRunWay.status == PLANE_FLYING)
							printf("RunWay Busy! Plane(%d) has to wait Plane(%d) to FLY in %d:%d mins\n", pLand->id, pRunWay.id, pRunWay.RunWayTime.min, pRunWay.RunWayTime.sec);
						else if(pRunWay.status == PLANE_LANDING)
							printf("RunWay Busy! Plane(%d) has to wait Plane(%d) to LAND in %d:%d mins\n", pLand->id, pRunWay.id, pRunWay.RunWayTime.min, pRunWay.RunWayTime.sec);
					}
					if(!flagRunWay){
						CurrentRunWayTime.hr = pRunWay.RunWayTime.hr;
						CurrentRunWayTime.min = pRunWay.RunWayTime.min;
						CurrentRunWayTime.sec = pRunWay.RunWayTime.sec;
					}
					flagRunWay = 1;	
				}
				
				if(QueueEmpty(RunWay) == QUEUE_NOT_EMPTY && QueueEmpty(WaitToLand) == QUEUE_EMPTY){
					Plane_t pRunWay, pFly, *p;
					QueueFront(&pRunWay, RunWay);
		
					if(QueueEmpty(WaitToFly) != QUEUE_EMPTY){
						p = &pFly;
						QueueFront(p, WaitToFly);
						ChangePlaneStatus(PLANE_ON_GROUND_BUT_HAVE_TO_WAIT, &p);
						printf("%d:%d:%d : ", NowTime.hr, NowTime.min, NowTime.sec);
						if(pRunWay.status == PLANE_FLYING)
							printf("RunWay Busy! Plane(%d) has to wait Plane(%d) to FLY in %d:%d mins\n", pFly.id, pRunWay.id, pRunWay.RunWayTime.min, pRunWay.RunWayTime.sec);
						else if(pRunWay.status == PLANE_LANDING)
							printf("RunWay Busy! Plane(%d) has to wait Plane(%d) to LAND in %d:%d mins\n", pFly.id, pRunWay.id, pRunWay.RunWayTime.min, pRunWay.RunWayTime.sec);
					}
					else{
						printf("%d:%d:%d : ", NowTime.hr, NowTime.min, NowTime.sec);
						printf("RunWay Busy and No Waiting Processes\n");
					}
					if(!flagRunWay){
						CurrentRunWayTime.hr = pRunWay.RunWayTime.hr;
						CurrentRunWayTime.min = pRunWay.RunWayTime.min;
						CurrentRunWayTime.sec = pRunWay.RunWayTime.sec;
					}
					flagRunWay = 1;
				}
				
				if(CurrentRunWayTime.sec <= 0 && CurrentRunWayTime.min <= 0 && CurrentRunWayTime.hr <= 0 && QueueEmpty(RunWay) == QUEUE_NOT_EMPTY){
					Plane_t* p, Pl;
					p = &Pl;
					Pop(p, &RunWay);
					if(p->status == PLANE_FLYING){
						ChangePlaneStatus(PLANE_ON_AIR, &p);
						Push(Pl, &OnAir);						
						printf("Plane(%d) is On Air\n", p->id);
					}
					else{
						ChangePlaneStatus(PLANE_ON_GROUND, &p);
						printf("Plane(%d) Landed in the airport peacefully!\n");
					}
				}
				
				if(QueueEmpty(OnAir) != QUEUE_EMPTY){
					Plane_t* p, Pl;
					p = &Pl;
					QueueFront(p, OnAir);
					
					if(NowTime.hr == p->LandTime.hr && NowTime.min == p->LandTime.min && NowTime.sec == p->LandTime.sec){
						Pop(&Pl, &OnAir);
						ChangePlaneStatus(READY_TO_LAND, &p);
						Push(Pl, &WaitToLand);
						printf("Plane(%d) is Waiting Order To LAND\n", p->id);
					}
				}
				
				if(CurrentRunWayTime.sec-- < 0 && CurrentRunWayTime.min >= 0){
					CurrentRunWayTime.min--;
					if(CurrentRunWayTime.hr > 0 && CurrentRunWayTime.min <= 0){
						CurrentRunWayTime.min = 59;
						CurrentRunWayTime.hr--;
					}
					CurrentRunWayTime.sec = 59;
				}
			}
			
		}

	}
	ClearQueue(&RunWay);
	ClearQueue(&WaitToFly);
	ClearQueue(&WaitToLand);
	ClearQueue(&OnAir);
	return 0;
}



int IsSameTime(Time_t time, Time_t T){
	if(time.hr != T.hr && time.min != T.min && time.sec != T.sec)
		return 0;
	
	return 1;
}

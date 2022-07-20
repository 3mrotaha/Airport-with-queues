#ifndef	_PLANE_PRIVILEGE_
#define _PLANE_PRIVILEGE_


PlaneError_t	CreatePlane(Plane_t*, Flight_t);

PlaneError_t	SetTakeOffTime(Time_t, Plane_t*);

PlaneError_t	SetFlightTime(Time_t, Plane_t*);

PlaneError_t 	WaitForRunWay(Plane_t, Plane_t*);

PlaneError_t	PlaneFly(Time_t, Plane_t*);

PlaneError_t	PlaneLand(Plane_t*);

PlaneError_t	ChangePlaneStatus(PlaneStatus_t, Plane_t*);

#endif
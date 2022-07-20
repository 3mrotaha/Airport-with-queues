#ifndef	_APP_PRIVILEGE_
#define _APP_PRIVILEGE_

AppError_t	AppInit(void);

AppError_t	LaunchPlane(Time_t);

AppError_t	CheckLanding(Time_t);

AppError_t	CheckTakeoff(Time_t);

#endif

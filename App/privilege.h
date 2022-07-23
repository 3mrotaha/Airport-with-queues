#ifndef	_APP_PRIVILEGE_
#define _APP_PRIVILEGE_

AppError_t	AppInit(void);

AppError_t	LaunchPlane(Time_t);

AppError_t	CheckLanding(Time_t);

AppError_t	CheckTakeoff(Time_t);

AppError_t PushToAir(Time_t);

AppError_t PopFromAir(Time_t);

AppError_t PopFromRunWay(Time_t);

AppError_t DestroyApp(void);
#endif

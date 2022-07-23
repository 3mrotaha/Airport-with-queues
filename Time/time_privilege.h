#ifndef _TIME_PRIVILEGE_
#define _TIME_PRIVILEGE_

TimeError_t InitializeTime(Time_t*);

TimeError_t IsSameTime(Time_t, Time_t);

TimeError_t SumTimes(Time_t, Time_t, Time_t*);

TimeError_t DecrementTimeBy1sec(Time_t*);
#endif
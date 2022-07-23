#include <stdio.h>
#include "time_ErrorStates.h"
#include "time_config.h"
#include "time_private.h"


TimeError_t InitializeTime(Time_t* time){
	if(time != NULL){
		time->hr = 0;
		time->min = 0;
		time->sec = 0;
		return TIME_DEFINED;
	}
	else{
		return TIME_IS_NOT_DEFINED;
	}
}

TimeError_t	IsSameTime(Time_t time, Time_t T){
	if(IsSame(time, T) == TRUE){
		return TIME_COMPARE_SAME;
	}
	else{
		return TIME_COMPARE_DIFFERERNT;
	}
}

TimeError_t SumTimes(Time_t t1, Time_t t2, Time_t* t){
	if(t != NULL){
		t->sec = (t1.sec + t2.sec) % 60;
		
		if((t1.sec + t2.sec) >= 60){
			t->min = (t1.min + t2.min + 1) % 60;	
			if((t1.min + t2.min + 1) >= 60)
				t->hr = (t1.hr + t2.hr + 1) % 24;
			else
				t->hr = (t1.hr + t2.hr) % 24;
		}
		else{
			t->min = (t1.min + t2.min) % 60;
			if((t1.min + t2.min) >= 60)
				t->hr = (t1.hr + t2.hr + 1) % 24;
			else
				t->hr = (t1.hr + t2.hr) % 24;			
		}
			
		return TIME_DEFINED;
	}
	else{
		return TIME_IS_NOT_DEFINED;
	}
}

TimeError_t DecrementTimeBy1sec(Time_t* t){
	if(t != NULL){
		t->sec--;
		if(t->sec <= 0){
			t->min--;
			if(t->min < 0){
				t->hr--;
				if(t->hr < 0){
					t->hr = 0;
					t->min = 0;
					t->sec = 0;
				}
				else{
					t->sec = 59;
					t->min = 59;
				}
			}
			else{
				t->sec = 59;
			}
		}
		return TIME_DEFINED;
	}
	else{
		return TIME_IS_NOT_DEFINED;
	}
}

static Boolean IsSame(Time_t time, Time_t T){
	if(time.hr == T.hr && time.min == T.min && time.sec == T.sec)
		return TRUE;
	return FALSE;
}
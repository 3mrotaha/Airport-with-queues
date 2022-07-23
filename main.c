#include <stdio.h>

#include "Time\time_config.h"

#include "App\app_ErrorStates.h"
#include "App\configurations.h"
#include "App\privilege.h"


int main(void){
	
	Time_t NowTime;
	int i = 0;
	AppInit();
	for(NowTime.hr = 0; NowTime.hr < 24; NowTime.hr++){
		for(NowTime.min = 0; NowTime.min < 60; NowTime.min++){
			for(NowTime.sec = 0; NowTime.sec < 60; NowTime.sec++){
				/* Asumming 1 second == 10^8 Iterations */
				for(long long loopsSec = 0; loopsSec <= 1000000; loopsSec++);				
				
				LaunchPlane(NowTime);			
				CheckTakeoff(NowTime);
				PushToAir(NowTime);
				CheckLanding(NowTime);
				PopFromAir(NowTime);
				PopFromRunWay(NowTime);
			}
		}

	}
	DestroyApp();
	return 0;
}



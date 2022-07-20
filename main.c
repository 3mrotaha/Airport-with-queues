#include <stdio.h>
#include "Plane\plane_ErrorStates.h"
#include "Plane\Plane_config.h"
#include "Plane\Plane_privilege.h"

#include "Queue\Error_States.h"
#include "Queue\queue_config.h"
#include "Queue\queue_privilege.h"

#include "List\Error_States.h"
#include "List\list_config.h"
#include "List\list_privilege.h"


int main(void){
	
	
	int i = 0;
	for(NowTime.hr = 0; NowTime.hr < 24; NowTime.hr++){
		for(NowTime.min = 0; NowTime.min < 60; NowTime.min++){
			for(NowTime.sec = 0; NowTime.sec < 60; NowTime.sec++){
				/* Asumming 1 second == 10^8 Iterations */
				for(long long loopsSec = 0; loopsSec <= 1000000; loopsSec++);				
				
				
				// time to create new plane
				
			}
		}

	}
	return 0;
}



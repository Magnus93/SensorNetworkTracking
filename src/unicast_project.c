/*

	PURPOSE : 75 sends, 63 receives the signal strength

*/
#include "settings.h"
#include "distance.h"
#include "sink.h"
#include "moron.h"


PROCESS(unicast_process, "unicast");
AUTOSTART_PROCESSES(&unicast_process);

PROCESS_THREAD(unicast_process, ev, data)
{
	PROCESS_BEGIN();
	
	init_mote();


	switch (mote) {
		case Sink: 
			printf("I am the sink!\n");
			process_start(&sink_process, NULL);
			break;
		default :
			printf("I am not a sink!\n");
	}

	PROCESS_END();
}

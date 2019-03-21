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
		case Origin:
			printf("I am the Origin\n");
			process_start(&moron_process, NULL);
			break;
		case Yaxis:
			process_start(&moron_process, NULL);
			printf("I am the Yaxis\n");
			break;
		case Xaxis:
			process_start(&moron_process, NULL);
			printf("I am the Xaxis\n");
			break;
		case Sink: 
			printf("I am the Sink!\n");
			process_start(&sink_process, NULL);
			break;
		default :
			printf("I am nothing, so now I die!\n");
	}

	PROCESS_END();
}

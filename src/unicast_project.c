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

	process_start(&sink_process, NULL);

	PROCESS_END();
}

/*

	PURPOSE : 75 sends, 63 receives the signal strength

*/


#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include "dev/cc2420/cc2420.h"

#define TX_POWER 10
#define OFFSET 10		// the RSSI power indication when placing two motes on top of eachother (Cooja)
// The actual received + the offset. If at same location == 0
#define RECEIVED_RSSI packetbuf_attr(PACKETBUF_ATTR_RSSI) + 10    //PACKETBUF_ATTR_RSSI is unsigned integer
#define RSSI_AMOUNT 10	// the number of RSSI readings to store in a buffer for the moving average

PROCESS(unicast_process, "unicast");
AUTOSTART_PROCESSES(&unicast_process);

static int rssi_readings[RSSI_AMOUNT];		// store the last readings to calculate a moving average (is EWA better?)
static int index = 0;			// keeps track of the next location to store data in from rssi_readings
int avg_rssi = 0;
int accumulator = 0;

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
	printf("unicast message received from %d.%d\n",from->u8[0], from->u8[1]);
	rssi_readings[index % RSSI_AMOUNT] = RECEIVED_RSSI;		// store the latest reading at the oldest value
	index++;
	if (index >= RSSI_AMOUNT) {
		accumulator = 0;

		int i;
		for (i = 0; i < RSSI_AMOUNT; ++i)
		{
			accumulator += rssi_readings[i];
		}
		avg_rssi = accumulator / RSSI_AMOUNT;
	}
	printf("accumulator: %d\n",  accumulator);;
	printf("index: %d\n", index);
	printf("avg_rssi: %d \n", avg_rssi); 
}

static void sent_uc(struct unicast_conn *c, int status, int num_tx) {
	const linkaddr_t *dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
	if(linkaddr_cmp(dest, &linkaddr_null)) {
		return;
	}
	printf("unicast message sent to %d.%d: status %d num_tx %d power (0-31) %d\n", 
		dest->u8[0], dest->u8[1], status, num_tx, TX_POWER);
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc, sent_uc};
static struct unicast_conn uc;

PROCESS_THREAD(unicast_process, ev, data)
{
	PROCESS_EXITHANDLER(unicast_close(&uc);)
	
	PROCESS_BEGIN();
	
	unicast_open(&uc, 146, &unicast_callbacks);
	cc2420_set_txpower(TX_POWER);
	while(1) {
		static struct etimer et;
		linkaddr_t addr;
		
		etimer_set(&et, CLOCK_SECOND);
		
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		
		packetbuf_copyfrom("Hello", 5);
		addr.u8[0] = 1;
		addr.u8[1] = 0;
		if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
			unicast_send(&uc, &addr);
			printf("My addr : %d.%d \n",linkaddr_node_addr.u8[0],linkaddr_node_addr.u8[1]);
		}	
	}
	
	PROCESS_END();
}

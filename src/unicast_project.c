/*

	PURPOSE : 75 sends, 63 receives the signal strength

*/


#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include "dev/cc2420/cc2420.h"

#define TX_POWER 31
#define OFFSET 10		// the RSSI power indication when placing two motes on top of eachother (Cooja) 
#define RSSI_AMOUNT 10	// the number of RSSI readings to store in a buffer for the moving average

PROCESS(unicast_process, "unicast");
AUTOSTART_PROCESSES(&unicast_process);

static int rssi_readings[RSSI_AMOUNT];		// store the last readings to calculate a moving average (is EWA better?)
static int index = 0;			// keeps track of the next location to store data in from rssi_readings
int avg_rssi = 0;
int accumulator = 0;

void store_RSSI_value(int rssi_value);
void calculate_RSSI_average();

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
	printf("unicast message received from %d.%d\n",from->u8[0], from->u8[1]);
	int received_rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);

	store_RSSI_value(received_rssi);
	calculate_RSSI_average();
	
}

static void sent_uc(struct unicast_conn *c, int status, int num_tx) {
	const linkaddr_t *dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
	if(linkaddr_cmp(dest, &linkaddr_null)) {
		return;
	}
	printf("unicast message sent to %d.%d: status %d num_tx %d power (0-31) %d\n", 
		dest->u8[0], dest->u8[1], status, num_tx, TX_POWER);
}

void store_RSSI_value(int rssi_value) {
	printf("index: %d\n", index);
	rssi_readings[index % RSSI_AMOUNT] = rssi_value + OFFSET;		// store the latest reading at the oldest value
	index++;
	if (index >= 100 * RSSI_AMOUNT) {index = RSSI_AMOUNT;}  // to not overflow, caution! dont set RSSI_AMOUNT to high!
}

// Calculates the average RSSI value of the RSSI_AMOUNT latest packets
void calculate_RSSI_average() {
	accumulator = 0;
	if (index >= RSSI_AMOUNT) {
		int i;
		for (i = 0; i < RSSI_AMOUNT; ++i)
		{
			accumulator += rssi_readings[i];
		}
		avg_rssi = accumulator / RSSI_AMOUNT;
	} else {
		avg_rssi = 0;  // if we dont have enough readings for the moving average set the average to 0
	}
	printf("avg_rssi: %d \n", avg_rssi); 
	printf("accumulator: %d\n",  accumulator);;
}

uint8_t calculate_distance(int rssi_value) {
	return 1;
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

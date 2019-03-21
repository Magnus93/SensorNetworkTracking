#include "settings.h"
#include "distance.h"

PROCESS(moron_process, "moron");


static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
	printf("unicast message received from %d.%d\n",from->u8[0], from->u8[1]);
	int received_rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);

	store_RSSI_value(received_rssi);
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

PROCESS_THREAD(moron_process, ev, data)
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
		addr.u8[0] = Sink;
		addr.u8[1] = 0;
		if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
			unicast_send(&uc, &addr);
		}	
		printf("My addr : %d.%d \n",linkaddr_node_addr.u8[0],linkaddr_node_addr.u8[1]);
		calculate_RSSI_average();	
		calculate_distance();
	}
	
	PROCESS_END();
}
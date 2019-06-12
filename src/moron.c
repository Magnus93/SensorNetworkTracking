#include "settings.h"
#include "distance.h"



PROCESS(moron_process, "moron");

Mote_t mote;
uint32_t packet_data[2];	// [command, distance in cm] ie [REPLY_DISTANCE, 1992]

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
	
	uint32_t *received_data = packetbuf_dataptr();
	int received_rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);

	packet_data[COMMAND] = (received_data[COMMAND]);
	packet_data[DISTANCE] = (received_data[DISTANCE]);
	printf("unicast message received from %d.%d\n",from->u8[0], from->u8[1]);
	printf("with data[COMMAND]: %ld and data[DISTANCE]: %ld\n", packet_data[COMMAND], packet_data[DISTANCE]);

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

	mote = get_type();

	unicast_open(&uc, 146, &unicast_callbacks);
	cc2420_set_txpower(TX_POWER);
	
	while(1) {
		static struct etimer et;
		linkaddr_t addr;
		
		etimer_set(&et, CLOCK_SECOND/2);
		
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		addr.u8[0] = Sink;
		addr.u8[1] = 0;
		
		if (packet_data[COMMAND] == REQUEST_DISTANCE) {
			printf("Someone requested the distance\n");
			// only transmitting to the sink for now! gather axis later
			packet_data[COMMAND] = REPLY_DISTANCE;
			packet_data[DISTANCE] = calculate_distance();
			packetbuf_copyfrom(&packet_data, sizeof(uint32_t) * 2);
			unicast_send(&uc, &addr);
			packet_data[COMMAND] = WAIT_FOR_COMMAND;
		}
		else if (packet_data[COMMAND] == REQUEST_AXIS) {
			// make Origo request the axis
			addr.u8[0] = Yaxis;
			addr.u8[1] = 0;
			// Handle Yaxis distance here

			addr.u8[0] = Xaxis;
			addr.u8[1] = 0;
			// Handle Xaxis distance here


			addr.u8[0] = Xaxis;
			addr.u8[1] = 0;



		}
		//uint32_t sent_message = get_type();
		//packetbuf_copyfrom(&sent_message, 1);

		//if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
		//	unicast_send(&uc, &addr);
		//}	


		printf("My addr : %d.%d \n",linkaddr_node_addr.u8[0],linkaddr_node_addr.u8[1]);
		
	}
	
	PROCESS_END();
}
/*

	PURPOSE : 75 sends, 63 receives the signal strength

*/
#include "settings.h"
#include "distance.h"

typedef enum Sink_States
{	
	ZreqAxis,
	Waiting, 
	SetAxis,
	ZreqOrigoDist, 
	ZreqXDist, 
	ZreqYDist,
	ZcalcPos,
	ZdisplayPos
} Sink_States_t; 

Sink_States_t state = ZreqAxis;
uint8_t received_axis = 0;
uint32_t packet_data[2];
static Distances_t distances;
static Position_t pos;
PROCESS(sink_process, "sink");


static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
	printf("unicast message received from %d.%d\n",from->u8[0], from->u8[1]);
	uint8_t sender = from->u8[0];
	
	uint32_t *received_msg;
	int received_rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
	
	received_msg = (uint32_t*)packetbuf_dataptr();
	
	switch(sender) {
		case Origin:
			if (received_msg[COMMAND] == REPLY_AXIS) {
				printf("Received length of x and y axis from Origin\n");
				// we need to do some magic stuff here to get both values from Origin
			}
			if (received_msg[COMMAND] == REPLY_DISTANCE) {
				printf("Received the distance to Origin: %ld\n", received_msg[DISTANCE]);
				distances.origin = received_msg[DISTANCE];
				// store the distance value in a proper location 
			}
			break;
		case Yaxis:
			if (received_msg[COMMAND] == REPLY_DISTANCE) {
				printf("Received the distance to Yaxis: %ld\n", received_msg[DISTANCE]);
				distances.yaxis = received_msg[DISTANCE];
				// store the distance value in a proper location 
			}
			break;
		case Xaxis:
			if (received_msg[COMMAND] == REPLY_DISTANCE) {
				printf("Received the distance to Xaxis: %ld\n", received_msg[DISTANCE]);
				distances.xaxis = received_msg[DISTANCE];
				// store the distance value in a proper location 
			}
			break;
		default:
			printf("We got something from someone that should not exist!!\n");
	}

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


PROCESS_THREAD(sink_process, ev, data)
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

		switch(state) {
			case ZreqAxis:
				printf("Requesting Axis State\n");
				packet_data[COMMAND] = REQUEST_AXIS;
				packet_data[DISTANCE] = 0;
				packetbuf_copyfrom(packet_data, 2);
				addr.u8[0] = Origin;
				addr.u8[1] = 0;
				unicast_send(&uc, &addr);
				state = Waiting;
				break;
			case Waiting:
				printf("Waiting State\n");
				// read x and y values from Origin
				received_axis = 1;  // remove this when implemented!!!
				if (received_axis) {
					state = SetAxis;
				}
				break;
			case SetAxis:
				printf("Setting Length of Axis State\n");
				// use true distance when x and y is received
				set_axis(1000 , 1000);
				state = ZreqOrigoDist;
			case ZreqOrigoDist:
				printf("Requesting Origo Distance State\n");
				packet_data[COMMAND] = REQUEST_DISTANCE;
				packet_data[DISTANCE] = 0;
				packetbuf_copyfrom(packet_data, 2);
				addr.u8[0] = Origin;
				addr.u8[1] = 0;
				unicast_send(&uc, &addr);
				state = ZreqXDist;
				break;
			case ZreqXDist:
				printf("Requesting X Distance State\n");
				packet_data[COMMAND] = REQUEST_DISTANCE;
				packet_data[DISTANCE] = 0;
				packetbuf_copyfrom(packet_data, 2);
				addr.u8[0] = Xaxis;
				addr.u8[1] = 0;
				unicast_send(&uc, &addr);
				state = ZreqYDist;
				break;
			case ZreqYDist:
				printf("Requesting Y Distance State\n");
				packet_data[COMMAND] = REQUEST_DISTANCE;
				packet_data[DISTANCE] = 0;
				packetbuf_copyfrom(packet_data, 2);
				addr.u8[0] = Yaxis;
				addr.u8[1] = 0;
				unicast_send(&uc, &addr);
				state = ZcalcPos;
				break;
			case ZcalcPos:
				printf("Calculating Distance State\n");
				calculate_distance(get_type());
				pos = calculate_position(distances);
				state = ZdisplayPos;
				break;
			case ZdisplayPos:
				printf("Displaying Position State\n");
				state = ZreqOrigoDist;
				break;
			default:
				printf("We fucked up by entering a none existing state!  ¯\\_(ツ)_/¯ \n");
		}
		
		packetbuf_copyfrom("Hello", 5);
		addr.u8[0] = Sink;
		addr.u8[1] = 0;
		if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
			unicast_send(&uc, &addr);
		}	
		printf("My addr : %d.%d \n",linkaddr_node_addr.u8[0],linkaddr_node_addr.u8[1]);
	}
	
	PROCESS_END();
}

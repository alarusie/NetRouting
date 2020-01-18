#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../include/network.h"
#include "../include/wire.h"

struct _network *rand_init_network(int size);
int              rand_init_links(struct _network *network);
int              write_to_file(struct _network *network, char *filename);
struct _network *read_from_file(char *filename);
int              generate_routes_tables(struct _network *network);
int              send_packet(struct _wire *wire, struct _node *source, struct _node *destination, int t);
int              process_packets_on_wire(struct _network *network, struct _wire *wire, int t);
int              process_packets_on_queues(struct _network *network, int t);
int              destroy_subwire(struct _wire *wire);
void             print_statistics();

#define NUM_SOURCES     20 
#define REGULAR_NODE    0x00
#define SOURCE_NODE     0x01
int packet_ID = 0;
struct _statistics {
    int generated[MAX_NUM_NODES];
    int succeed[MAX_NUM_NODES];
    int dropped[MAX_NUM_NODES];
} stats;

int send_packet(struct _wire *wire, struct _node *source, struct _node *destination, int t) {
    struct _packet *packet;
    struct _link   *next_hop;

    assert( wire != NULL );
    assert( source != NULL );
    assert( destination != NULL );

    packet = create_packet(packet_ID, source, destination);
    update_packet_t(packet, t);
    set_packet_status(packet, ON_WIRE);
    assign_packet_hop(packet, source);
    place_wire_packet(wire, packet);

    return 0;
}

int process_packets_on_wire(struct _network *network, struct _wire *wire, int t) {
    struct _packet      *packet;
    struct _node        *node;
    struct _link        *link;
    struct _queue       *input_q;
    int                  i;

    assert( wire != NULL );
    assert( network != NULL );
    
    for (i = 0; i < get_network_size(network); i++) {
        node = get_network_node(network, i);

        packet = get_wire_first_packet(wire);
        do {
            printf("packet 0x%p on wire with status 0x%x \n", 
                    packet, packet->status);
            if (is_same_node(node, get_packet_hop(packet))) {
                /* this node should act as router for this packet
                 * so, place the packet on input_q
                 */
                link = lookup_node_next_hop(node, packet->destination);
                input_q = lookup_node_input_q(node, link);
                if (input_q == NULL || enqueue(input_q, packet) < 0) {
                    /* node's input_q is full. drop the packet */
                    stats.dropped[i]++;
                    set_packet_status(packet, DROP);
                } else {
                    set_packet_status(packet, ON_INPUT_Q);
                } 
            }
        } while ((packet = get_wire_next_packet(wire)));
    } // end for
}

int process_packets_on_queues(struct _network *network, int t) {
    struct _packet      *packet;
    struct _node        *node;
    struct _link        *link;
    struct _link        *next_hop;
    struct _queue       *input_q;
    struct _queue       *output_q;
    int                  i;
    int                  j;

    assert( network != NULL );
    
    for (i = 0; i < get_network_size(network); i++) {
        node = get_network_node(network, i);

        /* start with processing input queues of this node */
        for (j = 0; j < get_node_nlinks(node); j++) {
            link = get_node_link(node, j);
            input_q = get_node_input_q(node, j);
            if (link == NULL || input_q == NULL) {
                continue;
            }
            while (!is_queue_empty(input_q)) {
                packet = dequeue(input_q);
                if (is_same_node(node, get_packet_destination(packet))) {
                    /* this node is a destination itself */
                    set_packet_status(packet, DELIVERED);
                    stats.succeed[i]++;
                } else {
                    /* move the packet to output queue after adding total
                     * delay time
                     */
                    update_packet_t(packet, t + (int) link->total_delay);
                    if (enqueue(input_q, packet) < 0) {
                        /* node's output_q is full. drop the packet */
                        set_packet_status(packet, DROP);
                        stats.dropped[i]++;
                    } else {
                        set_packet_status(packet, ON_OUTPUT_Q);
                    }
                }
            }
        }

        /* start with processing output queues of this node */
        for (j = 0; j < get_node_nlinks(node); j++) {
            link = get_node_link(node, j);
            output_q = get_node_output_q(node, j);
            if (link == NULL || output_q == NULL) {
                continue;
            }
            while (!is_queue_empty(input_q)) {
                packet = front(output_q);
                if (get_packet_t(packet) != t) {
                    /* this is not time to process the packet 
                     * skip this queue
                     */
                    break;
                }
                /* now it is time to dequeue packet and process it */
                packet = dequeue(input_q);
                set_packet_status(packet, ON_WIRE);
                next_hop = lookup_node_next_hop(node, packet->destination);
                assign_packet_hop(packet, get_link_node2(next_hop));
            }
        }
    }
    return 0;
}

int simulate_network(struct _network *network) {
    struct _node    *sources[NUM_SOURCES];
    struct _node    *node;
    struct _node    *dest;
    struct _packet  *packet;
    struct _link    *next_hop;
    struct _wire    *wire;
    struct _wire    *subwire;

    int              i;
    int              index;
    int              idx;
    int              t;
    float            u;
    int              tgd; // time to generate data

    /* randomly pick up 20 sources */
    for (i = 0; i < get_network_size(network); i++) {
        node = get_network_node(network, i);
        set_node_status(node, REGULAR_NODE);
    }
    i = 0;
    while (i < NUM_SOURCES) {
        index = rand() % get_network_size(network);
        node = get_network_node(network, index);
        if (is_node_status(node, REGULAR_NODE)) {
            set_node_status(node, SOURCE_NODE);
            sources[i] = node;
            i++;
        }
    }

    wire = create_wire();
    memset(&stats, 0, sizeof(struct _statistics));
    t = 0;
    u = (float) rand() / RAND_MAX;
    tgd = (int) ((-1 / 0.5) * log (u));
    while (t < 100) {
        if ((t % tgd) == 0) {
            /* go ahead and randomly pick up destination.
             * generate packets to sent from source to 
             * these destinations
             */
            i = 0;
            while (i < NUM_SOURCES) {
                index = rand() % get_network_size(network);
                dest = get_network_node(network, index);
                if ((is_node_status(dest, REGULAR_NODE)) && 
                    (!is_same_node(sources[i], dest))) 
                {   
                    send_packet(wire, sources[i], dest, t);
                    idx = get_node_index(sources[i]);
                    stats.generated[idx]++;
                    i++;
                }
            }
        } 
        /* collect packets on wire */
        subwire = extract_wire_packets_by_status(wire, ON_WIRE);
        packet = get_wire_first_packet(subwire);
        if (packet) {
            printf("at least one packet for t %d\n", t);
            /* if there was at least one packet, process it */
            process_packets_on_wire(network, subwire, t);
        }
        else {
            printf("subwire is empty for t %d\n", t);
        }
        destroy_subwire(subwire);
        process_packets_on_queues(network, t);
        t++;   
    }
    return 0;
}

int destroy_subwire(struct _wire *wire) {
    struct _llpacket *curr;

    assert( wire != NULL );
    curr = wire->head;
    while (curr) {
        wire->head = wire->head->next;
        free(curr);
        curr = wire->head;
    }
    free(wire);

    return 0;
}
struct _network *rand_init_network(int size) {
    int              i;
    struct _network *network;
    struct _node    *new_node;
    
    network = create_network(size);
    for (i = 0; i < get_network_size(network); i++) {
        new_node = create_node(i);
        set_network_node_at(network, i, new_node);
    }
	rand_init_links(network);

    return network;
}

int rand_init_links(struct _network *network) {
	int 	         i;
	int 	         j;
	int	             nlinks;
    int              ntries;
	struct _node    *node1;
	struct _node    *node2;

	for (i = 0; i < get_network_size(network); i++) {
		node1 = get_network_node(network, i);
		nlinks = (rand() % MAX_NUM_LINKS) + 1;
        ntries = 0;
		while (get_node_nlinks(node1) < nlinks) {
			j = rand() % get_network_size(network);
            node2 = get_network_node(network, j);
            if (create_link(node1, node2) == 0) {
                // link was successfully created
                ntries = 0;
            }
            if (++ntries > 10) {
                // give up on creating links for this node
                break;
            }
		}
	}
    return 0;
}

int generate_routes_tables(struct _network *network) {
	struct _node    *node;
	struct _node    *node2;
    struct _link    *link;
    struct _route   *route;
    struct _route   *minroute;
    unsigned int     mintcost;
	struct _node    *dest;
	unsigned int     tcost;
    float            total_delay;
    int              n;
	int 	         i;
	int	             j;
	int	             index;

	for (i = 0; i < get_network_size(network); i++) {
		node = get_network_node(network, i);
        allocate_node_routes(node, get_network_size(network));
	    for (j = 0; j < get_node_nroutes(node); j++) {
            route = get_node_route(node, j);
            dest = get_network_node(network, j);
            init_route(route, dest);

		}
        route = get_node_route(node, i);
        set_route_total_cost(route, 0.0);
		set_node_status(node, INPR);
		n = 0;
		while (n < get_network_size(network)) {
            minroute = NULL;
            mintcost = INFT;
			for (j = 0; j < get_node_nroutes(node); j++) {
                route = get_node_route(node, j);
				dest  = get_route_destination(route);
				tcost = get_route_total_cost(route);
				if (is_node_status(dest, INPR )&& tcost < mintcost) {
                    minroute = route;
                    mintcost = tcost;
				}
			}
            dest = get_route_destination(minroute);
			for (j = 0; j < get_node_nlinks(dest); j++) {
                link   = get_node_link(dest, j);
				total_delay = get_link_total_delay(link);
                node2  = get_link_node2(link);
                index  = get_node_index(node2);
                route  = get_node_route(node, index);
				
                if (get_route_total_cost(route) > 
                    get_route_total_cost(minroute) + total_delay) {
					set_route_destination(route, node2);
                    if (n == 0) {
                        // special case for first node
                        set_route_next_hop(route, link);
                    }
                    else {
                        set_route_next_hop(route, get_route_next_hop( minroute));
                    }
					set_route_total_cost(route, get_route_total_cost(minroute) + total_delay);
				}

				if (is_node_status(node2, NONE)) {
					set_node_status(node2, INPR);
                }
			}
			set_node_status(dest, DONE);
			n++;
		}
	}
    return 0;
}

int write_to_file(struct _network *network, char *filename) {
    int              i;
    int              j;
    int              k;
    int              found;
    int              nlinks;
    FILE            *f;
    struct _link    *links;
    struct _link    *link;
    struct _node    *node;
    struct _node    *node2;

    nlinks = 0;
    for (i = 0; i < get_network_size(network); i++) {
        node = get_network_node(network, i);
        nlinks += get_node_nlinks(node);
    }
    nlinks = (nlinks / 2) + 1;
    links = (struct _link *) malloc(nlinks  * sizeof(struct _link));
    memset(links, 0, (nlinks  * sizeof(struct _link)));

    nlinks = 0;
    for (i = 0; i < get_network_size(network); i++) {
        node = get_network_node(network, i);
        for (j = 0; j < get_node_nlinks(node); j++) {
            /* add this link to links[] IFF not already there */
            found = 0;
            link = get_node_link(node, j);
            for (k = 0; k < nlinks; k++) {
                if (is_same_link(link, &links[k])) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                links[nlinks] = *link;
                nlinks++;
            }
        }
    }

    f = fopen(filename, "w");
    if (f == NULL) {
        free(links);
        return -1;
    }

    fprintf(f,"%d %d\n", get_network_size(network), nlinks);
    for (i = 0; i < nlinks; i++) {
        node = get_link_node1(&links[i]);
        node2 = get_link_node2(&links[i]);
        fprintf(f,"%d %d\n", get_node_index(node), get_node_index(node2));
    }

    fclose(f);
    free(links);

    return 0;
}

struct _network *read_from_file(char *filename) {
    int              i;
    int              nlinks;
    int              size;
    FILE            *f;
    struct _node    *node;
    struct _node    *node1;
    struct _node    *node2;
    struct _network *network;
    int              index1;
    int              index2;

    nlinks = 0;
    size = 0;

    f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }

    if (fscanf(f, "%d %d", &size, &nlinks) != 2) {
        return NULL;
    }
    if ((size < 2) || (nlinks < 1)) {
        return NULL;
    }

    network = create_network(size);
    for (i = 0; i < size; i++) {
        node = create_node(i);
        set_network_node_at(network, i, node);
    }

    while (!feof(f)) {
        node1 = NULL;
        node2 = NULL;
        index1 = -1;
        index2 = -1;

        if (fscanf(f, "%d %d", &index1, &index2) != 2) {
            break;
        }
        for (i = 0; i < size; i++) {
            node = get_network_node(network, i);
            if (get_node_index(node) == index1) {
                node1 = node;
            }
            if (get_node_index(node) == index2) {
                node2 = node;
            }
            if ((node1) && (node2)) {
                create_link(node1, node2);
                break;
            }
        }
    }

    fclose(f);

    return network;
}

void print_statistics() {
    int generated;
    int succeed;
    int dropped;
    int i;

    generated = 0;
    succeed = 0;
    dropped = 0;

    for (i = 0; i < MAX_NUM_NODES; i++) {
        generated += stats.generated[i];
        succeed += stats.succeed[i];
        dropped += stats.dropped[i];
    }

    printf("\n ----- Statistics ----- \n");
    printf("\tnum of generated packets: %d \n", generated);
    printf("\tnum of succeed packets: %d \n", succeed);
    printf("\tpercentage of received packets: %d \n", ((float) succeed) / ((float) generated) * 100);
    printf("\tnum of dropped packets: %d \n", dropped);
}


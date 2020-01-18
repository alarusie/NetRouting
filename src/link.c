#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "../include/link.h"

int create_link(struct _node *node1, struct _node *node2) {    
    float            u;
    float            td;
    float            b;
    float            d;
    int              i;
    struct _link    *link1;
    struct _link    *link2;
    int              nlinks1;
    int              nlinks2;
    struct _node    *node;

#define bMIN    0
#define bMAX    1       /* 1mbps */
#define dMIN    0
#define dMAX    10      /* sec */
#define PACKET_SIZE 1.0
    /* 
     * make sure not to link a node with its self
     */
	if (is_same_node(node1, node2)) {
		return -1;
    }
    /*
     * make sure not to create links > MAX_NUM_LINKS
     */
    nlinks1 = get_node_nlinks(node1);
    nlinks2 = get_node_nlinks(node2);
    if (nlinks1 >= MAX_NUM_LINKS || nlinks2 >= MAX_NUM_LINKS) {
        return -1;
    }
	/*
     * make sure the link between node1 and node2
     * does not already exist
     */
    for (i = 0; i < nlinks1; i++) {
        link1 = get_node_link(node1, i);
        node = get_link_node2(link1);
		if (is_same_node(node, node2)) {
			return -1;
        }
	}
    /*
     * now, it is safe to create a link between 
     * node1 and node2
     */
    link1 = get_node_link(node1, nlinks1);
    link1->node1 = node1;
	link1->node2 = node2;
    link2 = get_node_link(node2, nlinks2);
    link2->node1 = node2;
    link2->node2 = node1;
    /* 
     * generate link bandwidth
     */
    u = (float) rand() / RAND_MAX;
    b = bMIN + u * (bMAX - bMIN);
    link1->bandwidth = b;
    link2->bandwidth = b;
    /*
     * generate link delay
     */
    u = (float) rand() / RAND_MAX;
    d = dMIN + u * (dMAX - dMIN);
    link1->delay = d;
    link2->delay = d;
    /*
     * compute total_delay
     */
    td = (PACKET_SIZE / b) + d;
    td = (td >= 32.0)? 32.0: td;
    link1->total_delay = td;
    link2->total_delay = td;
    
    /*
     * update nlinks for node1 and node2
     */
	set_node_nlinks(node1, nlinks1 + 1);
	set_node_nlinks(node2, nlinks2 + 1);
#undef bMIN
#undef bMAX
#undef dMIN  
#undef dMAX 
	
	return 0;
}

int is_same_link(struct _link *link1, struct _link *link2) {

    assert( link1 != NULL );
    assert( link2 != NULL );

    if ( is_same_node(link1->node1, link2->node1) && 
         is_same_node(link1->node2, link2->node2) ) {
        return 1;
    }

    if ( is_same_node(link1->node1, link2->node2) && 
         is_same_node(link1->node2, link2->node1) ) {
        return 1;
    }

    return 0;
}

struct _node *get_link_node1(struct _link *link) {
    assert( link != NULL);

    return link->node1;
}

struct _node *get_link_node2(struct _link *link) {
    assert( link != NULL);

    return link->node2;
}

float get_link_total_delay(struct _link *link) {
    assert( link != NULL);

    return link->total_delay;
}

float set_link_total_delay(struct _link *link, float total_delay) {
    float old_total_delay;
    assert( link != NULL );
    old_total_delay = link->total_delay;
    link->total_delay = total_delay;

    return old_total_delay;
}

float get_link_bandwidth(struct _link *link) {
    assert( link != NULL);

    return link->bandwidth;
}

float set_link_bandwidth(struct _link *link, float bandwidth) {
    float old_bandwidth;
    assert( link != NULL );
    old_bandwidth = link->bandwidth;
    link->bandwidth = bandwidth;

    return old_bandwidth;
}

float get_link_delay(struct _link *link) {
    assert( link != NULL);

    return link->delay;
}

float set_link_delay(struct _link *link, float delay) {
    float old_delay;
    assert( link != NULL );
    old_delay = link->delay;
    link->delay = delay;

    return old_delay;
}

void print_link(struct _link *link) {
    struct _node *node2;

    assert( link != NULL );
    node2 = link->node2;
    printf("\tlinked with node %d \ttotal_delay %.2f \tbandwidth %.2f \tdelay %.2f\n",
            get_node_index(node2), link->total_delay, link->bandwidth, link->delay);

}
 

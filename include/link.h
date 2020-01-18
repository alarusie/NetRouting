#ifndef _LINK_H
#define _LINK_H

#include "node.h"

#define MAX_NUM_LINKS 4

struct _link {
    struct _node    *node1;         // node1
	struct _node 	*node2;	        // node2
	float 		     total_delay;	// total delay
    float            bandwidth;     // bandwidth
    float            delay;         // delay
};

int           create_link(struct _node *node1, struct _node *node2);
struct _node *get_link_node1(struct _link *link);
struct _node *get_link_node2(struct _link *link);
int           is_same_link(struct _link *link1, struct _link *link2);
float         get_link_total_delay(struct _link *link);
float         set_link_total_delay(struct _link *link, float total_delay);
float         get_link_bandwidth(struct _link *link);
float         set_link_bandwidth(struct _link *link, float bandwidth);
float         get_link_delay(struct _link *link);
float         set_link_delay(struct _link *link, float delay);
void          prrint_link(struct _link *link);

#endif

#ifndef _ROUTE_H
#define _ROUTE_H

#include "node.h"
#include "link.h"
#include <limits.h>

#define MAX_NUM_ROUTES  0x14
#define NONE            0x00
#define INFT            INT_MAX


struct _route {
	struct _node  *destination; 
	struct _link  *next_hop;    // next hop to destination
    float          total_cost;  // total cost to destination
};

int           init_route(struct _route *route, struct _node *destination);
struct _node *get_route_destination(struct _route *route);
struct _link *get_route_next_hop(struct _route *route);
float         get_route_total_cost(struct _route *route);
struct _node *set_route_destination(struct _route *route, struct _node *node);
struct _link *set_route_next_hop(struct _route *route, struct _link *next_hop);
float         set_route_total_cost(struct _route *route, float total_cost);
void          print_route(struct _route *route);

#endif

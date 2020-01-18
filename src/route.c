#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/route.h"

int init_route(struct _route *route, struct _node *destination) {
    assert( route != NULL );
    assert( destination != NULL );
    route->destination = destination;
    route->next_hop = NULL;
    route->total_cost = INFT;
    set_node_status(destination, NONE);
    
    return 0;
}

struct _node *get_route_destination(struct _route *route) {
    assert( route != NULL );
    
    return route->destination;
}

struct _link *get_route_next_hop(struct _route *route) {
    assert( route != NULL );
    
    return route->next_hop; 
}

float get_route_total_cost(struct _route *route) {
    assert( route != NULL );
    
    return route->total_cost; 
}

struct _node *set_route_destination(struct _route *route, struct _node *node) {
    struct _node *old_destination;

    assert( route != NULL );
    old_destination = route->destination;
    route->destination = node;

    return old_destination;
}

struct _link *set_route_next_hop(struct _route *route, struct _link *next_hop) {
    struct _link *old_next_hop;

    assert( route != NULL );
    old_next_hop = route->next_hop;
    route->next_hop = next_hop;

    return old_next_hop;
}

float set_route_total_cost(struct _route *route, float total_cost) {
    float old_total_cost;

    assert( route != NULL );
    old_total_cost = route->total_cost;
    route->total_cost = total_cost;

    return old_total_cost;
}

void print_route(struct _route *route) {
    struct _node *node2;

    assert( route != NULL );
    if (route->next_hop) {
        node2 = get_link_node2(route->next_hop);
        printf(" \tdestination %d \tnext_hop %d \ttotal_cost %.2f\n",
                get_node_index(route->destination),
                get_node_index(node2), route->total_cost);
    }
}

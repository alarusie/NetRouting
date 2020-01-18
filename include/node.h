#ifndef _NODE_H
#define _NODE_H

#include "link.h"
#include "route.h"
#include "queue.h"

#define MAX_NUM_PACKETS_TO_ROUTE 0x10
struct _node {
    int              index;                 // index within network->nodes[]
	int 	         data;	                // data
	int	             nlinks;                // number of links
	struct _link     *links;                // actual links
    int              nroutes;               // size of route table
    struct _route   *routes;                // route table
    struct _queue   **input_q;              // one input queue for each link
    struct _queue   **output_q;             // one output queue for each link
	int              status;        
};


struct _node  *create_node(int idx);
int            print_node(struct _node *node);
int            destroy_node(struct _node *node);
int            is_same_node(struct _node *node1, struct _node *node2);

int            get_node_index(struct _node *node);
int            set_node_index(struct _node *node, int index);

int            get_node_nlinks(struct _node *node);
int            set_ndeo_nlinks(struct _node *node, int nlinks);

int            get_node_nroutes(struct _node *node);
int            set_node_nroutes(struct _node *node, int nroutes);

int            get_node_status(struct _node *node);
int            set_node_status(struct _node *node, int status);

struct _link  *get_node_link(struct _node *node, int i);
struct _route *get_node_route(struct _node *node, int i);
struct _queue *get_node_input_q(struct _node *node, int i);
struct _queue *get_node_output_q(struct _node *node, int i);
struct _queue *lookup_node_input_q(struct _node *node, struct _link *link);
struct _queue *lookup_node_output_q(struct _node *node, struct _link *link);
struct _link  *lookup_node_next_hop(struct _node *node, struct _node *destination); 

#endif

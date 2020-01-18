#ifndef _NETWORK_H
#define _NETWORK_H

#include "queue.h"

#define NONE            0x00
#define HVST            0x01
#define VIST            0x02
#define INPR            0x04
#define DONE            0x05

#define INFT            INT_MAX

#define MAX_NUM_NODES 150 
struct _network
{
        int 	       size;    // number of nodes
        struct _node **nodes;   // array of nodes
};

struct _network *create_network(int size);
int              dfs_network(struct _node *node);
int              get_network_size(struct _network *network);
struct _node    *get_network_node(struct _network *network, int i);
struct _node    *set_network_node_at(struct _network *network, int i, struct _node *node);
int              print_network(struct _network *network);
int              destroy_network(struct _network *network);
int              is_connected_network(struct _network *network);
int              fix_network(struct _network *netwrok);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "../include/network.h"

struct _network *create_network(int size) {
	int              i;
    struct _network  *network;

    assert( size > 0 );
    network = (struct _network *) malloc(sizeof(struct _network));
    memset(network, 0, sizeof(struct _network));
    network->size = size;
    network->nodes = (struct _node **) malloc(network->size * sizeof(struct _node *));
    memset(network->nodes, 0, network->size * sizeof(struct _node *));

    return network;
}

int print_network(struct _network *network) {
    int i;

    assert( network != NULL );
    for (i = 0; i < network->size; i++) {
		printf("\n");
		print_node(network->nodes[i]);
    }

    return 0;
}

int destroy_network(struct _network *network) {
	int i;

    assert( network != NULL );
	for (i = 0; i < network->size; i++) {
		destroy_node(network->nodes[i]);
	}

    return 0;
}

int dfs_network(struct _node *node) {
    struct _node *node2;
    struct _link *link;
    int           i;

    set_node_status(node, HVST);
    for (i = 0; i < get_node_nlinks(node); i++) {
        link = get_node_link(node, i);
        node2 = get_link_node2(link);
        if (is_node_status(node2, NONE)) {
            if (dfs_network(node2) < 0) {
                return -1;
            }
        }
    }
    set_node_status(node, VIST);

    return 0;
}

int get_network_size(struct _network *network) {
    assert( network != NULL );

    return network->size;
}

struct _node *get_network_node(struct _network *network, int i) {
    assert( network != NULL );
    assert( i >= 0 );

    if (i < network->size) {
        return network->nodes[i];
    } else {
        return NULL;
    }
}

struct _node *set_network_node_at(struct _network *network, int i, struct _node *node) {
    struct _node *old_node;

    assert( network != NULL );
    assert( i >= 0 && i < network->size );
    assert( node != NULL );

    old_node = network->nodes[i];
    network->nodes[i] = node;

    return old_node;
}


int is_connected_network(struct _network *network) {
    int     i;

    assert( network != NULL );
    for (i = 0; i < network->size; i++) {
        set_node_status(network->nodes[i], NONE);
    }
    if (dfs_network(network->nodes[0]) < 0) {
            return -1;
    }

    for (i = 0; i < network->size; i++) {
        if (!is_node_status(network->nodes[i], VIST)) {
            printf("OOPS! NOT visited node!\n");
            print_node(network->nodes[i]);

            return 0;
        }
    }

    return 1;
}

int fix_network(struct _network *network) {
    struct _node    *node1;
    struct _node    *node2;
    int              i;

    if (dfs_network(network->nodes[0]) < 0) {
            return -1;
    }

    node1 = NULL;
    node2 = NULL;
    for (i = 0; i < network->size; i++) {
        if (is_node_status(network->nodes[i], VIST)) {
            if (node1 == NULL) {
                node1 = network->nodes[i];
            } 
            else if (get_node_nlinks(node1) > get_node_nlinks(network->nodes[i])) {
                node1 = network->nodes[i];
            }
        } else {
            if (node2 == NULL) {
                node2 = network->nodes[i];
            } else if (get_node_nlinks(node2) > get_node_nlinks(network->nodes[i])) {
                node2 = network->nodes[i];
            }
        }
    }

    if (node1 != NULL && node2 != NULL) {
        printf("fix_network: got node1 %d node2 %d\n",
                get_node_index(node1), get_node_index(node2));
        create_link(node1, node2);
        if (fix_network(network) < 0) {
            return -1;
        }
    }

    return 0;
}


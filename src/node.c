#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "../include/node.h"

struct _node *create_node(int index)
{
    struct _node    *node;
    int              i;
    assert( index >= 0 );
    node = (struct _node *)malloc(sizeof(struct _node));
	memset(node, 0, sizeof(struct _node));
    node->index = index;
    node->links = (struct _link *) malloc(MAX_NUM_LINKS * sizeof(struct _link));
    node->input_q = (struct _queue **) malloc(MAX_NUM_LINKS * sizeof(struct _queue **));
    node->output_q = (struct _queue **) malloc(MAX_NUM_LINKS * sizeof(struct _queue **));
    for (i =0; i < MAX_NUM_LINKS; i++) {
        node->input_q[i] = create_queue(MAX_NUM_PACKETS_TO_ROUTE);
        node->output_q[i] = create_queue(MAX_NUM_PACKETS_TO_ROUTE);
    }

    return node;
}

int is_same_node(struct _node *node1, struct _node *node2) {
    assert( node1 != NULL );
    assert( node2 != NULL );

    return (node1->index == node2->index);
}

int get_node_index(struct _node *node) {
    assert( node != NULL );

    return node->index;
}

int set_node_index(struct _node *node, int index) {
    int old_index;

    assert( node != NULL );
    assert( index >= 0 );
    old_index = node->index;
    node->index = index;

    return old_index;
}

int get_node_nlinks(struct _node *node) {
    assert( node != NULL );

    return node->nlinks;
}

int set_node_nlinks(struct _node *node, int nlinks) {
    int old_nlinks;

    assert( node != NULL );
    assert( nlinks >= 0 );
    old_nlinks = node->nlinks;
    node->nlinks = nlinks;

    return old_nlinks;
}

int get_node_nroutes(struct _node *node) {
    assert( node != NULL );

    return node->nroutes;
}

int set_node_nroutes(struct _node *node, int nroutes) {
    int old_nroutes;

    assert( node != NULL );
    assert( nroutes >= 0 );
    old_nroutes = node->nroutes;
    node->nroutes = nroutes;

    return old_nroutes;
}

int is_node_status(struct _node *node, int status) {
    assert( node != NULL );

    return (node->status == status);
}

int get_node_status(struct _node *node) {
    assert( node != NULL );

    return node->status;
}

int set_node_status(struct _node *node, int status) {
    int old_status;

    assert( node != NULL );
    assert( status >= 0 );
    old_status = node->index;
    node->status = status;

    return old_status;
}

struct _link *get_node_link(struct _node *node, int i) {
    assert( node != NULL );
    assert( i >= 0);
    if (i < MAX_NUM_LINKS) {
        return &node->links[i];
    } else {
        return NULL;
    }
}

struct _route *get_node_route(struct _node *node, int i) {
    assert( node != NULL );
    assert( i >= 0);
    if (i < node->nroutes) {
        return &node->routes[i];
    } else {
        return NULL;
    }
}

int allocate_node_routes(struct _node *node, int size) {
    assert( node != NULL );
    assert( size >= 0 );
    node->nroutes = size;
	node->routes = (struct _route *) malloc(size * sizeof(struct _route));
	memset(node->routes, 0, size * sizeof(struct _route));
	
    return 0;
}

struct _queue *get_node_input_q(struct _node *node, int i) {
    assert( node != NULL );

    if (i >= 0 && i < node->nlinks) {
        return node->input_q[i];
    } else {
        return NULL;
    }
}

struct _queue *get_node_output_q(struct _node *node, int i) {
    assert( node != NULL );

    if (i >= 0 && i < node->nlinks) {
        return node->output_q[i];
    } else {
        return NULL;
    }
}

struct _link *lookup_node_next_hop(struct _node *node, struct _node *destination) {
    struct _link *next_hop;
    int           i;

    assert( node != NULL );
    assert( destination != NULL );
    next_hop = NULL;
    for (i = 0; i < node->nroutes; i++) {
        if (is_same_node(node->routes[i].destination, destination)) {
            next_hop = node->routes[i].next_hop;
        }
    }

    return next_hop;
}

struct _queue *lookup_node_input_q(struct _node *node, struct _link *link) {
    int           i;

    assert( node != NULL );
    assert( link != NULL );
    for (i = 0; i < node->nlinks; i++) {
        if (is_same_link(&node->links[i], link)) {
            break;
        }
    }

    if (i == node->nlinks) {
        return NULL;
    } else {
        return node->input_q[i];
    }
}

struct _queue *lookup_node_output_q(struct _node *node, struct _link *link) {
    int           i;

    assert( node != NULL );
    assert( link != NULL );
    for (i = 0; i < node->nlinks; i++) {
        if (is_same_link(&node->links[i], link)) {
            break;
        }
    }

    if (i == node->nlinks) {
        return NULL;
    } else {
        return node->output_q[i];
    }
}


int print_node(struct _node *node) {
	int     i;

    assert( node != NULL );
    printf("++++ node %d\n", node->index);
	printf ("node[0x%p data %d nlinks %d nroutes %d]\n",
            node, node->data, node->nlinks, node->nroutes);
    if (node->nlinks) {
        printf("   links:\n");
        for (i = 0; i < node->nlinks; i++) {
            print_link(&node->links[i]);
        }
    }
    if (node->nroutes) {
        printf("   route table:\n");
        for (i = 0; i < node->nroutes; i++) {
                print_route(&node->routes[i]);
        }
    }

    return 0;
}

int destroy_node(struct _node *node) {
    int i;

	assert( node != NULL );

    free(node->links);
    if (node->nroutes) {
		free(node->routes);
	}
    for (i =0; i < MAX_NUM_LINKS; i++) { 
        destroy_queue(node->input_q[i]);
        destroy_queue(node->output_q[i]);
    }
    free(node->input_q);
    free(node->output_q);
	free(node);
    return 0;
}



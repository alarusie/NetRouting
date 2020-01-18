#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "../include/network.h"
#include <math.h>
#include<stdlib.h>

// Driver program to test above functions

int main(int argc, char *argv[]) {
    struct _network *network;

    float u;
    if ( argc != 3 ) {
        /* argc must be 3 */
        printf("usage: %s seed filename\n", argv[0]);
        return -1;
    }

    srand(atoi(argv[1]));

    network = (struct _network *) rand_init_network(MAX_NUM_NODES);
    printf("Reading network from file: %s\n", argv[2]);
    network = read_from_file(argv[2]);
	if (!is_connected_network(network)) {
        printf("ERROR! not connected network\n");
    }
    printf("Generate routers tables\n");
    generate_routes_tables(network);
    simulate_network(network);
    print_statistics();
	destroy_network(network);

    return 0;
}

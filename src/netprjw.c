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

    printf("Generate randomly network ... \n");
    network = (struct _network *) rand_init_network(MAX_NUM_NODES);
	if (!is_connected_network(network)) {
        printf("ERROR! not connected network!\n");
        printf("Going to fix it\n");
        fix_network(network);
        is_connected_network(network);
    }
    printf("Writing network to file: %s\n", argv[2]);
    write_to_file(network, argv[2]);
	destroy_network(network);

    return 0;
}

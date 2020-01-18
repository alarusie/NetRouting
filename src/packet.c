#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "../include/packet.h"

struct _packet  *create_packet(int ID, struct _node *source, struct _node *destination) {

    struct _packet *packet;
    float           u;
#define sMIN    0
#define sMAX    1000 /* 1mb */
    assert( ID >= 0);
    assert( source != NULL );
    assert( destination != NULL);

    packet = (struct _packet *) malloc(sizeof(struct _packet));
    memset(packet, 0, sizeof(struct _packet));
    packet->ID = ID;
    u = (float) rand() / RAND_MAX;
    packet->size = sMIN + u * (sMAX - sMIN);
    packet->source = source;
    packet->destination = destination;
    packet->hop = NULL;
    packet->status = ON_NONE;
#undef sMIN
#undef sMAX
    return packet;
}

int get_packet_ID(struct _packet *packet) {
    assert( packet != NULL);
    
    return packet->ID;
}

int set_packet_ID(struct _packet *packet, int ID) {
    int old_ID;

    assert( packet != NULL);
    assert( ID >= 0);
    old_ID = packet->ID;
    packet->ID = ID;

    return old_ID;
}

int get_packet_size(struct _packet *packet) {
    assert( packet != NULL);
    
    return packet->size;
}

int set_packet_size(struct _packet *packet, int size) {
    int old_size;

    assert( packet != NULL);
    assert( size > 0);
    old_size = packet->size;
    packet->size = size;

    return old_size;
}

int is_packet_status(struct _packet *packet, int status) {
    assert (packet != NULL );
    
    return (packet->status == status);
}

int set_packet_status(struct _packet *packet, int status) {
    int old_status;

    assert( packet != NULL );
    assert( status >= 0 && status <= 0x4);
    old_status = packet->status;
    packet->status = status;
    
    return old_status;
}

struct _node *get_packet_source(struct _packet *packet) {
    assert( packet != NULL );
    
    return packet->source;
}

struct _node *get_packet_destination(struct _packet *packet) {
    assert( packet != NULL );
    
    return packet->destination;
}

struct _node *get_packet_hop(struct _packet *packet) {
    assert( packet != NULL );
    
    return packet->hop;
}

struct _node * assign_packet_hop(struct _packet *packet, struct _node *hop) {
    struct _node *old_hop;

    assert( packet != NULL);
    assert( hop != NULL);
    old_hop = packet->hop;
    packet->hop = hop;

    return old_hop;
}

int get_packet_t(struct _packet *packet) {
    assert( packet != NULL);
    
    return packet->t;
}

int update_packet_t(struct _packet *packet, int t) {
    int old_t;

    assert( packet != NULL);
    assert( t >= 0);
    old_t = packet->t;
    packet->t = t;

    return old_t;
}

int destroty_packet(struct _packet *packet) {
    assert( packet != NULL );
    free(packet);

    return 0;
}


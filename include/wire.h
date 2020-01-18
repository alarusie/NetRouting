#ifndef _WIRE_H
#define _WIRE_H

#include "packet.h"
struct _llpacket {
    struct _packet      *packet;
    struct _llpacket    *next;
};

struct _wire {
    struct _llpacket *head;
    struct _llpacket *curr;
};

struct _wire    *create_wire();
struct _wire    *extract_wire_packets_by_t(struct _wire *wire, int t); 
struct _wire    *extract_wire_packets_by_status(struct _wire *wire, int status);
int              place_wire_packet(struct _wire *wire, struct _packet *packet);
int              remove_wire_packet(struct _wire *wire, struct _packet *packet);
struct _packet  *get_wire_first_packet(struct _wire *wire);
struct _packet  *get_wire_next_packet(struct _wire *wire);
struct _packet  *get_wire_current_packet(struct _wire *wire);
int              get_wire_num_packet(struct _wire *wire, int status);
int              reset_wire_curr(struct _wire *wire);
#endif


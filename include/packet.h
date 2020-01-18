#ifndef _PACKET
#define _PACKET

#include "node.h"

#define ON_NONE     0x00
#define ON_WIRE     0x01
#define ON_INPUT_Q  0x02
#define ON_OUTPUT_Q 0x03
#define DROP        0x04
#define DELIVERED   0x05

struct _packet {
    int              ID;
    int              size;
    struct _node    *source;
    struct _node    *destination;
    struct _node    *hop;
    int              t;
    int              status; // ON_WIRE, ON_INPUT_Q, ON_OUTPUT_Q
};

struct _packet  *create_packet(int ID, struct _node *source, struct _node *destination);
int              get_packet_ID(struct _packet *packet);
int              set_packet_ID(struct _packet *packet, int ID);
int              get_packet_size(struct _packet *packet);
int              set_packet_size(struct _packet *packet, int size);
int              is_packet_status(struct _packet *packet, int status);
int              set_packet_status(struct _packet *packet, int status);
struct _node    *get_packet_source(struct _packet *packet);
struct _node    *get_packet_destination(struct _packet *packet);
struct _node    *get_packet_hop(struct _packet *packet);
struct _node    *assign_packet_hop(struct _packet *packet, struct _node *hop);
int              get_packet_t(struct _packet *packet);
int              update_packet_t(struct _packet *packet, int t);
int              destroty_packet(struct _packet *packet);

#endif

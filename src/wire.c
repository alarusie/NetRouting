#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/wire.h"

struct _wire *create_wire() {
    struct _wire *wire;

    wire = (struct _wire *) malloc(sizeof(struct _wire));
    memset(wire, 0, sizeof(struct _wire));
    wire->head = NULL;
    wire->curr = NULL;

    return wire;
}

struct _wire *extract_wire_packets_by_t(struct _wire *wire, int t) {
    struct _wire        *subwire;
    struct _packet      *packet;
    struct _llpacket    *llpacket;
#define curr (subwire->curr)

    assert( wire != NULL );
   
    subwire = create_wire();
    packet = get_wire_first_packet(wire);
    if (packet) {
        do {
            if (get_packet_t(packet) == t) {
                llpacket = (struct _llpacket *) malloc(sizeof(struct _llpacket));
                memset(llpacket, 0, sizeof(struct _llpacket));
                llpacket->packet = packet;
                llpacket->next = NULL;
                if (subwire->head == NULL) {
                    subwire->head = llpacket;
                    curr = subwire->head;
                } else {
                    curr->next = llpacket;
                }
            }
        } while ((packet = get_wire_next_packet(wire)));
    }
#undef curr
    return subwire;
}

struct _wire *extract_wire_packets_by_status(struct _wire *wire, int status) {
    struct _wire        *subwire;
    struct _packet      *packet;
    struct _llpacket    *llpacket;
#define curr (subwire->curr)

    assert( wire != NULL );

    subwire = create_wire();
    packet = get_wire_first_packet(wire);
    if (packet) {
        do {
            if (is_packet_status(packet, status)) {
                
                llpacket = (struct _llpacket *) malloc(sizeof(struct _llpacket));
                memset(llpacket, 0, sizeof(struct _llpacket));
                llpacket->packet = packet;
                llpacket->next = NULL;
                if (subwire->head == NULL) {
                    subwire->head = llpacket;
                    curr = subwire->head;
                } else {
                    curr->next = llpacket;
                }
            } 
        } while ((packet = get_wire_next_packet(wire)));
    }
#undef curr
    return subwire;
}
 
int place_wire_packet(struct _wire *wire, struct _packet *packet) {
    struct _llpacket *prev;
    int              i;
#define curr (wire->curr)    
    assert( packet != NULL );
    i = 0;
    if (wire->head == NULL) {
        wire->head = (struct _llpacket *) malloc(sizeof(struct _llpacket));
        memset(wire->head, 0, sizeof(struct _llpacket));
    }
    prev = wire->head;
    while(prev->next) {
        i++;
        prev = prev->next;
    }
    curr = (struct _llpacket *) malloc(sizeof(struct _llpacket));
    memset(curr, 0, sizeof(struct _llpacket));
    curr->packet = packet;
    curr->next = NULL;
    prev->next = curr;
#undef curr
    return i+1;
}

int remove_wire_packet(struct _wire *wire, struct _packet *packet) {
    struct _llpacket *prev;
    int               i;
    int               found;
#define curr (wire->curr)

    assert( wire != NULL);
    assert( packet != NULL );
    found = 0;
    i = 1;

    curr = wire->head;
    if (curr->packet == packet) {
        found = 1;
        wire->head = curr->next;
        free(curr);
        prev = wire->head;
    } else{
        prev = curr;
        curr = curr->next;
    }
    while(curr) {
        i++;
        if (curr->packet == packet) {
            found = 1;
            prev->next = curr->next;
            free(curr);
            break;
        }
        curr = curr->next;
        prev = prev->next;
    }
    curr = prev;
#undef curr
    return (found)? i: 0;
}

struct _packet *get_wire_first_packet(struct _wire *wire) {
#define curr (wire->curr)
    assert( wire != NULL );

    if (wire->head) {
        curr = wire->head;
        return wire->head->packet;
    } else {
        return NULL;
    }
#undef curr
}

struct _packet *get_wire_next_packet(struct _wire *wire) {
#define curr (wire->curr)
    assert( wire != NULL );

    if (curr && curr->next) {
        curr = curr->next;
        return curr->packet;
    } else {
        curr = wire->head;
        return NULL;
    }
#undef curr
}

struct _packet *get_wire_current_packet(struct _wire *wire) {
#define curr wire->curr
    assert( wire != NULL );
    if (curr) {
        return curr->packet;
    } else {
        return NULL;
    }
#undef curr
}

int get_wire_num_packets(struct _wire *wire, int status) {
    struct _packet *packet;
    int             scount;
    int             tcount;

    assert( wire != NULL );
    scount = 0;
    tcount = 0;
    packet = get_wire_first_packet(wire);
    if (packet) {
        do {
            if (is_packet_status(packet, status)) {
                scount++;
            } else {
                tcount++;
            }
        } while ((packet = get_wire_next_packet(wire)));
    }
    if (status == -1) {
        return tcount+scount;
    } else {
        return scount;
    }
}

int reset_wire_curr(struct _wire *wire) {
    assert( wire != NULL );
    wire->curr = wire->head;

    return 0;
}


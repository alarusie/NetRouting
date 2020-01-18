#ifndef _QUEUE
#define _QUEUE

#include "packet.h"

struct _queue {
    struct _packet  **packets;
    int               front;
    int               size;
};

struct _queue   *create_queue(int size);
int              destroy_queue(struct _queue *queue);
int              enqueue(struct _queue *queue, struct _packet *packet);
struct _packet  *dequeue(struct _queue *queue);
struct _packet  *front(struct _queue *queue);
int              is_queue_empty(struct _queue *queue);
int              get_queue_size(struct _queue *queue);
#endif

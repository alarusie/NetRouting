#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/queue.h"

struct _queue *create_queue(int size) {
    struct _queue *queue;

    assert( size > 0);
    queue = (struct _queue *) malloc(sizeof(struct _queue));
    memset(queue, 0, sizeof(struct _queue));
    queue->size = size;
    queue->packets = (struct _packet **) malloc(queue->size * sizeof(struct _packet **));
    memset(queue->packets, 0, queue->size * sizeof(struct _packet **));
    queue->front = 0;

    return queue;
}

int enqueue(struct _queue *queue, struct _packet *packet) {
    assert( queue != NULL );
    assert( packet != NULL );
    if (queue->front >= queue->size) {
        return -1;
    }
    queue->packets[queue->front] = packet;
    queue->front++;

    return 0;
}

struct _packet *dequeue(struct _queue *queue) {
    struct _packet *packet = NULL;

    assert( queue != NULL );
    if (queue->front > 0) {
        --queue->front;
        packet = queue->packets[queue->front];
        queue->packets[queue->front] = NULL;
    }

    return packet;
}

struct _packet *front(struct _queue *queue) {
    struct _packet *packet = NULL;

    assert( queue != NULL );
    if (queue->front > 0) {
        packet = queue->packets[queue->front];
    }

    return packet;
}


int is_queue_empty(struct _queue *queue) {
    assert( queue != NULL );
    return (queue->front == 0);
}

int get_queue_size(struct _queue *queue) {
    assert( queue != NULL);
    return queue->size;
}

int destroy_queue(struct _queue *queue) {
    assert( queue != NULL);
    free(queue->packets);
    free(queue);

    return 0;
}

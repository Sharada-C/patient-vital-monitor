#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <pthread.h>

#include "vital.h"
#include "monitor.h"

#define BUFFER_SIZE 10

typedef struct
{
    VitalReading buffer[BUFFER_SIZE];

    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

    MonitorState *state;

} RingBuffer;

void ring_buffer_init(RingBuffer *rb, MonitorState *state);

int ring_buffer_push(RingBuffer *rb, VitalReading reading);

int ring_buffer_pop(RingBuffer *rb, VitalReading *reading);
void ring_buffer_stop(RingBuffer *rb);
void ring_buffer_destroy(RingBuffer *rb);

#endif

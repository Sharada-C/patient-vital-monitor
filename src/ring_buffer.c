#include <stdatomic.h>

#include "ring_buffer.h"

void ring_buffer_init(RingBuffer *rb, MonitorState *state)
{
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->state = state;

    pthread_mutex_init(&rb->mutex, NULL);
    pthread_cond_init(&rb->not_empty, NULL);
    pthread_cond_init(&rb->not_full, NULL);
}

int ring_buffer_push(RingBuffer *rb, VitalReading reading)
{
    pthread_mutex_lock(&rb->mutex);

    while (rb->count == BUFFER_SIZE &&
           atomic_load(&rb->state->running))
    {
        pthread_cond_wait(&rb->not_full, &rb->mutex);
    }

    /*
     * Shutdown was requested while we were waiting.
     */
    if (!atomic_load(&rb->state->running))
    {
        pthread_mutex_unlock(&rb->mutex);
        return 0;
    }

    rb->buffer[rb->head] = reading;
    rb->head = (rb->head + 1) % BUFFER_SIZE;
    rb->count++;

    pthread_cond_signal(&rb->not_empty);

    pthread_mutex_unlock(&rb->mutex);

    return 1;
}

int ring_buffer_pop(RingBuffer *rb, VitalReading *reading)
{
    pthread_mutex_lock(&rb->mutex);

    while (rb->count == 0 &&
           atomic_load(&rb->state->running))
    {
        pthread_cond_wait(&rb->not_empty, &rb->mutex);
    }

    /*
     * Shutdown was requested and there is no reading
     * left to process.
     */
    if (rb->count == 0 &&
        !atomic_load(&rb->state->running))
    {
        pthread_mutex_unlock(&rb->mutex);
        return 0;
    }

    *reading = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
    rb->count--;

    pthread_cond_signal(&rb->not_full);

    pthread_mutex_unlock(&rb->mutex);

    return 1;
}

void ring_buffer_stop(RingBuffer *rb)
{
    atomic_store(&rb->state->running, 0);

    pthread_mutex_lock(&rb->mutex);

    /*
     * Wake any threads currently waiting on the
     * condition variables so they can observe shutdown.
     */
    pthread_cond_broadcast(&rb->not_empty);
    pthread_cond_broadcast(&rb->not_full);

    pthread_mutex_unlock(&rb->mutex);
}

void ring_buffer_destroy(RingBuffer *rb)
{
    pthread_mutex_destroy(&rb->mutex);
    pthread_cond_destroy(&rb->not_empty);
    pthread_cond_destroy(&rb->not_full);
}

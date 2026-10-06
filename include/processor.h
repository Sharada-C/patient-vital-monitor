#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "ring_buffer.h"
#include "monitor.h"

typedef struct
{
    RingBuffer *buffer;
    MonitorState *state;
} ProcessorContext;

void *processor_thread(void *arg);

#endif

#ifndef SENSOR_H
#define SENSOR_H

#include "ring_buffer.h"
#include "monitor.h"

typedef enum
{
    SENSOR_HEART_RATE,
    SENSOR_SPO2,
    SENSOR_TEMPERATURE
} SensorType;

typedef struct
{
    RingBuffer *buffer;
    MonitorState *state;
    SensorType type;
    
    unsigned int seed;
} SensorContext;

void *sensor_thread(void *arg);

#endif

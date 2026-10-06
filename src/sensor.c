#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include "sensor.h"

static VitalReading generate_reading(SensorType type, unsigned int *seed)
{
    VitalReading reading;

    reading.heart_rate = 70;
    reading.spo2 = 98;
    reading.temperature = 36.7;

    if (type == SENSOR_HEART_RATE)
    {
        reading.heart_rate = 60 + rand_r(seed) % 81;

    }
    else if (type == SENSOR_SPO2)
    {
        reading.spo2 = 85 + rand_r(seed) % 16;
    }
    else if (type == SENSOR_TEMPERATURE)
    {
        reading.temperature = 36.0 + (rand_r(seed) % 41) / 10.0;
    }

    return reading;
}

void *sensor_thread(void *arg)
{
    SensorContext *context = (SensorContext *)arg;

    while (atomic_load(&context->state->running))
    {
        VitalReading reading = generate_reading(context->type, &context->seed);

        if (!ring_buffer_push(context->buffer, reading))
        {
            break;
        }

        printf("[Sensor] Reading added\n");

        sleep(1);
    }

    return NULL;
}

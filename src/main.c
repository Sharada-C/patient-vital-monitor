#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <signal.h>

#include "sensor.h"
#include "processor.h"
#include "logger.h"
#include "monitor.h"

static MonitorState *global_state = NULL;

static void handle_sigint(int signal_number)
{
    (void)signal_number;

    if (global_state != NULL)
    {
        monitor_request_shutdown(global_state);
    }
}

int main(void)
{
    /*
     * Shared monitor state.
     */
    MonitorState state;
    atomic_init(&state.running, 1);

    global_state = &state;

    /*
     * Initialize the shared ring buffer.
     */
    RingBuffer buffer;
    ring_buffer_init(&buffer, &state);

    /*
     * Initialize logger.
     */
    if (!logger_init("logs/monitor.log"))
    {
        fprintf(stderr, "Error: failed to initialize logger.\n");

        ring_buffer_destroy(&buffer);

        return 1;
    }

    /*
     * Register Ctrl+C handler.
     */
    if (signal(SIGINT, handle_sigint) == SIG_ERR)
    {
        fprintf(stderr, "Error: failed to register SIGINT handler.\n");

        logger_close();
        ring_buffer_destroy(&buffer);

        return 1;
    }

    /*
     * Initialize random seed.
     */
    unsigned int seed = (unsigned int)time(NULL);

    /*
     * Sensor contexts.
     */
    SensorContext heart_rate_sensor = {
        .buffer = &buffer,
        .state = &state,
        .type = SENSOR_HEART_RATE,
        .seed = seed + 1
    };

    SensorContext spo2_sensor = {
        .buffer = &buffer,
        .state = &state,
        .type = SENSOR_SPO2,
        .seed = seed + 2
    };

    SensorContext temperature_sensor = {
        .buffer = &buffer,
        .state = &state,
        .type = SENSOR_TEMPERATURE,
        .seed = seed + 3
    };

    /*
     * Processor context.
     */
    ProcessorContext processor = {
        .buffer = &buffer,
        .state = &state
    };

    /*
     * Thread IDs.
     */
    pthread_t heart_rate_thread;
    pthread_t spo2_thread;
    pthread_t temperature_thread;
    pthread_t processor_thread_id;

    /*
     * Track successfully created threads.
     */
    int heart_rate_created = 0;
    int spo2_created = 0;
    int temperature_created = 0;
    int processor_created = 0;

    /*
     * Create heart-rate sensor thread.
     */
    int result = pthread_create(
        &heart_rate_thread,
        NULL,
        sensor_thread,
        &heart_rate_sensor
    );

    if (result != 0)
    {
        fprintf(stderr,
                "Error: failed to create heart-rate sensor thread.\n");

        goto cleanup;
    }

    heart_rate_created = 1;

    /*
     * Create SpO2 sensor thread.
     */
    result = pthread_create(
        &spo2_thread,
        NULL,
        sensor_thread,
        &spo2_sensor
    );

    if (result != 0)
    {
        fprintf(stderr,
                "Error: failed to create SpO2 sensor thread.\n");

        goto shutdown;
    }

    spo2_created = 1;

    /*
     * Create temperature sensor thread.
     */
    result = pthread_create(
        &temperature_thread,
        NULL,
        sensor_thread,
        &temperature_sensor
    );

    if (result != 0)
    {
        fprintf(stderr,
                "Error: failed to create temperature sensor thread.\n");

        goto shutdown;
    }

    temperature_created = 1;

    /*
     * Create processor thread.
     */
    result = pthread_create(
        &processor_thread_id,
        NULL,
        processor_thread,
        &processor
    );

    if (result != 0)
    {
        fprintf(stderr,
                "Error: failed to create processor thread.\n");

        goto shutdown;
    }

    processor_created = 1;

    printf("Patient vital-sign monitor started.\n");
    printf("Press Ctrl+C to stop monitoring.\n");

    /*
     * Wait until SIGINT requests shutdown.
     *
     * The main thread does not perform processing.
     * Worker threads handle sensor readings and processing.
     */
    while (atomic_load(&state.running))
    {
        struct timespec delay = {
            .tv_sec = 0,
            .tv_nsec = 100000000
        };

        nanosleep(&delay, NULL);
    }

shutdown:

    /*
     * Stop the ring buffer and wake any blocked
     * producer or consumer threads.
     */
    ring_buffer_stop(&buffer);

    /*
     * Wait for successfully created threads.
     */
    if (heart_rate_created)
    {
        pthread_join(heart_rate_thread, NULL);
    }

    if (spo2_created)
    {
        pthread_join(spo2_thread, NULL);
    }

    if (temperature_created)
    {
        pthread_join(temperature_thread, NULL);
    }

    if (processor_created)
    {
        pthread_join(processor_thread_id, NULL);
    }

cleanup:

    logger_close();
    ring_buffer_destroy(&buffer);

    global_state = NULL;

    printf("Monitor shutdown complete.\n");

    return result == 0 ? 0 : 1;
}

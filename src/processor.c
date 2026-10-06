#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>

#include "processor.h"
#include "alarm.h"
#include "logger.h"

void *processor_thread(void *arg)
{
    ProcessorContext *context = (ProcessorContext *)arg;

    while (atomic_load(&context->state->running))
    {
        VitalReading reading;

        /*
         * Get the next reading from the shared buffer.
         * If shutdown was requested and there are no
         * readings left, ring_buffer_pop() returns 0.
         */
        if (!ring_buffer_pop(context->buffer, &reading))
        {
            break;
        }

        /*
         * Evaluate the current vital signs.
         */
        AlarmLevel level = evaluate_alarm(&reading);

        /*
         * Store the reading and alarm status in the log.
         */
        logger_log_reading(&reading, level);

        /*
         * Display the processed reading.
         */
        printf(
            "[Processor] HR=%d SpO2=%d Temp=%.1f | Alarm=%s\n",
            reading.heart_rate,
            reading.spo2,
            reading.temperature,
            alarm_level_to_string(level)
        );

        /*
         * Simulate processing time.
         */
        usleep(200000);
    }

    return NULL;
}

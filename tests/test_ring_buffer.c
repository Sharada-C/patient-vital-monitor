#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>

#include "ring_buffer.h"
#include "monitor.h"

static void test_push_and_pop(void)
{
    MonitorState state;
    atomic_init(&state.running, 1);

    RingBuffer buffer;
    ring_buffer_init(&buffer, &state);

    VitalReading input = {
        .heart_rate = 80,
        .spo2 = 98,
        .temperature = 36.7
    };

    VitalReading output;

    assert(ring_buffer_push(&buffer, input) == 1);
    assert(ring_buffer_pop(&buffer, &output) == 1);

    assert(output.heart_rate == 80);
    assert(output.spo2 == 98);
    assert(output.temperature == 36.7f);

    ring_buffer_destroy(&buffer);
}

static void test_fifo_order(void)
{
    MonitorState state;
    atomic_init(&state.running, 1);

    RingBuffer buffer;
    ring_buffer_init(&buffer, &state);

    VitalReading first = {
        .heart_rate = 70,
        .spo2 = 98,
        .temperature = 36.5
    };

    VitalReading second = {
        .heart_rate = 90,
        .spo2 = 96,
        .temperature = 37.0
    };

    VitalReading output;

    assert(ring_buffer_push(&buffer, first) == 1);
    assert(ring_buffer_push(&buffer, second) == 1);

    assert(ring_buffer_pop(&buffer, &output) == 1);
    assert(output.heart_rate == 70);

    assert(ring_buffer_pop(&buffer, &output) == 1);
    assert(output.heart_rate == 90);

    ring_buffer_destroy(&buffer);
}

static void test_shutdown(void)
{
    MonitorState state;
    atomic_init(&state.running, 1);

    RingBuffer buffer;
    ring_buffer_init(&buffer, &state);

    ring_buffer_stop(&buffer);

    VitalReading reading = {
        .heart_rate = 80,
        .spo2 = 98,
        .temperature = 36.7
    };

    assert(ring_buffer_push(&buffer, reading) == 0);

    ring_buffer_destroy(&buffer);
}

int main(void)
{
    test_push_and_pop();
    test_fifo_order();
    test_shutdown();

    printf("All ring buffer tests passed.\n");

    return 0;
}

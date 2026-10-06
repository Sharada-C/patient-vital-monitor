#include <assert.h>
#include <stdio.h>

#include "vital.h"

static void test_vital_reading(void)
{
    VitalReading reading = {
        .heart_rate = 75,
        .spo2 = 98,
        .temperature = 36.7
    };

    assert(reading.heart_rate == 75);
    assert(reading.spo2 == 98);
    assert(reading.temperature == 36.7f);
}

int main(void)
{
    test_vital_reading();

    printf("All vital reading tests passed.\n");

    return 0;
}


#include <assert.h>
#include <stdio.h>

#include "alarm.h"
#include "vital.h"

static void test_normal_reading(void)
{
    VitalReading reading = {
        .heart_rate = 80,
        .spo2 = 98,
        .temperature = 36.7
    };

    assert(evaluate_alarm(&reading) == ALARM_NORMAL);
}

static void test_low_spo2(void)
{
    VitalReading reading = {
        .heart_rate = 80,
        .spo2 = 89,
        .temperature = 36.7
    };

    assert(evaluate_alarm(&reading) == ALARM_CRITICAL);
}

static void test_high_heart_rate(void)
{
    VitalReading reading = {
        .heart_rate = 121,
        .spo2 = 98,
        .temperature = 36.7
    };

    assert(evaluate_alarm(&reading) == ALARM_HIGH);
}

static void test_low_heart_rate(void)
{
    VitalReading reading = {
        .heart_rate = 49,
        .spo2 = 98,
        .temperature = 36.7
    };

    assert(evaluate_alarm(&reading) == ALARM_HIGH);
}

static void test_high_temperature(void)
{
    VitalReading reading = {
        .heart_rate = 80,
        .spo2 = 98,
        .temperature = 39.1
    };

    assert(evaluate_alarm(&reading) == ALARM_HIGH);
}

static void test_alarm_strings(void)
{
    assert(
        alarm_level_to_string(ALARM_NORMAL) != NULL
    );

    assert(
        alarm_level_to_string(ALARM_HIGH) != NULL
    );

    assert(
        alarm_level_to_string(ALARM_CRITICAL) != NULL
    );
}

int main(void)
{
    test_normal_reading();
    test_low_spo2();
    test_high_heart_rate();
    test_low_heart_rate();
    test_high_temperature();
    test_alarm_strings();

    printf("All alarm tests passed.\n");

    return 0;
}

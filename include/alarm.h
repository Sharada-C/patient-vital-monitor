#ifndef ALARM_H
#define ALARM_H

#include "vital.h"

typedef enum
{
    ALARM_NORMAL,
    ALARM_LOW,
    ALARM_HIGH,
    ALARM_CRITICAL
} AlarmLevel;

AlarmLevel evaluate_alarm(const VitalReading *reading);

const char *alarm_level_to_string(AlarmLevel level);

#endif

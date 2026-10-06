#include "alarm.h"

AlarmLevel evaluate_alarm(const VitalReading *reading)
{
    if (reading->spo2 < 90)
    {
        return ALARM_CRITICAL;
    }

    if (reading->heart_rate > 120 ||
        reading->heart_rate < 50)
    {
        return ALARM_HIGH;
    }

    if (reading->temperature > 39.0)
    {
        return ALARM_HIGH;
    }

    return ALARM_NORMAL;
}

const char *alarm_level_to_string(AlarmLevel level)
{
    switch (level)
    {
        case ALARM_NORMAL:
            return "NORMAL";

        case ALARM_LOW:
            return "LOW";

        case ALARM_HIGH:
            return "HIGH";

        case ALARM_CRITICAL:
            return "CRITICAL";

        default:
            return "UNKNOWN";
    }
}

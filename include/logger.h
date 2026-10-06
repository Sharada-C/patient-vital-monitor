#ifndef LOGGER_H
#define LOGGER_H

#include "vital.h"
#include "alarm.h"

int logger_init(const char *filename);

void logger_log_reading(
    const VitalReading *reading,
    AlarmLevel level
);

void logger_close(void);

#endif

#include <stdio.h>
#include <time.h>

#include "logger.h"

static FILE *log_file = NULL;

int logger_init(const char *filename)
{
    log_file = fopen(filename, "a");

    if (log_file == NULL)
    {
        return 0;
    }

    return 1;
}

void logger_log_reading(
    const VitalReading *reading,
    AlarmLevel level
)
{
    if (log_file == NULL)
    {
        return;
    }

    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);

    char timestamp[32];

    strftime(
        timestamp,
        sizeof(timestamp),
        "%Y-%m-%d %H:%M:%S",
        time_info
    );

    fprintf(
        log_file,
        "[%s] HR=%d SpO2=%d Temp=%.1f Alarm=%s\n",
        timestamp,
        reading->heart_rate,
        reading->spo2,
        reading->temperature,
        alarm_level_to_string(level)
    );

    fflush(log_file);
}

void logger_close(void)
{
    if (log_file != NULL)
    {
        fclose(log_file);
        log_file = NULL;
    }
}

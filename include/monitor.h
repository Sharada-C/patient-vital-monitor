#ifndef MONITOR_H
#define MONITOR_H

#include <stdatomic.h>

typedef struct
{
    atomic_int running;
} MonitorState;

void monitor_request_shutdown(MonitorState *state);

#endif

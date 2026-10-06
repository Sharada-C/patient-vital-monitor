#include <stdatomic.h>

#include "monitor.h"

void monitor_request_shutdown(MonitorState *state)
{
    atomic_store(&state->running, 0);
}

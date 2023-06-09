#include <stdint.h>
#include <tinycore/api.h>

// This is the event buffer, placed in zero page for efficiency.
__zpage struct event_t event;

uint8_t __saved_sp;

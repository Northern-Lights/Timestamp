/*
 * A Window with a SimpleMenuLayer that allows
 * a user to select a time and date display
 * format.
 */

#pragma once

#include <pebble.h>

#define FMT_SIZE 64

/*
 * Gets the set format string.
 */
char *get_format(void);

/*
 * A callback which initiates the window for
 * timestamp format selection.
 */
void timestamp_format_callback(int idx, void *ctx);
/*
 * A Window with a SimpleMenuLayer that allows
 * a user to select a time and date display
 * format.
 */

#pragma once

#include <pebble.h>

#define FMT_SIZE 64
#define TS_STR_SIZE 64

/*
 * Gets the set format string.
 */
char *get_format(void);

/*
 * Return the formatted timestamp string.
 */
char *get_formatted_timestamp(time_t timestamp);

/*
 * Initialize the format with the user's settings or default.
 */
void init_timestamp_format(void);

/*
 * A callback which initiates the window for
 * timestamp format selection.
 */
void timestamp_format_callback(int idx, void *ctx);
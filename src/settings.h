#pragma once

#include <pebble.h>

#define FMT_SIZE 64

// Use %x vs. %D for "locale-dependent" date
// Use %X vs. %T for "locale-dependent" date
const char *formats[] = {
  "%D %T",  // 08/23/01 14:55:02
  "%D %R",  // 08/23/01 14:55
  "%D %r"  // 08/23/01 02:55:02 pm
};

/*
 * Sets the format string selected at index idx.
 */
void set_format(int idx);

/*
 * Gets the set format string.
 */
char *get_format(void);
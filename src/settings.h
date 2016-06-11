#pragma once

#include <pebble.h>

#define FMT_SIZE 64

char format[FMT_SIZE];
const char *formats[] = {
  "%D %T",  // %m/%d/%y %H:%M:%S  (24hr)
  "%D %H:%M",  // %m/%d/%y %H:%M  (24hr)
  "%D %I:%M"  // %m/%d/%y %I:%M  (12hr)
};

char *get_format(int idx) {
  strncpy(format, formats[idx], FMT_SIZE);
  return format;
}
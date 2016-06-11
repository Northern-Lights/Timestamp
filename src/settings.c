#include <pebble.h>
#include "settings.h"

static char format[FMT_SIZE];

void set_format(int idx) {
  strncpy(format, formats[idx], sizeof(format) - 1);
}

char *get_format(void) {
  return format;
}
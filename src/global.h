/*
 * This header contains global #defines and variables
 * that are to be used across multiple modules.
 */

// Maximum timestamps that the app can store.
#define MAX_TIMESTAMPS (512)

#include <pebble.h>

/*
 * Storage-related
 */

// Storage key for number of entries
#define KNUM_ENTRIES (0)

/*
 * Settings-related; preferences the user can store.
 */

// Storage key for index of timestamp format
#define KTS_FORMAT (0xffffffff)
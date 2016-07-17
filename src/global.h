/*
 * This header contains global #defines and variables
 * that are to be used across multiple modules.
 */

// Maximum timestamps that the app can store.
#define MAX_TIMESTAMPS (512)

#include <pebble.h>

/*
 * Storage-related
 * Keys in range: 0xa0000000 - 0xafffffff
 */

// Storage key for number of entries
#define KNUM_ENTRIES (0xa0000000)

/*
 * Settings-related; preferences the user can store.
 * Keys in range: 0xf0000000 - 0xffffffff
 */

// Storage key for index of timestamp format
#define KTS_FORMAT (0xf0000000)
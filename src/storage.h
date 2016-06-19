/*
 * This module handles storage of timestamps.
 * Entries are basically stored in an array
 * starting with index 1 since 0 is reserved
 * for the number of timestamps.
 *
 * The future implementation should use a
 * linked list data structure to make deletion
 * more sensible.
 */

#pragma once

#include <pebble.h>

/*
 * Initialize the persistent storage.
 */
status_t init_storage(void);

/*
 * Get the number of entries available for viewing.
 */
uint32_t available_entries(void);

/*
 * Adds a time_t timestamp to persistent storage.
 */
status_t add_entry(time_t timestamp);

/*
 * Deletes the timestamp at index from persistent storage.
 * Shifts all later values to the beginning of the array if
 * the index is less than the number of entries.
 */
status_t delete_entry(uint32_t index);

/*
 * Deletes all entries from persistent storage.
 */
status_t delete_all_entries(void);

/*
 * Returns the number of timestamps.
 * The timestamps are stored in the array *timestamps.
 * If no timestamps are returned, *timestamps will be NULL.
 * The array must be freed when no longer used.
 */
uint32_t get_entries(time_t **timestamps);
#include <pebble.h>
#include "storage.h"

// TODO: Implement linked list for more efficient delete & move

#define MAX_TIMESTAMPS (512)

// Define static keys for persistent storage
// TODO: This should probably go under settings
#define KNUM_ENTRIES (0)

static uint32_t num_entries = 0;
static bool initialized = S_FALSE;

// This is a blank store. No entries.
// Returns: Result of write operation ( > 0  = success).
static status_t create_store() {
  num_entries = 0;
  status_t result = persist_write_int(KNUM_ENTRIES, num_entries);
  if (result < S_SUCCESS)
    APP_LOG(APP_LOG_LEVEL_ERROR,
            "Create store failed. Result: %d",
            (int) result);
  return result;
}

status_t init_storage() {
  status_t result = S_TRUE;
  if (persist_exists(KNUM_ENTRIES) == S_FALSE) {
    result = create_store();
    if (result < S_SUCCESS) {
      // Error; We should have an error handler.
      APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to create store.");
      return result;
    }
    else {
      APP_LOG(APP_LOG_LEVEL_INFO, "Created store successfully");
      initialized = S_TRUE;
    }
  }
  num_entries = persist_read_int(KNUM_ENTRIES);
  APP_LOG(APP_LOG_LEVEL_INFO, "There are %u entries", (unsigned int) num_entries);
  return result;
}

uint32_t available_entries() {
  return num_entries;
}

// Linear search to find index of our timestamp
// Return 0 on error since that is not used by timestamps.
static uint32_t get_timestamp_index(time_t timestamp) {
  for (uint32_t i = 0; i <= num_entries; i++) {
    if (!persist_exists(i)) {
      APP_LOG(
        APP_LOG_LEVEL_ERROR,
        "Searching for timestamp; num_entries = %u, but entry %u does not exist.",
        (unsigned int) num_entries,
        (unsigned int) i);
      return 0;
    }
    int32_t tmp = persist_read_int(i);
    if (tmp == timestamp) {
      return i;
    }
  }
  return 0;
}

// Need to check that we have not exceeded MAX_ENTRIES
status_t add_entry(time_t timestamp) {
  uint32_t position = num_entries + 1;
  status_t result = persist_write_int(position, (int) timestamp);
  if (result >= S_SUCCESS) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Successful write");
    num_entries++;
    status_t result2 = persist_write_int(KNUM_ENTRIES, num_entries);
    if (result2 < S_SUCCESS) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to update num_entries in add_entry()");
    }
  }
  else APP_LOG(APP_LOG_LEVEL_ERROR, "Not able to write entry to persistent storage. Result: %d WHICH MAY ACTUALLY REPRESENT THE NUMBER OF BYTES WRITTEN, NOT AN ERROR!", (int) result);
  
  return result;
}

status_t delete_entry(uint32_t index) {
  
  // Shift everything to the right of index toward the beginning of the array
  for (uint32_t i = index; i <= num_entries - 1; i++) {
    if (persist_exists(i) && persist_exists(i+1)) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Left and right exist for delete");
      persist_write_int(i, persist_read_int(i+1));
    }
    else {
      APP_LOG(
        APP_LOG_LEVEL_ERROR,
        "Tried to put value at %u into %u, but one does not exist.",
        (unsigned int) i+1,
        (unsigned int) i);
      return E_DOES_NOT_EXIST;
    }
  }
  
  // Delete the last entry and decrement the number of entries.
  status_t result = persist_delete(num_entries);
  if (result < S_SUCCESS) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to delete last entry after shift. Result: %d", (int) result);
    return result;
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Deleted index %u", (unsigned int) index);
  num_entries--;
  persist_write_int(KNUM_ENTRIES, num_entries);
  return result;
}

status_t delete_all_entries() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Deleting all %u entries", (unsigned int) num_entries);
  for (uint32_t i = num_entries; i >= 1; i--) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Trying to delete entry %u", (unsigned int) i);
    status_t result = delete_entry(i);
    if (result < S_SUCCESS) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Deletion of entry %u failed.", (unsigned int) i);
      return result;
    }
  }
  if (num_entries == 0)  return S_SUCCESS;
  APP_LOG(APP_LOG_LEVEL_ERROR, "Tried to delete all, but %u entries still exist.", (unsigned int) num_entries);
  return E_ERROR;
}

// TODO: Allow NULL **entries to just return # of available entries.
uint32_t get_entries(time_t **entries) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Getting entries");
  if (num_entries < 1) {
    entries = NULL;
    return 0;
  }
  *entries = (time_t *) malloc(num_entries * sizeof(time_t));
  APP_LOG(APP_LOG_LEVEL_INFO, "Initialized entry array %p", entries);
  
  // Get entries, stick them in the array.
  // If we run into a 0 time_t, error. Return NULL.
  for (uint32_t i = 1; i <= num_entries; i++) {
    time_t entry = persist_read_int(i);
    APP_LOG(APP_LOG_LEVEL_INFO, "Got entry %u: %u", (unsigned int) i, (unsigned int) entry);
    if (!entry) {
      free(entries);
      return 0;
    }
    (*entries)[i-1] = entry;
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Got %u entries", (unsigned int) num_entries);
  return num_entries;
}
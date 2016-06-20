#include <pebble.h>
#include "show_timestamps.h"

#include "timestamp_format.h"
#include "storage.h"

#define MSG_DELAY (3 * 1000)

// Just 1 section showing all timestamps
#define NSECTIONS 1

// Pointer to the show_timestamps item in the main menu
static SimpleMenuItem *menu_item = NULL;

static SimpleMenuLayer *smenu_layer;
static SimpleMenuSection smenu_sections[NSECTIONS];
static SimpleMenuItem *smenu_items = NULL;
static uint32_t num_entries = 0;
static time_t *entries = NULL;

static const char *no_entries_text = "No timestamps";

void show_timestamps_store_menu_item(SimpleMenuItem *item) {
  if (item) menu_item = item;
}

void show_timestamps_callback_end(void *ctx) {
  menu_item->callback = show_timestamps_callback;
  menu_item->subtitle = NULL;
  SimpleMenuLayer *smenu_layer = *((SimpleMenuLayer **) ctx);
  layer_mark_dirty((Layer *) smenu_layer);
}

void show_timestamps_callback(int idx, void *ctx) {
  
  // If no entries, just say so in the subtitle of the main_menu item.
  if (!available_entries()) {
    menu_item->subtitle = no_entries_text;
    SimpleMenuLayer *smenu_layer = *((SimpleMenuLayer**) ctx);
    layer_mark_dirty((Layer *) smenu_layer);
    app_timer_register(MSG_DELAY, show_timestamps_callback_end, ctx);
    menu_item->callback = NULL;
    return;
  }
  
  show_show_timestamps();
}

void show_action_bar(int idx, void *ctx);

static uint32_t setup_timestamp_menu(Window *window) {
  
  // Get saved timestamps and create an array for them
  num_entries = get_entries(&entries);
  smenu_items = (SimpleMenuItem *) malloc(sizeof(SimpleMenuItem) * num_entries);
  
  // For each time_t (epoch) timestamp, convert to string for menu display
  for (uint32_t i = 0; i < num_entries; i++) {
    time_t epoch = entries[i];
    struct tm *timedata = localtime(&epoch);
    char *title = (char *) malloc(64 * sizeof(char));  // Need non-stack mem
    strftime(title, 64, get_format(), timedata);
    smenu_items[i] = (SimpleMenuItem) {
//       .title = title,
//       .subtitle = NULL,
      .title = NULL,
      .subtitle = title,
      .callback = show_action_bar
    };
  }
  
  // Setup the section with the items
  smenu_sections[0] = (SimpleMenuSection) {
    .title = "Timestamps",
    .num_items = num_entries,
    .items = smenu_items
  };
  
  // Setup the menu layer, give it the items, and push the new window.
  smenu_layer = simple_menu_layer_create(
                  GRect(0, 0, 144, 152),
                  window,
                  smenu_sections,
                  NSECTIONS,
                  NULL);  //The last one is void *ctx which can be abitrary data passed to callbacks
  layer_add_child(window_get_root_layer(window), (Layer *) smenu_layer);
  window_stack_push(window, true);
  
  return num_entries;
}

static void setup_no_entries_window(Window *window) {
  TextLayer *text_layer = text_layer_create(GRect(0,0,144,152));
  text_layer_set_text(text_layer, no_entries_text);
  layer_add_child(window_get_root_layer(window), (Layer *) text_layer);
  window_stack_push(window, true);
}

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
}

static void destroy_ui(void) {
  window_destroy(s_window);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

// TODO: Remove "if"; decision made by the callback.
// Also make static since outsiders do not open this window.
void show_show_timestamps(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  
  if (available_entries()) {
    setup_timestamp_menu(s_window);
  } else {
    setup_no_entries_window(s_window);
  }
  
}

void hide_show_timestamps(void) {
  // Clean the slate; everything is redone on next show_show_timestamps.
  num_entries = 0;
  for (uint32_t i = 0; i < num_entries; i++) {
    char *title = (char *) (smenu_items[i].title);
    free(title);
  }
  free(entries);
  entries = NULL;
  free(smenu_items);
  smenu_items = NULL;
  window_stack_remove(s_window, true);
}

ActionBarLayer *abl;

void show_action_bar(int idx, void *ctx) {
  abl = action_bar_layer_create();
  action_bar_layer_add_to_window(abl, s_window);
//   layer_add_child(window_get_root_layer(window), (Layer *) smenu_layer);
  layer_mark_dirty(window_get_root_layer(s_window));
}
#include <pebble.h>
#include "global.h"
#include "storage.h"
#include "timestamp_format.h"
#include "view_timestamp.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static TextLayer *timestamp_label;
static ActionBarLayer *actionbar;
static TextLayer *timestamp_notes;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  // timestamp_label
  timestamp_label = text_layer_create(GRect(0, 0, 124, 20));
  text_layer_set_text(timestamp_label, "Date & Time");
  text_layer_set_text_alignment(timestamp_label, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timestamp_label);
  
  // actionbar
  actionbar = action_bar_layer_create();
  action_bar_layer_add_to_window(actionbar, s_window);
  action_bar_layer_set_background_color(actionbar, GColorBlack);
  layer_add_child(window_get_root_layer(s_window), (Layer *)actionbar);
  
  // timestamp_notes
  timestamp_notes = text_layer_create(GRect(0, 20, 124, 128));
  text_layer_set_text(timestamp_notes, "Notes regarding this timestamp input from your phone would go here.");
  layer_add_child(window_get_root_layer(s_window), (Layer *)timestamp_notes);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timestamp_label);
  action_bar_layer_destroy(actionbar);
  text_layer_destroy(timestamp_notes);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void show_view_timestamp(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_view_timestamp(void) {
  window_stack_remove(s_window, true);
}

static int selected = 0;

void view_timestamp(int idx, void *ctx) {
  show_view_timestamp();
  // TODO: Add Click config providers here.
  if (idx >= 0 && idx <= MAX_TIMESTAMPS) {
    selected = idx;
    time_t timestamp = get_entry(selected);
    text_layer_set_text(timestamp_label, get_formatted_timestamp(timestamp));
  }
}
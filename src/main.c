#include <pebble.h>
#include "main_menu.h"
#include "storage.h"

// TODO: Move the main_window into main_menu.c
static Window *message_window;

static void storage_error(Window *window) {
  TextLayer *text_layer = text_layer_create(GRect(0,0,144,152));
  text_layer_set_text(text_layer, "ERROR: Storage failed to initialize.");
  layer_add_child(window_get_root_layer(window), (Layer *) text_layer);
  window_stack_push(window, false);
}

static void unload_window(Window *window) {
  window_destroy(window);
}

static void setup_message_window(Window *window) {
  message_window = window_create();
  
  window_set_window_handlers(message_window, (WindowHandlers) {
    .unload = unload_window
  });
  
  #ifndef PBL_SDK_3
    window_set_fullscreen(message_window, 0);
  #endif
}

static void handle_init(void) {
  // Set up the message window in case we need it,
  // but don't push it unless it's needed.
  setup_message_window(message_window);
  
  if (init_storage() == S_FALSE) {
    storage_error(message_window);
    return;
  }
  
  setup_menu();
}

static void handle_deinit(void) {
  window_destroy(message_window);
}

int main(void) {
  /*
   * New feature: if opened using a hotkey (hold up or down for 2 sec),
   * then immediately take a snapshot, show a window saying so,
   * then... quit? Go to main menu?
   */
  handle_init();
  app_event_loop();
  handle_deinit();
}

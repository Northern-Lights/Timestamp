#include <pebble.h>
#include "main_menu.h"
#include "take_timestamp.h"
#include "show_timestamps.h"

#define NSECTIONS 2
#define NACTION_ITEMS 2
#define NCONFIG_ITEMS 1

// Action section and items
#define SACTION 0
#define ITIMESTAMP 0
#define ISHOW_TIMESTAMPS 1

// Config section and items
#define SCONFIG 1
#define ISETTINGS 0

static Window *main_menu_window;
static SimpleMenuLayer *smenu_layer;
static SimpleMenuSection smenu_sections[NSECTIONS];
static SimpleMenuItem action_items[NACTION_ITEMS];
static SimpleMenuItem config_items[NCONFIG_ITEMS];

static void setup_action_section(void) {
  action_items[ITIMESTAMP] = (SimpleMenuItem) {
    .title = "Take Timestamp",
    .subtitle = NULL,
    .callback = take_timestamp_callback
  };
  take_timestamp_store_menu_item(&action_items[ITIMESTAMP]);
  
  action_items[ISHOW_TIMESTAMPS] = (SimpleMenuItem) {
    .title = "Show Timestamps",
    .subtitle = NULL,
    .callback = show_timestamps_callback
  };
  show_timestamps_store_menu_item(&action_items[ISHOW_TIMESTAMPS]);
  
  smenu_sections[SACTION] = (SimpleMenuSection) {
    .title = "Actions",
    .num_items = NACTION_ITEMS,
    .items = action_items    
  };
}

// Temporary until we develop a true delete/delete all feature.
#include "storage.h"
void delete(int idx, void *ctx) {
  delete_all_entries();
}

static void setup_config_section(void) {
  config_items[ISETTINGS] = (SimpleMenuItem) {
    .title = "Clear All Data",
    .subtitle = "Wipe all entries",
    .callback = delete
  };
  
  smenu_sections[SCONFIG] = (SimpleMenuSection) {
    .title = "Configuration",
    .num_items = NCONFIG_ITEMS,
    .items = config_items
  };
}

static void destroy_menu(Window *window) {
  window_destroy(window);
  simple_menu_layer_destroy(smenu_layer);
}

void setup_menu() {
  
  main_menu_window = window_create();
  
  window_set_window_handlers(main_menu_window, (WindowHandlers) {
    .unload = destroy_menu,
  });
  
  setup_action_section();
  setup_config_section();

  smenu_layer = simple_menu_layer_create(
                  GRect(0, 0, 144, 152),
                  main_menu_window,
                  smenu_sections,
                  NSECTIONS,
                  &smenu_layer);  //*ctx so callbacks can refresh the menu
  layer_add_child(window_get_root_layer(main_menu_window), (Layer *) smenu_layer);
  window_stack_push(main_menu_window, true);
}
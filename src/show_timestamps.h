#include <pebble.h>

/*
 * Lets the take_timestamp component
 * store its own item pointer.
 */
void show_timestamps_store_menu_item(SimpleMenuItem *item);

/*
 * Callback to initiate the show_timestamps component.
 */
void show_timestamps_callback(int idx, void *ctx);

void show_show_timestamps(void);
void hide_show_timestamps(void);
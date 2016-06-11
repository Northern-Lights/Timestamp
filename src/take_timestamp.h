#include <pebble.h>

void show_take_timestamp(void);
void hide_take_timestamp(void);

/*
 * Lets the take_timestamp component
 * store its own item pointer.
 */
void take_timestamp_store_menu_item(SimpleMenuItem *item);

/*
 * A callback which displays a message when the user
 * selects to take a timestamp. Also temporarily
 * disables taking additional timestamps until the
 * message is cleared by take_timestamp_callback_end.
 */
void take_timestamp_callback(int idx, void *ctx);
/*
 * Retrieves the current epoch time and
 * enters it into permanent storage.
 *
 * Exposes a callback to display a message
 * in the SimpleMenuItem in the main_menu
 * when the timestamp is taken.
 */

#include <pebble.h>

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
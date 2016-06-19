/*
 * A Window with a SimpleMenuLayer that displays
 * a list of stored timestamps.
 *
 * Allows its SimpleMenuItem from main_menu to be
 * stored to display a message in the item
 * without opening a new window if no Timestamps
 * are stored.
 *
 * Timestamps are displayed in the subtitle with
 * a blank title for best fit since there is no
 * control over font/size for the text.
 */

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
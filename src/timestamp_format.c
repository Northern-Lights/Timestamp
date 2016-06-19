#include <pebble.h>
#include "global.h"
#include "timestamp_format.h"

// Use %x vs. %D for "locale-dependent" date
// Use %X vs. %T for "locale-dependent" time
static const char *formats[] = {
  "%x %X",  // 08/23/01 14:55:02
  "%x %R",  // 08/23/01 14:55
  "%x %r"  // 08/23/01 02:55:02 pm
};

static char format[FMT_SIZE];

#define NFORMATS ((int)(sizeof(formats)/sizeof(char *)))

/*
 * Sets the format string selected at index idx.
 * Takes (void *) because it is a SimpleMenuItem
 * callback but does nothing with it.
 */
static void set_format(int idx, void *ctx) {
  strncpy(format, formats[idx], sizeof(format) - 1);
  persist_write_int(KTS_FORMAT, (int) idx);
}

char *get_format(void) {
  
  // If not set, default to the first.
  // TODO: Make it a user setting so it can't be unset.
  if (format[0] == '\0') {
    set_format(0, NULL);
  }
  return format;
}

void init_timestamp_format(void) {
  
  // Default to the first format if not saved by user.
  uint32_t idx = 0;
  
  // Check if there's a user-saved value.
  // If so, and it's out of range, default to 0.
  if (persist_exists(KTS_FORMAT)) {
    idx = (uint32_t) persist_read_int(KTS_FORMAT);
    if (idx > NFORMATS) {
      idx = 0;
      APP_LOG(APP_LOG_LEVEL_WARNING,
              "TS_FORMAT is %u, but there are only %u formats.",
              (unsigned int) idx,
              (unsigned int) NFORMATS);
    }
  }
  
  set_format(idx, NULL);
}

#define NSECTIONS 1
#define NITEMS NFORMATS

static Window *window;
static SimpleMenuLayer *smenu_layer;
static SimpleMenuSection smenu_sections[NSECTIONS];
static SimpleMenuItem smenu_items[NITEMS];

#define NCHARS 30
static char format_strs[NITEMS][NCHARS];

static void hide_timestamp_format(Window *w) {
  window_destroy(w);
  window_stack_remove(w, true);
}

static void show_timestamp_format(void) {
  window = window_create();  
  window_set_window_handlers(window, (WindowHandlers) {
    .unload = hide_timestamp_format
  });
  
  // Take the time now and use it to display what the
  // timestamp would look like using all of our formats
  time_t t = time(NULL);
  struct tm *timenow = localtime(&t);
  
  // Set the subtitle of the items to the timestamp format
  for (int i = 0; i < NITEMS; i++) {
    strftime(format_strs[i], NCHARS, formats[i], timenow);
    smenu_items[i] = (SimpleMenuItem) {
      .title = NULL,
      .subtitle = format_strs[i],
      .callback = set_format
    };
  }
  
  // Create the section and give it the items
  smenu_sections[0] = (SimpleMenuSection) {
    .title = "Timestamp Display Format",
    .items = smenu_items,
    .num_items = NITEMS
  };
  
  // Set up the layer and display the window.
  smenu_layer = simple_menu_layer_create(
                  GRect(0, 0, 144, 152),
                  window,
                  smenu_sections,
                  NSECTIONS,
                  NULL);
  layer_add_child(window_get_root_layer(window), (Layer *) smenu_layer);
  window_stack_push(window, true);
}

void timestamp_format_callback(int idx, void *ctx) {
  show_timestamp_format();
}
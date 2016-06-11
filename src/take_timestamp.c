#include <pebble.h>
#include "take_timestamp.h"
#include "storage.h"
//#include "settings.h"  //Will be needed to get setings such as timestamp formatting.

#define BUFSIZE 64
#define MSG_DELAY (3*1000)

static SimpleMenuItem *menu_item = NULL;
const char *msg_succ = "Timestamp saved";
const char *msg_err = "Error saving timestamp";

// TODO: Get this from settings
int selected = 0;
static char *format[] = {
  "%D %T",  // %m/%d/%y %H:%M:%S
};

static char current_time[BUFSIZE];

static time_t get_time(void) {
  const time_t timenow = time(NULL);
  struct tm *timetm = localtime(&timenow);
  strftime(current_time, BUFSIZE, format[selected], timetm);
  return timenow;
}

void take_timestamp_store_menu_item(SimpleMenuItem *item) {
  if (item) menu_item = item;
}

static void take_timestamp_callback_end(void *ctx) {
  menu_item->callback = take_timestamp_callback;
  menu_item->subtitle = NULL;
  SimpleMenuLayer *smenu_layer = *((SimpleMenuLayer**) ctx);
  layer_mark_dirty((Layer *) smenu_layer);
}

void take_timestamp_callback(int idx, void *ctx) {
  // Add the entry to storage and select appropriate message
  time_t t = get_time();
  
  if (add_entry(t) > S_SUCCESS) menu_item->subtitle = msg_succ;
  else menu_item->subtitle = msg_err;
  
  SimpleMenuLayer *smenu_layer = *((SimpleMenuLayer**) ctx);
  layer_mark_dirty((Layer *) smenu_layer);
  app_timer_register(MSG_DELAY, take_timestamp_callback_end, ctx);
  menu_item->callback = NULL;
}
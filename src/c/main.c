#include <pebble.h>
#include "battery.h"
#include "times.h"
#include "health.h"
#include "train.h"

static Window* s_main_window;

static void main_window_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  create_battery_layers(window_layer);
  create_times_layers(window_layer);
  create_health_layers(window_layer);
  create_train_layers(window_layer);
}

static void main_window_unload(Window* window) {
  tick_timer_service_unsubscribe();
  health_service_events_unsubscribe();
  
  destroy_battery_layers();
  destroy_times_layers();
  destroy_health_layers();
  destroy_train_layers();
}

static void send_int(uint8_t key, uint8_t cmd) {
  DictionaryIterator* iter;
  app_message_outbox_begin(&iter);

  Tuplet value = TupletInteger(key, cmd);
  dict_write_tuplet(iter, &value);

  app_message_outbox_send();
}

void update_layers(struct tm* tick_time, TimeUnits units_changed) {
  update_times_layers(tick_time);

  if (tick_time->tm_min % 10 == 0)
    send_int(1, 1);  // send arbitrary message
}

static void inbox_received_callback(DictionaryIterator* iterator, void* context) {
  Tuple* train_tuple = dict_find(iterator, MESSAGE_KEY_MSG_TRAIN_TEXT);
  update_train_layers(train_tuple->value->cstring);
  
//  Tuple* times_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_TIME_OFFSET0);
//  update_times_offsets(times_tuple->value->int32);
}

static void init() {
  // Create main Window element
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorDarkCandyAppleRed);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
  
  // Subscribe to updates
  tick_timer_service_subscribe(MINUTE_UNIT, update_layers);
  health_service_events_subscribe(update_health_layers, NULL);
  app_message_register_inbox_received(inbox_received_callback);
  app_message_open(128, 128);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
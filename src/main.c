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
  destroy_battery_layers();
  destroy_times_layers();
  destroy_health_layers();
  destroy_train_layers();
}

void update_layers(struct tm* tick_time, TimeUnits units_changed) {
  update_times_layers(tick_time);
  update_train_layers(tick_time);
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
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
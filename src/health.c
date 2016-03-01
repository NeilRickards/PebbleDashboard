#include <pebble.h>
#include "health.h"

static TextLayer* s_steps_layer;

void create_health_layers(Layer* window_layer) {
  GRect bounds = layer_get_bounds(window_layer);

  s_steps_layer = text_layer_create(GRect(2, 88, bounds.size.w - 2, 26));
  text_layer_set_background_color(s_steps_layer, GColorClear);
  text_layer_set_text_color(s_steps_layer, GColorWhite);  
  text_layer_set_font(s_steps_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  layer_add_child(window_layer, text_layer_get_layer(s_steps_layer));
  
  update_health_layers(HealthEventSignificantUpdate, NULL);
}

void update_health_layers(HealthEventType event, void *context) {
  static char steps[16];
  snprintf(steps, sizeof(steps), "Steps: %d", (int)health_service_sum_today(HealthMetricStepCount));
  text_layer_set_text(s_steps_layer, steps);
}

void destroy_health_layers() {
  text_layer_destroy(s_steps_layer);
}
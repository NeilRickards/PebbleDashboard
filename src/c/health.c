#include "main.h"

static TextLayer* s_steps_layer;

void create_health_layers(Layer* window_layer) {
  GRect bounds = layer_get_bounds(window_layer);

  s_steps_layer = text_layer_create(GRect(2, 88, bounds.size.w - 2, 26));
  format_layer(s_steps_layer, FONT_KEY_GOTHIC_18, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_steps_layer));
  
  update_health_layers(HealthEventSignificantUpdate, NULL);
}

void update_health_layers(HealthEventType event, void* context) {
  static char steps[16];
  snprintf(steps, sizeof(steps), "Steps: %d", (int)health_service_sum_today(HealthMetricStepCount));
  text_layer_set_text(s_steps_layer, steps);
}

void destroy_health_layers() {
  text_layer_destroy(s_steps_layer);
}
#include <pebble.h>
#include "battery.h"

static Layer* s_output_layer;

static void draw_battery(Layer* layer, GContext* ctx) {
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_draw_rect(ctx, GRect(2, 0, 5, 2));
  graphics_draw_rect(ctx, GRect(0, 2, 9, 18));

  // Can't draw a 1 pixel rectangle.  Use 13 to ensure 10% => 2px height
  int gone = 13 - 0.13 * battery_state_service_peek().charge_percent;
  if (14-gone >= 2)
    graphics_fill_rect(ctx, GRect(2, 4+gone, 5, 14-gone), 0, GCornerNone);
}

void create_battery_layers(Layer* window_layer) {
  s_output_layer = layer_create(GRect(4, 22, 9, 20));
  layer_add_child(window_layer, s_output_layer);
  layer_set_update_proc(s_output_layer, draw_battery);
}

void destroy_battery_layers() {
  layer_destroy(s_output_layer);
}
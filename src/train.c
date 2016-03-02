#include <pebble.h>
#include "train.h"

static TextLayer* s_train_layer;

void create_train_layers(Layer* window_layer) {
  GRect bounds = layer_get_bounds(window_layer);

  s_train_layer = text_layer_create(GRect(2, 108, bounds.size.w - 2, 26));
  text_layer_set_background_color(s_train_layer, GColorClear);
  text_layer_set_text_color(s_train_layer, GColorWhite);  
  text_layer_set_font(s_train_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  layer_add_child(window_layer, text_layer_get_layer(s_train_layer));
}

void update_train_layers(const char* text) {
  text_layer_set_text(s_train_layer, text);
}

void destroy_train_layers() {
  text_layer_destroy(s_train_layer);
}
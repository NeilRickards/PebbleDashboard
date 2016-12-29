#include <pebble.h>
#include "utilities.h"

void format_layer(TextLayer* layer, const char* font, GTextAlignment alignment) {
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_text_alignment(layer, alignment);
  text_layer_set_font(layer, fonts_get_system_font(font));
}
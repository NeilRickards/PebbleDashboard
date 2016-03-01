#include <pebble.h>
#include "times.h"

#define NUM_INTL 3
// TODO: daylight savings
int s_intl_offsets[] = { -8, -5, 8 };  // Offsets from GMT
char* s_intl_places[] = { "sf%R", "ny%R", "si%R" };

static TextLayer* s_time_layer;
static TextLayer* s_date_layer;
static TextLayer* s_intl_layer[NUM_INTL];

static void format_layer(TextLayer* layer, const char* font) {
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_text_alignment(layer, GTextAlignmentRight);
  text_layer_set_font(layer, fonts_get_system_font(font));
}

void create_times_layers(Layer* window_layer) {
  GRect bounds = layer_get_bounds(window_layer);
  int width = bounds.size.w - 2;  // Border on right

  // Create the time TextLayer
  s_time_layer = text_layer_create(GRect(0, 0, width, 50));
  format_layer(s_time_layer, FONT_KEY_ROBOTO_BOLD_SUBSET_49);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  // Create the date TextLayer
  s_date_layer = text_layer_create(GRect(0, 48, width, 26));
  format_layer(s_date_layer, FONT_KEY_GOTHIC_24);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  // Create the international TextLayers
  int w = (bounds.size.w - 4) / NUM_INTL;
  for (int i = 0; i < NUM_INTL; ++i) {
    s_intl_layer[i] = text_layer_create(GRect(2+i*w, 72, w, 20));
    text_layer_set_background_color(s_intl_layer[i], GColorClear);
    text_layer_set_font(s_intl_layer[i], fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(window_layer, text_layer_get_layer(s_intl_layer[i]));
  }
  text_layer_set_text_alignment(s_intl_layer[1], GTextAlignmentCenter);
  text_layer_set_text_alignment(s_intl_layer[2], GTextAlignmentRight);
  
  time_t temp = time(NULL);
  update_times_layers(localtime(&temp));
}

static GColor get_color(int hour) {
  if (hour >= 9 && hour < 18)  // office hours => green
    return GColorGreen;
  if (hour >= 8 && hour < 23)  // day => white
    return GColorWhite;
  return GColorBlack;
}

void update_times_layers(struct tm* tick_time) {
  // Write the time
  static char time_buffer[9];
  strftime(time_buffer, sizeof(time_buffer), "%R", tick_time);
  text_layer_set_text(s_time_layer, time_buffer);
  
  // Write the date
  static char date_buffer[15];
  strftime(date_buffer, sizeof(date_buffer), "%F %a", tick_time);
  text_layer_set_text(s_date_layer, date_buffer);  

  // Write the international times
  time_t gmt = mktime(tick_time);
  static char intl_buffer[NUM_INTL][9];
  for (int i = 0; i < NUM_INTL; ++i) {
    time_t local_time = gmt + s_intl_offsets[i] * SECONDS_PER_HOUR;
    struct tm* local_tm = gmtime(&local_time);
    
    strftime(intl_buffer[i], sizeof(intl_buffer[i]), s_intl_places[i], local_tm);
    text_layer_set_text(s_intl_layer[i], intl_buffer[i]);
    text_layer_set_text_color(s_intl_layer[i], get_color(local_tm->tm_hour));
  }
}

void destroy_times_layers() {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  for (int i = 0; i < NUM_INTL; ++i) {
    text_layer_destroy(s_intl_layer[i]);
  }
}
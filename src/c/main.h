#pragma once

#include <pebble.h>

#define SETTINGS_KEY 1

typedef struct Settings {
  int offsetTimesTen[3];
} Settings;


void format_layer(TextLayer* layer, const char* font, GTextAlignment alignment);


void create_battery_layers(Layer* window_layer);
void destroy_battery_layers();


void create_health_layers(Layer* window_layer);
void update_health_layers(HealthEventType event, void* context);
void destroy_health_layers();


void create_times_layers(Layer* window_layer);
void update_times_layers(struct tm* tick_time, Settings* settings);
void destroy_times_layers();


void create_train_layers(Layer* window_layer);
void update_train_layers(const char* text);
void destroy_train_layers();
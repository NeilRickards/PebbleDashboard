#pragma once

void create_health_layers(Layer* window_layer);
void update_health_layers(HealthEventType event, void *context);
void destroy_health_layers();
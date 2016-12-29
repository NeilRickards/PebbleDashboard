#pragma once

void create_times_layers(Layer* window_layer);
void update_times_layers(struct tm* tick_time);
//void update_times_offsets(int offset0);
void destroy_times_layers();
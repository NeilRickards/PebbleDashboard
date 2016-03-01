#include <pebble.h>
#include "train.h"

void create_train_layers(Layer* window_layer) {
  
}

void update_train_layers(struct tm* tick_time) {
  if (tick_time->tm_min % 10 > 0) return;
}

void destroy_train_layers(){
  
}
/*
time
http://www.cplusplus.com/reference/ctime/strftime/
http://developer.getpebble.com/getting-started/watchface-tutorial/part2/

Font
http://developer.getpebble.com/guides/pebble-apps/display-and-animations/ux-fonts/
https://gist.github.com/sarfata/aec9c06ba9d66d159ed4
*/
	
#include <pebble.h>
#include <stdlib.h>
#include <pebble_fonts.h> 
#define SIZE 256

static Window *s_main_window;

static BitmapLayer *s_background_layer;	
static TextLayer *s_command_layer;
static TextLayer *s_time_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[SIZE];
	
  strftime (buffer, SIZE, "%a, %b %d. %H:%M:%S\n", tick_time);
  //strftime(buffer, sizeof(), "%s", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
//  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_background_color(s_background_layer, GColorBlack);
//  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  // Create console TextLayer
  s_command_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_background_color(s_command_layer, GColorClear);
  text_layer_set_text_color(s_command_layer, GColorWhite);
  text_layer_set_text(s_command_layer, "$ date '+%a, %b %d. %X'");

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 10, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_command_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_command_layer, GTextAlignmentLeft);

  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
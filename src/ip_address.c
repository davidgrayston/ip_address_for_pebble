#include <pebble.h>

Window *window;
TextLayer *text_layer;

#define apidata 0

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char ip_buffer[16];

  // Read tuples for data
  Tuple *ip_tuple = dict_find(iterator, apidata);

  // If all data is available, use it
  if(ip_tuple) {
    snprintf(ip_buffer, sizeof(ip_buffer), "%s", ip_tuple->value->cstring);
  }

  text_layer_set_text(text_layer, strcat("\n\nYour IP: ", ip_buffer));
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void handle_init(void) {
	// Create a window and text layer
	window = window_create();
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);
	text_layer = text_layer_create(bounds);
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "\n\nHi, just fetching your IP address...");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

  // Add the TextLayer and ScrollLayer to the view heirachy
  ScrollLayer *scroll_layer = scroll_layer_create(bounds);
  scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));
  layer_add_child(root_layer, scroll_layer_get_layer(scroll_layer));

  // Set the ScrollLayer's content size to the total size of the text
  scroll_layer_set_content_size(scroll_layer, text_layer_get_content_size(text_layer));

  // Enable TextLayer text flow and paging with inset size 2px
  text_layer_enable_screen_text_flow_and_paging(text_layer, 2);

  // Enable ScrollLayer paging
  scroll_layer_set_paging(scroll_layer, true);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

	// Push the window
	window_stack_push(window, true);

	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}

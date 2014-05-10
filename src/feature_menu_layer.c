#include "pebble.h"

//Ducati Azione
//Michael Buthut
	
static AppSync sync;
static uint8_t sync_buffer[32];

enum DUCATI_KEY {
  DUCATI_RIDE_MODE_KEY = 0x0,         
  DUCATI_GRIPS_KEY = 0x1, 
  DUCATI_ABS_KEY = 0x2,
  DUCATI_PROFILES_KEY = 0x3
};


static char *RIDE_MODES[] = {
	"Sport",
	"Urban",
	"Touring",
	"Enduro"
};

static char *GRIP[] = {
	"OFF",
	"ON"
};

static char *ABS[] = {
	"OFF",
	"ON"
};

static char *PROFILES[] = {
	"Commute",
	"Race",
	"2 up"
};
	
//Main Screen *************************************************************************************
#define NUM_FIRST_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 3
	
Window* window;

static SimpleMenuLayer *simple_main_menu_layer;

static SimpleMenuSection first_menu_sections[NUM_FIRST_MENU_SECTIONS];

static SimpleMenuItem first_menu_items[NUM_FIRST_MENU_ITEMS];

//*************************************************************************************************

//Settings Screen *********************************************************************************
#define NUM_SETTINGS_MENU_SECTIONS 2
#define NUM_SETTINGS_MENU_ITEMS 3
	
Window* settings;

static SimpleMenuLayer *simple_menu_layer_settings;

static SimpleMenuSection settings_menu_sections[NUM_SETTINGS_MENU_SECTIONS];

static SimpleMenuItem settings_menu_items[NUM_SETTINGS_MENU_ITEMS];

//************************************************************************************************

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case DUCATI_RIDE_MODE_KEY:
	    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_RIDE_MODE_KEY");
 	
		settings_menu_items[0].subtitle = (new_tuple->value->cstring);

  		layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));

      break;

    case DUCATI_GRIPS_KEY:
   	    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_GRIPS_KEY");
 	
		settings_menu_items[1].subtitle = (new_tuple->value->cstring);

  		layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));

      break;
	  
	case DUCATI_ABS_KEY:
      	APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_ABS_KEY");
 	
		settings_menu_items[2].subtitle = (new_tuple->value->cstring);

  		layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));

     break;
}
	
}

static void menu_select_callback_menu_item_4 (){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Riding Modes Clicked.");
	
  settings_menu_items[0].subtitle = RIDE_MODES[0];

  layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));

}

static void menu_select_callback_menu_item_5 (){
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Heated Grips Clicked.");

  settings_menu_items[0].subtitle = GRIP[0];

  layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
}

static void menu_select_callback_menu_item_6 (){
   APP_LOG(APP_LOG_LEVEL_DEBUG, "ABS Clicked.");
	
	  settings_menu_items[0].subtitle = ABS[0];

  layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
}

void settings_unload()
{
	app_sync_deinit(&sync);
	simple_menu_layer_destroy(simple_menu_layer_settings);
}


static void settings_load() {
	
	int num_a_items = 0;

  // This is an example of how you'd set a simple menu item
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
    .title = "Riding Modes",
      .subtitle = RIDE_MODES[0],
      .callback = menu_select_callback_menu_item_4,
  };
  // The menu items appear in the order saved in the menu items array
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Heated Grips",
      .subtitle = GRIP[0],
      .callback = menu_select_callback_menu_item_5,
  };
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "ABS",
      .subtitle = ABS[0],
      .callback = menu_select_callback_menu_item_6,
  };

  // Bind the menu items to the corresponding menu sections
  settings_menu_sections[0] = (SimpleMenuSection){
    .title = "Settings",
      .num_items = NUM_SETTINGS_MENU_ITEMS,
      .items = settings_menu_items,
  };

  Layer *window_layer = window_get_root_layer(settings);
  GRect bounds = layer_get_frame(window_layer);

  simple_menu_layer_settings = simple_menu_layer_create(bounds, settings, settings_menu_sections, NUM_SETTINGS_MENU_SECTIONS, NULL);
	
  Tuplet initial_values[] = {
   TupletCString(DUCATI_RIDE_MODE_KEY, RIDE_MODES[0] ),
   TupletCString(DUCATI_GRIPS_KEY, GRIP[0]),
   TupletCString(DUCATI_ABS_KEY, ABS[0]),  
   TupletCString(DUCATI_PROFILES_KEY, PROFILES[0])   
 };
	
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);


  layer_add_child(window_layer, simple_menu_layer_get_layer(simple_menu_layer_settings));
	
 // window_stack_push(settings, true); // The back button will dismiss the current window, not close the app.  So just press back to go back to the master view.

}



// This initializes the menu upon window load
void main_load() {
	
	window = window_create();
			
	
	// Although we already defined NUM_FIRST_MENU_ITEMS, you can define
	// an int as such to easily change the order of menu items later
	int num_a_items = 0;

	first_menu_items[num_a_items++] = (SimpleMenuItem){
		.title = "Profiles",
			//  .callback = menu_select_callback_menu_item_1,
			// .icon = menu_icon_image1,
	};
	first_menu_items[num_a_items++] = (SimpleMenuItem){
		.title = "Statistics",
			//  .callback = menu_select_callback_menu_item_2,
			// .icon = menu_icon_image2,
	};
	first_menu_items[num_a_items++] = (SimpleMenuItem){
		.title = "Settings",
		//	.subtitle = "This has an icon",
//		    .callback = settings_load,
			//  .icon = menu_icon_image3,
	};

	// Bind the menu items to the corresponding menu sections
	first_menu_sections[0] = (SimpleMenuSection){
		.title = "Ducati Azione",
			.num_items = NUM_FIRST_MENU_ITEMS,
			.items = first_menu_items,
	};

	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	
	simple_main_menu_layer = simple_menu_layer_create(bounds, window, first_menu_sections, NUM_FIRST_MENU_SECTIONS, NULL);

	layer_add_child(window_layer, simple_menu_layer_get_layer(simple_main_menu_layer));
	window_stack_push(window, true);
}

void main_unload() {
	simple_menu_layer_destroy(simple_main_menu_layer);
	window_destroy(window);
}
	
static void init() {
  settings = window_create();

  window_set_window_handlers(settings, (WindowHandlers) {
    .load = settings_load,
    .unload = settings_unload
  });

  const int inbound_size = 64;
  const int outbound_size = 16;
  app_message_open(inbound_size, outbound_size);

  const bool animated = true;
  window_stack_push(settings, animated);
}

static void deinit() {
  window_destroy(settings);
}


int main(void) {
  init();
	
  app_event_loop();
	
  deinit();
}


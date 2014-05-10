#include "pebble.h"

//Ducati Azione
//Michael Buthut
	
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


void settings_unload()
{
	simple_menu_layer_destroy(simple_menu_layer_settings);
	window_destroy(settings);
}


static void settings_load() {
	
	settings = window_create();
	
	int num_a_items = 0;

  // This is an example of how you'd set a simple menu item
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
    .title = "Riding Modes",
      .subtitle = "Currently Sport",
    //  .callback = menu_select_callback_menu_item_4,
  };
  // The menu items appear in the order saved in the menu items array
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Heated Grips",
      .subtitle = "Currently OFF",
   //   .callback = menu_select_callback_menu_item_5,
  };
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "ABS",
      .subtitle = "Currently ON",
   //   .callback = menu_select_callback_menu_item_6,
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

  layer_add_child(window_layer, simple_menu_layer_get_layer(simple_menu_layer_settings));
	
  window_stack_push(settings, true); // The back button will dismiss the current window, not close the app.  So just press back to go back to the master view.



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


int main(void) {
	
	settings_load();
 	
	app_event_loop();
	
	settings_unload();

}


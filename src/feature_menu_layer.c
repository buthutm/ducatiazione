#include "pebble.h"

//Ducati Azione
//Michael Buthut

static AppSync sync;
static uint8_t sync_buffer[32];

enum DUCATI_KEY {
    DUCATI_RIDE_MODE_KEY = 0x0,
    DUCATI_GRIPS_KEY = 0x1,
    DUCATI_ABS_KEY = 0x2,
    DUCATI_LOW_BEAM_KEY = 0x3
    DUCATI_HIGH_BEAM_KEY = 0x4
    DUCATI_FLASHER_KEY = 0x5
    DUCATI_HONK_KEY = 0x6
    DUCATI_POWER_KEY = 0x7
    DUCATI_ENGINE_KEY = 0x8
    
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

static char *LOWBEAM[] = {
    "OFF",
    "ON"
};

static char *HIGHBEAM[] = {
    "OFF",
    "ON"
};

static char *FLASHER[] = {
    "OFF",
    "Left",
    "Right"
};

static char *HONK[] = {
    "OFF",
    "ON"
};

static char *ENGINE[] = {
    "OFF",
    "ON"
};

static char *POWER[] = {
    "OFF",
    "ON"
};

static int current_ride_mode = 0;

static int current_grip = 0;

static int current_abs = 0;

static int current_profile = 0;

static int current_lowbeam = 0;

static int current_highbeam = 0;

static int current_flasher = 0;

static int current_honk = 0;

static int current_engine = 0;

static int current_power = 0;

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
#define NUM_FUNCTIONS_MENU_ITEMS 6

Window* settings;

static SimpleMenuLayer *simple_menu_layer_settings;

static SimpleMenuSection settings_menu_sections[NUM_SETTINGS_MENU_SECTIONS];

static SimpleMenuItem settings_menu_items[NUM_SETTINGS_MENU_ITEMS];
static SimpleMenuItem function_menu_items[NUM_FUNCTIONS_MENU_ITEMS];

//************************************************************************************************

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
    switch (key) {
        case DUCATI_RIDE_MODE_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_RIDE_MODE_KEY");
        
        settings_menu_items[0].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_GRIPS_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_GRIPS_KEY");
        
        settings_menu_items[1].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_ABS_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_ABS_KEY");
        
        settings_menu_items[2].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_LOW_BEAM_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_LOW_BEAM_KEY");
        
        function_menu_items[0].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_HIGH_BEAM_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_HIGH_BEAM_KEY");
        
        function_menu_items[1].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_FLASHER_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_FLASHER_KEY");
        
        function_menu_items[2].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_HONK_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_HONK_KEY");
        
        function_menu_items[3].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_POWER_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_POWER_KEY");
        
        function_menu_items[4].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case DUCATI_ENGINE_KEY:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_ENGINE_KEY");
        
        function_menu_items[5].subtitle = (new_tuple-&gt;value-&gt;cstring);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
    }
    
}

static void setDictionaryWithClick(char* mode, char* grips, char* abs, char* profile, char* lowbeam, char* highbeam, char* flasher, char* honk, char* power, char* engine){
    Tuplet new_tuples[] = {
        TupletCString(DUCATI_RIDE_MODE_KEY, mode ),
        TupletCString(DUCATI_GRIPS_KEY, grips),
        TupletCString(DUCATI_ABS_KEY, abs),
        TupletCString(DUCATI_PROFILES_KEY, profile),
        TupletCString(DUCATI_LOW_BEAM_KEY, lowbeam),
        TupletCString(DUCATI_HIGH_BEAM_KEY, highbeam),
        TupletCString(DUCATI_FLASHER_KEY, flasher),
        TupletCString(DUCATI_HONK_KEY, honk,
        TupletCString(DUCATI_POWER_KEY, power),
        TupletCString(DUCATI_ENGINE_KEY, engine)
        
    };
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_RIDE_MODE_KEY: %s",mode);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_GRIPS_KEY: %s",grips);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_ABS_KEY: %s",abs);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_PROFILES_KEY: %s",profile);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_LOW_BEAM_KEY: %s",lowbeam);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_HIGH_BEAM_KEY: %s",highbeam);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_FLASHER_KEY: %s",flasher);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_HONK_KEY: %s",honk);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_POWER_KEY: %s",power);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DUCATI_ENGINE_KEY: %s",engine);
    
    app_sync_set(&sync, new_tuples, 3);
}

static void menu_select_callback_menu_item_4 (){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Riding Modes Clicked.");
    
    settings_menu_items[0].subtitle = RIDE_MODES[3];
    
    layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
    
    switch(current_ride_mode){
        case 0:
        
        settings_menu_items[0].subtitle = RIDE_MODES[1];
        current_ride_mode=1;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case 1:
        
        settings_menu_items[0].subtitle = RIDE_MODES[2];
        current_ride_mode=2;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case 2:
        
        settings_menu_items[0].subtitle = RIDE_MODES[3];
        current_ride_mode=3;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case 3:
        
        settings_menu_items[0].subtitle = RIDE_MODES[0];
        current_ride_mode=0;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
    }
    
}

static void menu_select_callback_menu_item_5 (){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Heated Grips Clicked.");
    
    switch(current_grip){
        case 0:
        
        settings_menu_items[1].subtitle = GRIP[1];
        current_grip=1;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case 1:
        
        settings_menu_items[1].subtitle = GRIP[0];
        current_grip=0;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
    }
}

static void menu_select_callback_menu_item_6 (){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "ABS Clicked.");
    
    switch(current_abs){
        case 0:
        
        settings_menu_items[2].subtitle = ABS[1];
        current_abs=1;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
        
        case 1:
        
        settings_menu_items[2].subtitle = ABS[0];
        current_abs=0;
        setDictionaryWithClick(RIDE_MODES[current_ride_mode],GRIP[current_grip],ABS[current_abs],PROFILES[current_profile],LOWBEAM[current_lowbeam],HIGHBEAM[current_highbeam],FLASHER[current_flasher],HONK[current_honk],POWER[current_power],ENGINE[current_engine]);
        
        layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
        
        break;
    }
}



void settings_unload()
{
    app_sync_deinit(&sync);
    simple_menu_layer_destroy(simple_menu_layer_settings);
}


static void settings_load() {
    
    int num_a_items = 0;
    int num_b_items = 0;
    
    // This is an example of how you'd set a simple menu item
    settings_menu_items[num_a_items++] = (SimpleMenuItem){
        // You should give each menu item a title and callback
        .title = "Riding Modes",
        .subtitle = RIDE_MODES[current_ride_mode],
        .callback = menu_select_callback_menu_item_4,
    };
    // The menu items appear in the order saved in the menu items array
    settings_menu_items[num_a_items++] = (SimpleMenuItem){
        .title = "Heated Grips",
        .subtitle = GRIP[current_grip],
        .callback = menu_select_callback_menu_item_5,
    };
    settings_menu_items[num_a_items++] = (SimpleMenuItem){
        .title = "ABS",
        .subtitle = ABS[current_abs],
        .callback = menu_select_callback_menu_item_6,
    };
    
    function_menu_items[num_b_items++] = (SimpleMenuItem){
        .title = "Low Beam",
        .subtitle = ABS[current_lowbeam],
        // .callback = menu_select_callback_menu_item_7,
    };
    function_menu_items[num_b_items++] = (SimpleMenuItem){
        .title = "High Beam",
        .subtitle = ABS[current_highbeam],
        // .callback = menu_select_callback_menu_item_8,
    };
    function_menu_items[num_b_items++] = (SimpleMenuItem){
        .title = "Flasher",
        .subtitle = ABS[current_flasher],
        // .callback = menu_select_callback_menu_item_9,
    };
    function_menu_items[num_b_items++] = (SimpleMenuItem){
        .title = "Honk",
        .subtitle = ABS[current_honk],
        // .callback = menu_select_callback_menu_item_10,
    };
    function_menu_items[num_b_items++] = (SimpleMenuItem){
        .title = "Power",
        .subtitle = ABS[current_power],
        // .callback = menu_select_callback_menu_item_11,
    };
    function_menu_items[num_b_items++] = (SimpleMenuItem){
        .title = "Engine",
        .subtitle = ABS[current_engine],
        // .callback = menu_select_callback_menu_item_12,
    };
    
    // Bind the menu items to the corresponding menu sections
    settings_menu_sections[0] = (SimpleMenuSection){
        .title = "Settings",
        .num_items = NUM_SETTINGS_MENU_ITEMS,
        .items = settings_menu_items,
    };
    
    // Bind the menu items to the corresponding menu sections
    settings_menu_sections[1] = (SimpleMenuSection){
        .title = "Function Tests",
        .num_items = NUM_FUNCTIONS_MENU_ITEMS,
        .items = function_menu_items,
    };
    
    Layer *window_layer = window_get_root_layer(settings);
    GRect bounds = layer_get_frame(window_layer);
    
    Tuplet initial_values[] = {
        TupletCString(DUCATI_RIDE_MODE_KEY, RIDE_MODES[current_ride_mode] ),
        TupletCString(DUCATI_GRIPS_KEY, GRIP[current_grip]),
        TupletCString(DUCATI_ABS_KEY, ABS[current_abs]),
        TupletCString(DUCATI_PROFILES_KEY, PROFILES[current_profile]),
        TupletCString(DUCATI_LOW_BEAM_KEY, LOWBEAM[current_lowbeam]),
        TupletCString(DUCATI_HIGH_BEAM_KEY, HIGHBEAM[current_highbeam]),
        TupletCString(DUCATI_FLASHER_KEY, FLASHER[current_flahser]),
        TupletCString(DUCATI_HONK_KEY, HONK[current_honk]),
        TupletCString(DUCATI_POWER_KEY, POWER[current_power]),
        TupletCString(DUCATI_ENGINE_KEY, ENGINE[current_engine])
    };
    
    app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
    sync_tuple_changed_callback, sync_error_callback, NULL);
    
    simple_menu_layer_settings = simple_menu_layer_create(bounds, settings, settings_menu_sections, NUM_SETTINGS_MENU_SECTIONS, NULL);
    
    
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
        //    .subtitle = "This has an icon",
        //            .callback = settings_load,
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
    const int outbound_size = 64;
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

	
	 
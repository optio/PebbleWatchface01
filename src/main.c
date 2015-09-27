#include <pebble.h>
  
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_timeintext_layer;
static TextLayer *s_timeintext_row_02_layer;
static TextLayer *s_timeintext_row_03_layer;
static TextLayer *s_timeintext_row_04_layer;
static TextLayer *s_timeintext_row_05_layer;
static TextLayer *s_bluetooth_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_date_layer;
static TextLayer *s_world_layer;
static TextLayer *s_weather_layer;

static GFont s_time_font;
static GFont s_timeintext_font;
static GFont s_timeintext_row_02_font;
static GFont s_timeintext_row_03_font;
static GFont s_timeintext_row_04_font;
static GFont s_timeintext_row_05_font;
static GFont s_bluetooth_font;
static GFont s_battery_font;
static GFont s_date_font;
static GFont s_world_font;
static GFont s_weather_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  // Manila = UTC+08:00

  // Create a long-lived buffer
  static char buffer[] = "00:00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 24h hour format
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00:00"), "%I:%M", tick_time);
  }
  
  
  // TODO: CONVERT TO subscribe
  // TODO: vibrate upon change
  static char buffer_bluetooth [100];
  if ((bluetooth_connection_service_peek() == true)) {
    strncpy(buffer_bluetooth, "", 100);
  } else {
    strncpy(buffer_bluetooth, "x", 100);
  }
  
  
  // http://developer.getpebble.com/docs/c/Foundation/Event_Service/BatteryStateService/
  // http://developer.getpebble.com/guides/pebble-apps/app-events/battery-service/
  
  static char buffer_battery [100];
  BatteryChargeState charge_state = battery_state_service_peek();
  if (charge_state.is_charging) {
    snprintf(buffer_battery, sizeof(buffer_battery), "charging");
  } else {
    snprintf(buffer_battery, sizeof(buffer_battery), "%d%% charged", charge_state.charge_percent);
  }
  
  //http://developer.getpebble.com/blog/2013/07/24/Using-Pebble-System-Fonts/
  
  static char buffer_date [100];
  strftime(buffer_date, sizeof("Saturday 31 September"), "%A %e %B", tick_time);
  
  static char buffer_world [100];
  time_t currentTime;
  tm * ptm;
  time ( &currentTime );
  ptm = tick_time; //gmtime ( &rawtime );
  ptm->tm_hour += 8;
  ptm->tm_min += 0;
  time_t shiftedTime = mktime( ptm );
  tm * pShiftedTm = gmtime( &shiftedTime );
  strftime(buffer_world, sizeof("00"), "%H", pShiftedTm);
  
  

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  
  int time_min = tick_time->tm_min;
  int time_hour = tick_time->tm_hour;
  
  static char buffer_timeintext [100];
  
  static char buffer_timeintext_row_02 [100];
  static char buffer_timeintext_row_03 [100];
  static char buffer_timeintext_row_04 [100];
  static char buffer_timeintext_row_05 [100];
  
  static char time_hour_word [100];
  strncpy(time_hour_word, "het uur", 100);
  
  static char time_hour_next_word [100];
  strncpy(time_hour_next_word, "volgend uur", 100);
  
  if ((time_hour == 0)) {
    strncpy(time_hour_word, "twaalf", 100);
    strncpy(time_hour_next_word, "een", 100);
  } else if ((time_hour == 1)) {
    strncpy(time_hour_word, "een", 100);
    strncpy(time_hour_next_word, "twee", 100);
  } else if ((time_hour == 2)) {
    strncpy(time_hour_word, "twee", 100);
    strncpy(time_hour_next_word, "drie", 100);
  } else if ((time_hour == 3)) {
    strncpy(time_hour_word, "drie", 100);
    strncpy(time_hour_next_word, "vier", 100);
  } else if ((time_hour == 4)) {
    strncpy(time_hour_word, "vier", 100);
    strncpy(time_hour_next_word, "vijf", 100);
  } else if ((time_hour == 5)) {
    strncpy(time_hour_word, "vijf", 100);
    strncpy(time_hour_next_word, "zes", 100);
  } else if ((time_hour == 6)) {
    strncpy(time_hour_word, "zes", 100);
    strncpy(time_hour_next_word, "zeven", 100);
  } else if ((time_hour == 7)) {
    strncpy(time_hour_word, "zeven", 100);
    strncpy(time_hour_next_word, "acht", 100);
  } else if ((time_hour == 8)) {
    strncpy(time_hour_word, "acht", 100);
    strncpy(time_hour_next_word, "negen", 100);
  } else if ((time_hour == 9)) {
    strncpy(time_hour_word, "negen", 100);
    strncpy(time_hour_next_word, "tien", 100);
  } else if ((time_hour == 10)) {
    strncpy(time_hour_word, "tien", 100);
    strncpy(time_hour_next_word, "elf", 100);
  } else if ((time_hour == 11)) {
    strncpy(time_hour_word, "elf", 100);
    strncpy(time_hour_next_word, "twaalf", 100);
  } else if ((time_hour == 12)) {
    strncpy(time_hour_word, "twaalf", 100);
    strncpy(time_hour_next_word, "een", 100);
  } else if ((time_hour == 13)) {
    strncpy(time_hour_word, "een", 100);
    strncpy(time_hour_next_word, "twee", 100);
  } else if ((time_hour == 14)) {
    strncpy(time_hour_word, "twee", 100);
    strncpy(time_hour_next_word, "drie", 100);
  } else if ((time_hour == 15)) {
    strncpy(time_hour_word, "drie", 100);
    strncpy(time_hour_next_word, "vier", 100);
  } else if ((time_hour == 16)) {
    strncpy(time_hour_word, "vier", 100);
    strncpy(time_hour_next_word, "vijf", 100);
  } else if ((time_hour == 17)) {
    strncpy(time_hour_word, "vijf", 100);
    strncpy(time_hour_next_word, "zes", 100);
  } else if ((time_hour == 18)) {
    strncpy(time_hour_word, "zes", 100);
    strncpy(time_hour_next_word, "zeven", 100);
  } else if ((time_hour == 19)) {
    strncpy(time_hour_word, "zeven", 100);
    strncpy(time_hour_next_word, "acht", 100);
  } else if ((time_hour == 20)) {
    strncpy(time_hour_word, "acht", 100);
    strncpy(time_hour_next_word, "negen", 100);
  } else if ((time_hour == 21)) {
    strncpy(time_hour_word, "negen", 100);
    strncpy(time_hour_next_word, "tien", 100);
  } else if ((time_hour == 22)) {
    strncpy(time_hour_word, "tien", 100);
    strncpy(time_hour_next_word, "elf", 100);
  } else if ((time_hour == 23)) {
    strncpy(time_hour_word, "elf", 100);
    strncpy(time_hour_next_word, "twaalf", 100);
  } else if ((time_hour == 24)) {
    strncpy(time_hour_word, "twaalf", 100);
    strncpy(time_hour_next_word, "een", 100);
  }
  
  //snprintf(buffer_timeintext, sizeof(buffer_timeintext), "%d", tick_time->tm_min);
  
  //http://forums.getpebble.com/discussion/5508/new-to-c-accessing-the-time-data-as-an-integer
  
  //layer_set_hidden(LAYERNAME, true);
  
  // display TimeInText
  if ((tick_time->tm_min >= 0 && tick_time->tm_min <= 2)) {
    // op het uur
    if ((time_hour > 23)) {
      strncpy(buffer_timeintext, "", 100);
      strncpy(buffer_timeintext_row_02, "Midder-\nnacht", 100);
      strncpy(buffer_timeintext_row_03, "", 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
    } else {
      strncpy(buffer_timeintext, time_hour_word, 100); 
      strncpy(buffer_timeintext_row_02, "", 100);
      strncpy(buffer_timeintext_row_03, "uur", 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
    }
  } else if ((time_min >= 58 && time_min <= 62)) {
    // op het uur
    if ((time_hour > 23)) {
      strncpy(buffer_timeintext, "", 100);
      strncpy(buffer_timeintext_row_02, "Midder-\nnacht", 100);
      strncpy(buffer_timeintext_row_03, "", 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
    } else {
      strncpy(buffer_timeintext, "", 100); 
      strncpy(buffer_timeintext_row_02, "", 100);
      strncpy(buffer_timeintext_row_03, "", 100);
      strncpy(buffer_timeintext_row_04, time_hour_word, 100);
      strncpy(buffer_timeintext_row_05, "uur", 100);
    }
  } else if ((time_min >= 3 && time_min <= 7)) {
    // 5 over
    strncpy(buffer_timeintext, "vijf", 100);
    strncpy(buffer_timeintext_row_02, "over", 100);
    strncpy(buffer_timeintext_row_03, time_hour_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
  } else if ((time_min >= 8 && time_min <= 12)) {
    // 10 over
    strncpy(buffer_timeintext, "tien", 100);
    strncpy(buffer_timeintext_row_02, "over", 100);
    strncpy(buffer_timeintext_row_03, time_hour_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
  } else if ((time_min >= 13 && time_min <= 17)) {
    // kwart over
    strncpy(buffer_timeintext, "kwart", 100);
    strncpy(buffer_timeintext_row_02, "over", 100);
    strncpy(buffer_timeintext_row_03, time_hour_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);    
  } else if ((time_min >= 18 && time_min <= 22)) {
    // 20 over
    strncpy(buffer_timeintext, "twintig", 100);
    strncpy(buffer_timeintext_row_02, "over", 100);
    strncpy(buffer_timeintext_row_03, time_hour_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);    
  } else if ((time_min >= 23 && time_min <= 27)) {
    // 5 voor half
    strncpy(buffer_timeintext, "vijf", 100);
    strncpy(buffer_timeintext_row_02, "voor half", 100);
    strncpy(buffer_timeintext_row_03, time_hour_next_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
  } else if ((time_min >= 28 && time_min <= 32)) {
    // half
    strncpy(buffer_timeintext, "", 100);
    strncpy(buffer_timeintext_row_02, "", 100);
    strncpy(buffer_timeintext_row_03, "", 100);
      strncpy(buffer_timeintext_row_04, "half", 100);
      strncpy(buffer_timeintext_row_05, time_hour_next_word, 100);
  } else if ((time_min >= 33 && time_min <= 37)) {
    // 5 over half
    strncpy(buffer_timeintext, "vijf", 100);
    strncpy(buffer_timeintext_row_02, "over half", 100);
    strncpy(buffer_timeintext_row_03, time_hour_next_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
  }  else if ((time_min >= 38 && time_min <= 42)) {
    // 20 voor
    strncpy(buffer_timeintext, "twintig", 100);
    strncpy(buffer_timeintext_row_02, "voor", 100);
    strncpy(buffer_timeintext_row_03, time_hour_next_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
  } else if ((time_min >= 43 && time_min <= 47)) {
    // kwart voor
    strncpy(buffer_timeintext, "kwart", 100);
    strncpy(buffer_timeintext_row_02, "voor", 100);
    strncpy(buffer_timeintext_row_03, time_hour_next_word, 100);
  } else if ((time_min >= 48 && time_min <= 52)) {
    // 10 voor
    strncpy(buffer_timeintext, "tien", 100);
    strncpy(buffer_timeintext_row_02, "voor", 100);
    strncpy(buffer_timeintext_row_03, time_hour_next_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
  } else if ((time_min >= 53 && time_min <= 57)) {
    // 5 voor
    strncpy(buffer_timeintext, "vijf", 100);
    strncpy(buffer_timeintext_row_02, "voor", 100);
    strncpy(buffer_timeintext_row_03, time_hour_next_word, 100);
      strncpy(buffer_timeintext_row_04, "", 100);
      strncpy(buffer_timeintext_row_05, "", 100);
  } else {
    // error
    strncpy(buffer_timeintext, "error_row_01", 100);
    strncpy(buffer_timeintext_row_02, "error_row_02", 100);
    strncpy(buffer_timeintext_row_03, "error_row_03", 100);
      strncpy(buffer_timeintext_row_04, "error_row_04", 100);
      strncpy(buffer_timeintext_row_05, "error_row_05", 100);
    //snprintf(buffer_timeintext, sizeof(buffer_timeintext), "%d", tick_time->tm_min);
    //snprintf(buffer_timeintext, sizeof(buffer_timeintext), "%d", time_min);
  }
  
  text_layer_set_text(s_timeintext_layer, buffer_timeintext);
  text_layer_set_text(s_timeintext_row_02_layer, buffer_timeintext_row_02);
  text_layer_set_text(s_timeintext_row_03_layer, buffer_timeintext_row_03);
  text_layer_set_text(s_timeintext_row_04_layer, buffer_timeintext_row_04);
  text_layer_set_text(s_timeintext_row_05_layer, buffer_timeintext_row_05);
  text_layer_set_text(s_bluetooth_layer, buffer_bluetooth);
  text_layer_set_text(s_battery_layer, buffer_battery);
  text_layer_set_text(s_date_layer, buffer_date);
  text_layer_set_text(s_world_layer, buffer_world);
  
}


static void process_temperature() {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
}

static void main_window_load(Window *window) {
  // 144 x 168
  // http://www.cplusplus.com/reference/ctime/strftime/
  // http://developer.getpebble.com/docs/c/Graphics/Graphics_Types/Color_Definitions/
  
  //Create GBitmap, then set to created BitmapLayer
  /*
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  */
  
  // Create time TextLayer
  //s_time_layer = text_layer_create(GRect(0, 52, 139, 50));
  s_time_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorInchworm);
  text_layer_set_text(s_time_layer, "00:00:00");
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Add TimeInText layer ROW 01 (BIG)
  s_timeintext_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_32));
  s_timeintext_layer = text_layer_create(GRect(0,28,144,168));
  text_layer_set_background_color(s_timeintext_layer, GColorClear);
  text_layer_set_text_color(s_timeintext_layer, GColorInchworm);
  text_layer_set_text(s_timeintext_layer, "ROW 01");
  text_layer_set_font(s_timeintext_layer, s_timeintext_font);
  text_layer_set_text_alignment(s_timeintext_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeintext_layer));
  
  // Add TimeInText layer ROW 02 (SMALL)
  s_timeintext_row_02_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
  s_timeintext_row_02_layer = text_layer_create(GRect(0,65,144,168));
  text_layer_set_background_color(s_timeintext_row_02_layer, GColorClear);
  text_layer_set_text_color(s_timeintext_row_02_layer, GColorInchworm);
  text_layer_set_text(s_timeintext_row_02_layer, "ROW 02");
  text_layer_set_font(s_timeintext_row_02_layer, s_timeintext_row_02_font);
  text_layer_set_text_alignment(s_timeintext_row_02_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeintext_row_02_layer));
  
  // Add TimeInText layer ROW 03 (BIG)
  s_timeintext_row_03_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_32));
  s_timeintext_row_03_layer = text_layer_create(GRect(0,85,144,168));
  text_layer_set_background_color(s_timeintext_row_03_layer, GColorClear);
  text_layer_set_text_color(s_timeintext_row_03_layer, GColorInchworm);
  text_layer_set_text(s_timeintext_row_03_layer, "ROW 03");
  text_layer_set_font(s_timeintext_row_03_layer, s_timeintext_row_03_font);
  text_layer_set_text_alignment(s_timeintext_row_03_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeintext_row_03_layer));
  
  // Add TimeInText layer ROW 04 (BIG)
  s_timeintext_row_04_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_32));
  s_timeintext_row_04_layer = text_layer_create(GRect(0,40,144,168));
  text_layer_set_background_color(s_timeintext_row_04_layer, GColorClear);
  text_layer_set_text_color(s_timeintext_row_04_layer, GColorInchworm);
  text_layer_set_text(s_timeintext_row_04_layer, "ROW 04");
  text_layer_set_font(s_timeintext_row_04_layer, s_timeintext_row_04_font);
  text_layer_set_text_alignment(s_timeintext_row_04_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeintext_row_04_layer));
  
  // Add TimeInText layer ROW 05 (BIG)
  s_timeintext_row_05_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_32));
  s_timeintext_row_05_layer = text_layer_create(GRect(0,75,144,168));
  text_layer_set_background_color(s_timeintext_row_05_layer, GColorClear);
  text_layer_set_text_color(s_timeintext_row_05_layer, GColorInchworm);
  text_layer_set_text(s_timeintext_row_05_layer, "ROW 05");
  text_layer_set_font(s_timeintext_row_05_layer, s_timeintext_row_05_font);
  text_layer_set_text_alignment(s_timeintext_row_05_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeintext_row_05_layer));
  
  
  // Add bluetooth layer
  s_bluetooth_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_12));
  s_bluetooth_layer = text_layer_create(GRect(0,115,144,168));
  text_layer_set_background_color(s_bluetooth_layer, GColorClear);
  text_layer_set_text_color(s_bluetooth_layer, GColorInchworm);
  text_layer_set_text(s_bluetooth_layer, "Bleutooth");
  text_layer_set_font(s_bluetooth_layer, s_bluetooth_font);
  text_layer_set_text_alignment(s_bluetooth_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bluetooth_layer));
  
  // Add battery layer
  s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_12));
  s_battery_layer = text_layer_create(GRect(0,125,144,168));
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorInchworm);
  text_layer_set_text(s_battery_layer, "Battery");
  text_layer_set_font(s_battery_font, s_bluetooth_font);
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
  
  // Add date layer
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_12));
  s_date_layer = text_layer_create(GRect(0,135,144,168));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorInchworm);
  text_layer_set_text(s_date_layer, "Date");
  text_layer_set_font(s_date_font, s_bluetooth_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  
  // Add world layer
  s_world_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_12));
  s_world_layer = text_layer_create(GRect(0,146,144,168));
  text_layer_set_background_color(s_world_layer, GColorClear);
  text_layer_set_text_color(s_world_layer, GColorInchworm);
  text_layer_set_text(s_world_layer, "World");
  text_layer_set_font(s_world_font, s_bluetooth_font);
  text_layer_set_text_alignment(s_world_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_world_layer));
    
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(GRect(0, 115, 144, 25));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorInchworm);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "Loading...");
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_12));
  text_layer_set_font(s_weather_layer, s_weather_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  //Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_timeintext_row_02_font);
  fonts_unload_custom_font(s_timeintext_row_03_font);
  fonts_unload_custom_font(s_bluetooth_font);
  fonts_unload_custom_font(s_battery_font);
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_world_font);
  
  //Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  
  // Destroy TimeInTextLayer
  text_layer_destroy(s_timeintext_layer);
  text_layer_destroy(s_timeintext_row_02_layer);
  text_layer_destroy(s_timeintext_row_03_layer);
  text_layer_destroy(s_bluetooth_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_world_layer);
  
  // Destroy weather elements
  text_layer_destroy(s_weather_layer);
  fonts_unload_custom_font(s_weather_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    process_temperature();
  }
}


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_TEMPERATURE:
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
      break;
    case KEY_CONDITIONS:
      snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  // Assemble full string and display
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
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
  
  // Set BG black
  window_set_background_color(s_main_window, GColorBlack);  
  
  // Register with TickTimerService
  //tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  //Process temperature to avoid loading upon reload
  //process_temperature();
}

static void deinit() {
  //tick_timer_service_unsubscribe();
  //battery_state_service_unsubscribe();
  //bluetooth_connection_service_unsubscribe();
  //layer_remove_from_parent()
  //layer_destroy(LAYERNAME)
  //text_layer_destroy(LAYERNAME)
  //gbitmap_destroy()
  //fonts_unload_custom_font()
  
  
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
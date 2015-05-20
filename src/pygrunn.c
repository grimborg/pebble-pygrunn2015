#include "pebble.h"
#include "schedule.h"
#include "show_session.h"

#define NUM_ROOMS 4

static Window *wnd_schedule;
static MenuLayer *menu_schedule;
static GBitmap *room_icons[NUM_ROOMS + 1];

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return schedule.num_slots;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  TimeSlot *ts = &schedule.slots[section_index];
  if (ts->is_common) {
      return 1;
  } else {
      return ts->num_sessions;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static int16_t menu_get_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  TimeSlot *ts = &schedule.slots[cell_index->section];
  if (ts->is_common) {
    return 20;
  } else {
    return 50;
  }
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  graphics_context_set_text_color(ctx, GColorBlack);
  menu_cell_basic_header_draw(ctx, cell_layer, schedule.slots[section_index].time);
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  TimeSlot *ts = &schedule.slots[cell_index->section];
      graphics_context_set_text_color(ctx, GColorBlack);
  if (ts->is_common) {
      graphics_draw_text(ctx, ts->title, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), layer_get_bounds(cell_layer), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  } else {
      GRect title_bounds = layer_get_bounds(cell_layer);
      title_bounds.origin.x += 30;
      title_bounds.size.w -= 30;
      GRect icon_bounds = layer_get_bounds(cell_layer);
      icon_bounds.origin.x += 3;
      icon_bounds.origin.y += 3;
      icon_bounds.size.w = 24;
      icon_bounds.size.h = 28;
      Session *s = &(ts->sessions[cell_index->row]); 
        graphics_draw_text(ctx, s->title, fonts_get_system_font(FONT_KEY_GOTHIC_14), title_bounds, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
        graphics_draw_bitmap_in_rect(ctx, room_icons[s->room], icon_bounds);
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  TimeSlot *ts = &schedule.slots[cell_index->section];
  if (!ts->is_common) {
    Session *s = &(ts->sessions[cell_index->row]); 
    show_session(s);
  }
}

static void main_window_load(Window *window) {
  room_icons[1] = gbitmap_create_with_resource(RESOURCE_ID_ICON_ROOM_1);
  room_icons[2] = gbitmap_create_with_resource(RESOURCE_ID_ICON_ROOM_2);
  room_icons[3] = gbitmap_create_with_resource(RESOURCE_ID_ICON_ROOM_3);
  room_icons[4] = gbitmap_create_with_resource(RESOURCE_ID_ICON_ROOM_4);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  menu_schedule = menu_layer_create(bounds);
  menu_layer_set_callbacks(menu_schedule, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .get_cell_height = menu_get_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(menu_schedule, window);

  layer_add_child(window_layer, menu_layer_get_layer(menu_schedule));
}

static void main_window_unload(Window *window) {
  menu_layer_destroy(menu_schedule);

  for (int i = 1; i < NUM_ROOMS + 1; i++) {
    gbitmap_destroy(room_icons[i]);
  }
}

static void init() {
  init_session_window();
  init_schedule();
  wnd_schedule = window_create();
  window_set_window_handlers(wnd_schedule, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(wnd_schedule, true);
}

static void deinit() {
  window_destroy(wnd_schedule);
  deinit_session_window();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

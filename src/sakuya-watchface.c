#include <pebble.h>
#include "calendar-component.h"
#include "date-component.h"
#include "time-component.h"

static Window *s_main_window;

static TimeComponent *s_time_component;
static DateComponent *s_date_component;
static CalendarComponent *s_calendar_component;

// Each Layer holds its respective component
static Layer *s_time_component_layer;
static Layer *s_date_component_layer;
static Layer *s_calendar_component_layer;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    if (units_changed & MINUTE_UNIT) {
        time_component_on_minute_tick(s_time_component, tick_time, units_changed);
    }
    if (units_changed & DAY_UNIT) {
        date_component_on_day_tick(s_date_component, tick_time, units_changed);
        calendar_component_on_day_tick(s_calendar_component, tick_time, units_changed);
    }
}

static void main_window_load(Window *window) {
    Layer *root_window_layer = window_get_root_layer(window);
    s_time_component_layer = layer_create(GRect(0, 16, 144, 84));
    s_time_component = time_component_create(s_time_component_layer);
    layer_add_child(root_window_layer, s_time_component_layer);

    s_date_component_layer = layer_create(GRect(0, 84, 144, 26));
    s_date_component = date_component_create(s_date_component_layer);
    layer_add_child(root_window_layer, s_date_component_layer);

    s_calendar_component_layer = layer_create(GRect(0, 84 + 26, 144, 168 - 84 - 26));
    s_calendar_component = calendar_component_create(s_calendar_component_layer);
    layer_add_child(root_window_layer, s_calendar_component_layer);

    tick_timer_service_subscribe(MINUTE_UNIT | HOUR_UNIT, tick_handler);
}

static void main_window_unload(Window *window) {
    time_component_destroy(s_time_component);
    date_component_destroy(s_date_component);
    calendar_component_destroy(s_calendar_component);
}

static void init() {
    s_main_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
    //update_date();
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

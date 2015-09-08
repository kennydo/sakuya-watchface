#include <pebble.h>
#define SAMPLE_TIME_STRING "00:00"

typedef struct {
    TextLayer *text_layer;
    char buffer[sizeof(SAMPLE_TIME_STRING)];
} TimeComponent;

void time_component_on_minute_tick(TimeComponent* component, struct tm *tick_time, TimeUnits units_changed) {
    if(clock_is_24h_style() == true) {
        // Use 24 hour format
        strftime(component->buffer, sizeof(SAMPLE_TIME_STRING), "%H:%M", tick_time);
    } else {
        // Use 12 hour format
        strftime(component->buffer, sizeof(SAMPLE_TIME_STRING), "%I:%M", tick_time);
    }
    text_layer_set_text(component->text_layer, component->buffer);
}

TimeComponent* time_component_create(Layer* parentLayer) {
    TimeComponent *component = malloc(sizeof(TimeComponent));

    if(component == NULL) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Could not allocate memory for TimeComponent");
        return NULL;
    }

    component->text_layer = text_layer_create(GRect(0, 0, 144, 84));
    text_layer_set_background_color(component->text_layer, GColorWhite);
    text_layer_set_text_color(component->text_layer, GColorClear);

    text_layer_set_font(component->text_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
    text_layer_set_text_alignment(component->text_layer, GTextAlignmentCenter);

    layer_add_child(parentLayer, text_layer_get_layer(component->text_layer));

    //component->buffer = malloc(sizeof(SAMPLE_TIME_STRING));
    strncpy(component->buffer, SAMPLE_TIME_STRING, sizeof(SAMPLE_TIME_STRING));

    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    time_component_on_minute_tick(component, tick_time, MINUTE_UNIT);

    return component;
}

void time_component_destroy(TimeComponent* component) {
    if(component == NULL){
        APP_LOG(APP_LOG_LEVEL_ERROR, "Aborting attempt to destroy a null TimeComponent");
        return;
    }

APP_LOG(APP_LOG_LEVEL_ERROR, "unloading text");
    text_layer_destroy(component->text_layer);
    free(component);
}

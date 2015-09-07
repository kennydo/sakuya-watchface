#include <pebble.h>
#define SAMPLE_DATE_STRING "Mon, Jan 31"

typedef struct {
    TextLayer *text_layer;
    TextLayer *background_layer;
    char* buffer;
} DateComponent;

void date_component_on_day_tick(DateComponent* component, struct tm *tick_time, TimeUnits units_changed) {
    strftime(component->buffer, sizeof(SAMPLE_DATE_STRING), "%a, %b %e", tick_time);

    text_layer_set_text(component->text_layer, component->buffer);
}

DateComponent* date_component_create(Layer* parentLayer) {
    DateComponent *component = malloc(sizeof(DateComponent));

    if(component == NULL) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Could not allocate memory for DateComponent");
        return NULL;
    }

    // First, set up the bright red background
    component->background_layer = text_layer_create(GRect(0, 0, 144, 26));
    text_layer_set_background_color(component->background_layer, GColorFolly);
    layer_add_child(parentLayer, text_layer_get_layer(component->background_layer));

    // Then, set up the text.
    // The text layer has a padding on its top, so shift it up by 4 pixels to make it line up.
    component->text_layer = text_layer_create(GRect(0, -4, 144, 30));
    text_layer_set_background_color(component->text_layer, GColorClear);
    text_layer_set_text_color(component->text_layer, GColorWhite);

    text_layer_set_font(component->text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(component->text_layer, GTextAlignmentCenter);

    layer_add_child(parentLayer, text_layer_get_layer(component->text_layer));

    component->buffer = malloc(sizeof(SAMPLE_DATE_STRING));
    component->buffer = SAMPLE_DATE_STRING;

    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    date_component_on_day_tick(component, tick_time, DAY_UNIT);

    return component;
}

void date_component_destroy(DateComponent* component) {
    if(component == NULL){
        APP_LOG(APP_LOG_LEVEL_ERROR, "Aborting attempt to destroy a null DateComponent");
        return;
    }

    text_layer_destroy(component->text_layer);
    text_layer_destroy(component->background_layer);
    free(component->buffer);

    free(component);
}

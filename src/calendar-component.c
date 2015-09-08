#include <pebble.h>

typedef struct {
    TextLayer *background_layer;
} CalendarComponent;

void calendar_component_on_day_tick(CalendarComponent* component, struct tm *tick_time, TimeUnits units_changed) {
    return;
}

CalendarComponent* calendar_component_create(Layer* parentLayer) {
    CalendarComponent *component = malloc(sizeof(CalendarComponent));

    if(component == NULL) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Could not allocate memory for CalendarComponent");
        return NULL;
    }

    component->background_layer = text_layer_create(GRect(0, 0, 144, 168 - 84 - 26));
    text_layer_set_background_color(component->background_layer, GColorBlueMoon);
    layer_add_child(parentLayer, text_layer_get_layer(component->background_layer));

    return component;
}

void calendar_component_destroy(CalendarComponent* component) {
    if(component == NULL){
        APP_LOG(APP_LOG_LEVEL_ERROR, "Aborting attempt to destroy a null CalendarComponent");
        return;
    }

    text_layer_destroy(component->background_layer);
    free(component);
}

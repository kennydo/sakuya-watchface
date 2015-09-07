typedef struct {
    TextLayer *text_layer;
    TextLayer *background_layer;
    char* buffer;
} DateComponent;

void date_component_on_day_tick(DateComponent* component, struct tm *tick_time, TimeUnits units_changed);

DateComponent* date_component_create(Layer* parentLayer);

void date_component_destroy(DateComponent* component);

typedef struct {
    TextLayer *text_layer;
    char* buffer;
} TimeComponent;

TimeComponent* time_component_create(Layer* parentLayer);

void time_component_destroy(TimeComponent* component);

void time_component_on_minute_tick(TimeComponent* component, struct tm *tick_time, TimeUnits units_changed);

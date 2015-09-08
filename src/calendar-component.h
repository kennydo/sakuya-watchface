typedef struct {
    TextLayer *background_layer;
} CalendarComponent;

void calendar_component_on_day_tick(CalendarComponent* component, struct tm *tick_time, TimeUnits units_changed);

CalendarComponent* calendar_component_create(Layer* parentLayer);

void calendar_component_destroy(CalendarComponent* component);

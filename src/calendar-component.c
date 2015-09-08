#include <pebble.h>
#define DAY_TEXT_COLOR GColorWhite
#define DAY_BG_COLOR GColorClear

typedef struct {
    TextLayer *background_layer;
    TextLayer *today_highlight_text_layer;
    TextLayer *weekday_header_text_layers[7];
    TextLayer *days_text_layers[2 * 7];
    char days_text_buffers[2 * 7][3];
} CalendarComponent;

const char weekday_headers[7][3] = {
    "Su",
    "Mo",
    "Tu",
    "We",
    "Th",
    "Fr",
    "Sa"
};

static int days_per_month[12] = {
    31, // Jan
    28, // Feb, does not take into account leap years yet
    31, // Mar
    30, // Apr
    31, // May
    30, // Jun
    31, // Jul
    31, // Aug
    30, // Sep
    31, // Oct
    30, // Nov
    31  // Dec
};

bool is_leap_year(int year){
    if (year % 4 != 0) {
        return false;
    } else if (year % 400 == 0){
        return true;
    } else if (year % 100 == 0) {
        return false;
    }
    return true;
};

void calendar_component_on_day_tick(CalendarComponent* component, struct tm *tick_time, TimeUnits units_changed) {
    int current_day_of_week = tick_time->tm_wday;

    if (is_leap_year(tick_time->tm_year)) {
        days_per_month[1] = 29;
    } else {
        days_per_month[1] = 28;
    }

    int current_day_in_month = tick_time->tm_mday;
    int number_of_days_in_current_month = days_per_month[tick_time->tm_mon];

    // Set the highlight box in the right area
    int highlight_x = -1 + 21 * current_day_of_week;
    int highlight_y = -4 + 20 + 3;
    layer_set_frame(text_layer_get_layer(component->today_highlight_text_layer), GRect(highlight_x, highlight_y, 19, 19));


    int cur_month = tick_time->tm_mon;
    int cur_date = current_day_in_month;

    // First, handle today and future days
    for(int i=current_day_of_week; i<14; i++){
        snprintf(component->days_text_buffers[i], sizeof("00"), "%d", cur_date);
        text_layer_set_text(component->days_text_layers[i], component->days_text_buffers[i]);

        cur_date++;
        if (cur_date > days_per_month[cur_month]) {
            cur_date = 1;
            cur_month = (cur_month + 1) % 12;
        }
    }

    // Reset the cursors to today.
    cur_month = tick_time->tm_mon;
    cur_date = current_day_in_month;

    // Now, handle days before today
    for(int i=current_day_of_week - 1; i>=0; i--){
        // Note that for this for loop, we decrement the cursor before
        // printing the number out, unlike the initial loop for today/future days.
        cur_date--;
        if(cur_date == 0){
            cur_month = (cur_month - 1) % 12;
            cur_date = days_per_month[cur_month];
        }
        snprintf(component->days_text_buffers[i], sizeof("00"), "%d", cur_date);
        text_layer_set_text(component->days_text_layers[i], component->days_text_buffers[i]);
    }
}

CalendarComponent* calendar_component_create(Layer* parentLayer) {
    CalendarComponent *component = malloc(sizeof(CalendarComponent));

    if(component == NULL) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Could not allocate memory for CalendarComponent");
        return NULL;
    }

    component->background_layer = text_layer_create(GRect(0, 0, 144, 58));
    text_layer_set_background_color(component->background_layer, GColorBlueMoon);
    layer_add_child(parentLayer, text_layer_get_layer(component->background_layer));

    // Set up the weekday headers
    for(int i=0; i<7; i++){
        component->weekday_header_text_layers[i] = text_layer_create(GRect(-1 + 21 * i, -4, 20, 20));

        text_layer_set_text_color(component->weekday_header_text_layers[i], GColorWhite);
        text_layer_set_background_color(component->weekday_header_text_layers[i], GColorClear);

        text_layer_set_font(component->weekday_header_text_layers[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
        text_layer_set_text_alignment(component->weekday_header_text_layers[i], GTextAlignmentCenter);

        text_layer_set_text(component->weekday_header_text_layers[i], weekday_headers[i]);

        layer_add_child(parentLayer, text_layer_get_layer(component->weekday_header_text_layers[i]));
    }

    component->today_highlight_text_layer = text_layer_create(GRect(0, 0, 20, 20));
    text_layer_set_background_color(component->today_highlight_text_layer, GColorFolly);
    layer_add_child(parentLayer, text_layer_get_layer(component->today_highlight_text_layer));

    TextLayer *cur_text_layer;
    // Set up the day cells
    for(int row=0; row<2; row++){
        for(int i=0; i<7; i++){
            strncpy(component->days_text_buffers[row * 7 + i], "00", sizeof("00"));

            cur_text_layer = text_layer_create(GRect(-1 + 21 * i, -4 + 20 * (row + 1), 20, 20));
            text_layer_set_font(cur_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
            text_layer_set_text_alignment(cur_text_layer, GTextAlignmentCenter);

            text_layer_set_text_color(cur_text_layer, DAY_TEXT_COLOR);
            text_layer_set_background_color(cur_text_layer, DAY_BG_COLOR);

            layer_add_child(parentLayer, text_layer_get_layer(cur_text_layer));

            component->days_text_layers[row * 7 + i] = cur_text_layer;
        }
    }

    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    calendar_component_on_day_tick(component, tick_time, DAY_UNIT);

    return component;
}

void calendar_component_destroy(CalendarComponent* component) {
    if(component == NULL){
        APP_LOG(APP_LOG_LEVEL_ERROR, "Aborting attempt to destroy a null CalendarComponent");
        return;
    }

    text_layer_destroy(component->background_layer);
    for(int i=0; i<7; i++){
        text_layer_destroy(component->weekday_header_text_layers[i]);
    }
    for(int i=0; i<14; i++){
        text_layer_destroy(component->days_text_layers[i]);
    }
    free(component);
}

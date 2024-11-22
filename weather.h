#ifndef WEATHER_H
#define WEATHER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cJSON.h"

// Struct for Weather Data
struct WeatherData {
    char location_name[100];
    double temperature;
    double humidity;
    char condition[100];
};

// Function prototypes
void fetch_weather_data(const char *location, const char *api_key, struct WeatherData *data, char **raw_data);
void write_raw_data(const char *raw_data);
void write_processed_data(const struct WeatherData *data);
void display_weather_data(const struct WeatherData *data);

#endif // WEATHER_H

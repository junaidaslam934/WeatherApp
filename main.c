#include "weather.h"

int main() {
    char location[100];
    const char *api_key = "3a69e50f28f648afbce180111240211"; // Your API key
    struct WeatherData data;
    char *raw_weather_data = NULL;

    printf("Enter location (City Name): ");
    fgets(location, sizeof(location), stdin);
    location[strcspn(location, "\n")] = 0;

    if (strlen(location) == 0) {
        printf("Error: Location cannot be empty.\n");
        return 1;
    }

    fetch_weather_data(location, api_key, &data, &raw_weather_data);

    if (raw_weather_data) {
        write_raw_data(raw_weather_data);
        write_processed_data(&data);
        display_weather_data(&data);
        free(raw_weather_data);
    } else {
        printf("Error: Failed to fetch weather data.\n");
    }

    return 0;
}


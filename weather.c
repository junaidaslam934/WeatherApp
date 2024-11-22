#include "weather.h"

// Function to fetch weather data using the WeatherAPI
void fetch_weather_data(const char *location, const char *api_key, struct WeatherData *data, char **raw_data) {
    char url[512];
    snprintf(url, sizeof(url), "https://api.weatherapi.com/v1/current.json?key=%s&q=%s", api_key, location);

    char command[600];
    snprintf(command, sizeof(command), "curl -s \"%s\" > weather_response.json", url);
    int ret = system(command);

    if (ret != 0) {
        printf("Error: Failed to fetch data from the API.\n");
        return;
    }

    FILE *file = fopen("weather_response.json", "r");
    if (!file) {
        printf("Error: Could not open the weather_response.json file.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    *raw_data = malloc(file_size + 1);
    if (*raw_data == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return;
    }

    fread(*raw_data, 1, file_size, file);
    (*raw_data)[file_size] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(*raw_data);
    if (!json) {
        printf("Error: Failed to parse the JSON response.\n");
        free(*raw_data);
        return;
    }

    cJSON *location_obj = cJSON_GetObjectItem(json, "location");
    cJSON *current_obj = cJSON_GetObjectItem(json, "current");

    if (location_obj && current_obj) {
        strcpy(data->location_name, cJSON_GetObjectItem(location_obj, "name")->valuestring);
        data->temperature = cJSON_GetObjectItem(current_obj, "temp_c")->valuedouble;
        data->humidity = cJSON_GetObjectItem(current_obj, "humidity")->valuedouble;
        strcpy(data->condition, cJSON_GetObjectItem(cJSON_GetObjectItem(current_obj, "condition"), "text")->valuestring);
    } else {
        printf("Error: Invalid JSON structure.\n");
    }

    cJSON_Delete(json);
}

// Function to write raw data to a file
void write_raw_data(const char *raw_data) {
    FILE *file = fopen("weather_raw.json", "a");
    if (!file) {
        printf("Error: Unable to save raw data.\n");
        return;
    }
    fprintf(file, "%s\n", raw_data);
    fclose(file);
}

// Function to write processed data to a file
// Function to write processed data to a file
void write_processed_data(const struct WeatherData *data) {
    FILE *file = fopen("weather_processed.json", "a");  // Change "w" to "a"
    if (!file) {
        printf("Error: Unable to save processed data.\n");
        return;
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "location_name", data->location_name);
    cJSON_AddNumberToObject(json, "temperature", data->temperature);
    cJSON_AddNumberToObject(json, "humidity", data->humidity);
    cJSON_AddStringToObject(json, "condition", data->condition);

    fprintf(file, "%s\n", cJSON_Print(json));
    cJSON_Delete(json);

    fclose(file);
}


// Function to display processed data
void display_weather_data(const struct WeatherData *data) {
    printf("Weather Data:\n");
    printf("Location: %s\n", data->location_name);
    printf("Temperature: %.2f°C\n", data->temperature);
    printf("Humidity: %.2f%%\n", data->humidity);
    printf("Condition: %s\n", data->condition);
}

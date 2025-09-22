#include "sensor.h"
#include "../../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static bool sensor_initialized = false;
static bool sensor_running = false;
static sensor_mode_t current_mode = SENSOR_MODE_SINGLE;
static uint32_t sample_count = 0;

static uint32_t get_timestamp_ms(void) {
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }
    return (uint32_t)(time(NULL) * 1000);
}

static float generate_temperature(void) {
    static float base_temp = 25.0f;
    float noise = ((float)rand() / RAND_MAX - 0.5f) * 4.0f;
    float drift = sin((float)sample_count * 0.1f) * 5.0f;

    float temp = base_temp + noise + drift;

    if (temp > MAX_TEMP_CELSIUS) temp = MAX_TEMP_CELSIUS;
    if (temp < MIN_TEMP_CELSIUS) temp = MIN_TEMP_CELSIUS;

    return temp;
}

static float generate_humidity(void) {
    float base_humidity = 45.0f;
    float noise = ((float)rand() / RAND_MAX - 0.5f) * 10.0f;
    float seasonal_drift = sin((float)sample_count * 0.05f) * 15.0f;

    float humidity = base_humidity + noise + seasonal_drift;

    if (humidity > 100.0f) humidity = 100.0f;
    if (humidity < 0.0f) humidity = 0.0f;

    return humidity;
}

int sensor_init(void) {
    sensor_initialized = true;
    sensor_running = false;
    sample_count = 0;

    DEBUG_PRINT("Temperature/Humidity sensor initialized");
    printf("[SENSOR] Sensor initialized\n");
    return 0;
}

int sensor_start(sensor_mode_t mode) {
    if (!sensor_initialized) {
        return -1;
    }

    current_mode = mode;
    sensor_running = true;

    DEBUG_PRINT("Sensor started in %s mode",
                (mode == SENSOR_MODE_SINGLE) ? "single" : "continuous");
    printf("[SENSOR] Started in %s mode\n",
           (mode == SENSOR_MODE_SINGLE) ? "single-shot" : "continuous");
    return 0;
}

int sensor_read(sensor_reading_t *reading) {
    if (!sensor_initialized || !sensor_running || reading == NULL) {
        return -1;
    }

    reading->temperature_celsius = generate_temperature();
    reading->humidity_percent = generate_humidity();
    reading->timestamp_ms = get_timestamp_ms();
    reading->valid = true;

    sample_count++;

    DEBUG_PRINT("Sensor reading: %.2f°C, %.1f%% RH",
                reading->temperature_celsius, reading->humidity_percent);

    printf("[SENSOR] T: %.2f°C, H: %.1f%% RH\n",
           reading->temperature_celsius, reading->humidity_percent);

    return 0;
}

int sensor_calibrate(void) {
    if (!sensor_initialized) {
        return -1;
    }

    printf("[SENSOR] Calibrating sensor...\n");

    for (volatile int i = 0; i < 1000000; i++) {

    }

    DEBUG_PRINT("Sensor calibration completed");
    printf("[SENSOR] Calibration complete\n");
    return 0;
}

bool sensor_is_ready(void) {
    return sensor_initialized && sensor_running;
}

void sensor_stop(void) {
    sensor_running = false;
    DEBUG_PRINT("Sensor stopped");
    printf("[SENSOR] Sensor stopped\n");
}

void sensor_deinit(void) {
    sensor_stop();
    sensor_initialized = false;
    sample_count = 0;
    DEBUG_PRINT("Sensor deinitialized");
    printf("[SENSOR] Sensor deinitialized\n");
}
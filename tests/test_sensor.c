#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "drivers/sensor.h"

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s\n", message); \
            return 1; \
        } else { \
            printf("PASS: %s\n", message); \
        } \
    } while(0)

int main(void) {
    printf("=== Sensor Tests ===\n");

    TEST_ASSERT(sensor_init() == 0, "Sensor initialization");
    TEST_ASSERT(sensor_is_ready() == false, "Sensor not ready before start");
    TEST_ASSERT(sensor_start(SENSOR_MODE_SINGLE) == 0, "Sensor start single mode");
    TEST_ASSERT(sensor_is_ready() == true, "Sensor ready after start");

    sensor_reading_t reading;
    TEST_ASSERT(sensor_read(&reading) == 0, "Sensor read");
    TEST_ASSERT(reading.valid == true, "Sensor reading valid");
    TEST_ASSERT(reading.temperature_celsius >= MIN_TEMP_CELSIUS &&
                reading.temperature_celsius <= MAX_TEMP_CELSIUS, "Temperature in range");
    TEST_ASSERT(reading.humidity_percent >= 0.0f &&
                reading.humidity_percent <= 100.0f, "Humidity in range");

    TEST_ASSERT(sensor_calibrate() == 0, "Sensor calibration");

    sensor_stop();
    TEST_ASSERT(sensor_is_ready() == false, "Sensor not ready after stop");

    sensor_deinit();

    TEST_ASSERT(sensor_read(NULL) != 0, "Sensor read with NULL pointer");

    printf("All sensor tests passed!\n");
    return 0;
}
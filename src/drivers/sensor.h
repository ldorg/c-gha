#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float temperature_celsius;
    float humidity_percent;
    uint32_t timestamp_ms;
    bool valid;
} sensor_reading_t;

typedef enum {
    SENSOR_MODE_SINGLE,
    SENSOR_MODE_CONTINUOUS
} sensor_mode_t;

int sensor_init(void);
int sensor_start(sensor_mode_t mode);
int sensor_read(sensor_reading_t *reading);
int sensor_calibrate(void);
bool sensor_is_ready(void);
void sensor_stop(void);
void sensor_deinit(void);

#endif
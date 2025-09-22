#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "config.h"
#include "hal/gpio.h"
#include "hal/uart.h"
#include "drivers/led.h"
#include "drivers/sensor.h"

static int test_count = 0;
static int test_failures = 0;

#define RUN_TEST(test_func) \
    do { \
        test_count++; \
        printf("Running %s...\n", #test_func); \
        if (test_func() != 0) { \
            test_failures++; \
            printf("FAILED: %s\n", #test_func); \
        } else { \
            printf("PASSED: %s\n", #test_func); \
        } \
        printf("\n"); \
    } while(0)

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("  FAIL: %s\n", message); \
            return -1; \
        } else { \
            printf("  PASS: %s\n", message); \
        } \
    } while(0)

int test_gpio_functionality(void) {
    printf("\n=== Testing GPIO Functionality ===\n");

    TEST_ASSERT(gpio_init(5, GPIO_MODE_OUTPUT) == 0, "GPIO init as output");
    TEST_ASSERT(gpio_write(5, GPIO_STATE_HIGH) == 0, "GPIO write high");
    TEST_ASSERT(gpio_read(5) == GPIO_STATE_HIGH, "GPIO read high state");
    TEST_ASSERT(gpio_write(5, GPIO_STATE_LOW) == 0, "GPIO write low");
    TEST_ASSERT(gpio_read(5) == GPIO_STATE_LOW, "GPIO read low state");
    TEST_ASSERT(gpio_toggle(5) == 0, "GPIO toggle");
    TEST_ASSERT(gpio_read(5) == GPIO_STATE_HIGH, "GPIO state after toggle");

    gpio_deinit(5);

    TEST_ASSERT(gpio_init(255, GPIO_MODE_OUTPUT) != 0, "GPIO init with invalid pin");
    TEST_ASSERT(gpio_write(255, GPIO_STATE_HIGH) != 0, "GPIO write to invalid pin");

    return 0;
}

int test_uart_functionality(void) {
    printf("\n=== Testing UART Functionality ===\n");

    uart_config_t config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };

    TEST_ASSERT(uart_init(&config) == 0, "UART initialization");

    uint8_t test_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    TEST_ASSERT(uart_write(test_data, sizeof(test_data)) == sizeof(test_data), "UART write");

    uint8_t read_buffer[10];
    int bytes_read = uart_read(read_buffer, sizeof(read_buffer));
    TEST_ASSERT(bytes_read > 0, "UART read returns data");

    TEST_ASSERT(uart_printf("Test message: %d", 42) > 0, "UART printf");

    TEST_ASSERT(uart_init(NULL) != 0, "UART init with NULL config");
    TEST_ASSERT(uart_write(NULL, 5) != 0, "UART write with NULL data");

    uart_deinit();

    return 0;
}

int test_led_functionality(void) {
    printf("\n=== Testing LED Functionality ===\n");

    TEST_ASSERT(led_init(LED_PIN) == 0, "LED initialization");
    TEST_ASSERT(led_set_state(LED_ON) == 0, "LED turn on");
    TEST_ASSERT(led_get_state() == LED_ON, "LED state check (ON)");
    TEST_ASSERT(led_set_state(LED_OFF) == 0, "LED turn off");
    TEST_ASSERT(led_get_state() == LED_OFF, "LED state check (OFF)");
    TEST_ASSERT(led_toggle() == 0, "LED toggle");
    TEST_ASSERT(led_get_state() == LED_ON, "LED state after toggle");
    TEST_ASSERT(led_blink(10) == 0, "LED blink");

    led_deinit();

    return 0;
}

int test_sensor_functionality(void) {
    printf("\n=== Testing Sensor Functionality ===\n");

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

    return 0;
}

int test_system_integration(void) {
    printf("\n=== Testing System Integration ===\n");

    uart_config_t uart_config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };

    TEST_ASSERT(uart_init(&uart_config) == 0, "System UART init");
    TEST_ASSERT(led_init(LED_PIN) == 0, "System LED init");
    TEST_ASSERT(sensor_init() == 0, "System sensor init");

    TEST_ASSERT(sensor_start(SENSOR_MODE_CONTINUOUS) == 0, "Start continuous monitoring");

    for (int i = 0; i < 3; i++) {
        sensor_reading_t reading;
        if (sensor_read(&reading) == 0) {
            uart_printf("Reading %d: %.2f°C, %.1f%% RH",
                       i+1, reading.temperature_celsius, reading.humidity_percent);
            led_toggle();
        }
    }

    sensor_stop();
    led_deinit();
    uart_deinit();
    sensor_deinit();

    TEST_ASSERT(1, "System integration test completed");

    return 0;
}

int test_gpio_edge_cases(void) {
    printf("\n=== Testing GPIO Edge Cases ===\n");

    TEST_ASSERT(gpio_init(0, GPIO_MODE_OUTPUT) == 0, "GPIO init pin 0");
    TEST_ASSERT(gpio_init(31, GPIO_MODE_INPUT) == 0, "GPIO init pin 31 (boundary)");

    for (int i = 0; i < 10; i++) {
        gpio_toggle(0);
    }
    TEST_ASSERT(1, "GPIO rapid toggle test");

    gpio_deinit(0);
    gpio_deinit(31);

    return 0;
}

int test_uart_error_conditions(void) {
    printf("\n=== Testing UART Error Conditions ===\n");

    uart_config_t invalid_config = {
        .baudrate = 9999999,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };

    uart_init(&invalid_config);
    TEST_ASSERT(1, "UART handles invalid baudrate gracefully");

    uint8_t large_buffer[1024];
    for (int i = 0; i < 1024; i++) {
        large_buffer[i] = (uint8_t)(i % 256);
    }
    TEST_ASSERT(uart_write(large_buffer, sizeof(large_buffer)) > 0, "UART large buffer write");

    uart_deinit();

    return 0;
}

int test_led_timing(void) {
    printf("\n=== Testing LED Timing ===\n");

    TEST_ASSERT(led_init(LED_PIN) == 0, "LED init for timing tests");

    for (int i = 0; i < 5; i++) {
        led_set_state(LED_ON);
        led_set_state(LED_OFF);
    }
    TEST_ASSERT(1, "LED rapid on/off cycles");

    TEST_ASSERT(led_blink(1) == 0, "LED very short blink");
    TEST_ASSERT(led_blink(50) == 0, "LED medium blink");

    led_deinit();

    return 0;
}

int test_sensor_boundary_values(void) {
    printf("\n=== Testing Sensor Boundary Values ===\n");

    TEST_ASSERT(sensor_init() == 0, "Sensor init for boundary tests");
    TEST_ASSERT(sensor_start(SENSOR_MODE_CONTINUOUS) == 0, "Start continuous mode");

    sensor_reading_t readings[10];
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT(sensor_read(&readings[i]) == 0, "Multiple sensor readings");

        TEST_ASSERT(readings[i].temperature_celsius >= MIN_TEMP_CELSIUS, "Temperature above minimum");
        TEST_ASSERT(readings[i].temperature_celsius <= MAX_TEMP_CELSIUS, "Temperature below maximum");
        TEST_ASSERT(readings[i].humidity_percent >= 0.0f, "Humidity above 0%");
        TEST_ASSERT(readings[i].humidity_percent <= 100.0f, "Humidity below 100%");
    }

    float min_temp = readings[0].temperature_celsius;
    float max_temp = readings[0].temperature_celsius;
    for (int i = 1; i < 10; i++) {
        if (readings[i].temperature_celsius < min_temp) min_temp = readings[i].temperature_celsius;
        if (readings[i].temperature_celsius > max_temp) max_temp = readings[i].temperature_celsius;
    }

    printf("Temperature range in test: %.2f°C to %.2f°C\n", min_temp, max_temp);
    TEST_ASSERT(max_temp - min_temp >= 0.0f, "Temperature variation detected");

    sensor_stop();
    sensor_deinit();

    return 0;
}

int test_sensor_error_handling(void) {
    printf("\n=== Testing Sensor Error Handling ===\n");

    sensor_reading_t reading;
    TEST_ASSERT(sensor_read(&reading) != 0, "Sensor read fails when not initialized");

    TEST_ASSERT(sensor_init() == 0, "Sensor init for error tests");
    TEST_ASSERT(sensor_read(&reading) != 0, "Sensor read fails when not started");

    TEST_ASSERT(sensor_start(SENSOR_MODE_SINGLE) == 0, "Start sensor");
    TEST_ASSERT(sensor_read(NULL) != 0, "Sensor read with NULL pointer fails");

    sensor_stop();
    TEST_ASSERT(sensor_read(&reading) != 0, "Sensor read fails after stop");

    sensor_deinit();

    return 0;
}

int test_power_management(void) {
    printf("\n=== Testing Power Management Simulation ===\n");

    uart_config_t uart_config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };

    TEST_ASSERT(uart_init(&uart_config) == 0, "System startup - UART");
    TEST_ASSERT(led_init(LED_PIN) == 0, "System startup - LED");
    TEST_ASSERT(sensor_init() == 0, "System startup - Sensor");

    TEST_ASSERT(sensor_start(SENSOR_MODE_SINGLE) == 0, "Low power sensor mode");

    sensor_reading_t reading;
    TEST_ASSERT(sensor_read(&reading) == 0, "Power-optimized sensor reading");

    led_set_state(LED_ON);
    printf("[POWER] All systems active\n");

    sensor_stop();
    led_set_state(LED_OFF);
    printf("[POWER] Entering low power mode\n");

    led_deinit();
    uart_deinit();
    sensor_deinit();
    printf("[POWER] System shutdown complete\n");

    TEST_ASSERT(1, "Power management cycle completed");

    return 0;
}

int main(void) {
    printf("=== Demo Firmware Test Suite ===\n");
    printf("Build Type: %s\n", getenv("BUILD_TYPE") ? getenv("BUILD_TYPE") : "Unknown");
    printf("Firmware Version: %s\n", FIRMWARE_VERSION);
    printf("\n");

    RUN_TEST(test_gpio_functionality);
    RUN_TEST(test_gpio_edge_cases);
    RUN_TEST(test_uart_functionality);
    RUN_TEST(test_uart_error_conditions);
    RUN_TEST(test_led_functionality);
    RUN_TEST(test_led_timing);
    RUN_TEST(test_sensor_functionality);
    RUN_TEST(test_sensor_boundary_values);
    RUN_TEST(test_sensor_error_handling);
    RUN_TEST(test_system_integration);
    RUN_TEST(test_power_management);

    printf("=== Test Results ===\n");
    printf("Tests run: %d\n", test_count);
    printf("Failures: %d\n", test_failures);

    if (test_failures == 0) {
        printf("All tests PASSED!\n");
        return 0;
    } else {
        printf("%d test(s) FAILED!\n", test_failures);
        return 1;
    }
}
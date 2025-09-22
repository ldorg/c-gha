#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#include "../include/config.h"
#include "hal/uart.h"
#include "hal/gpio.h"
#include "drivers/led.h"
#include "drivers/sensor.h"

static volatile bool running = true;

void signal_handler(int sig) {
    (void)sig;
    running = false;
    printf("\nShutdown signal received...\n");
}

void system_init(void) {
    uart_config_t uart_config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };

    printf("=== %s ===\n", BOARD_NAME);
    printf("Firmware Version: %s\n\n", FIRMWARE_VERSION);

    if (uart_init(&uart_config) != 0) {
        printf("ERROR: Failed to initialize UART\n");
        exit(1);
    }

    if (led_init(LED_PIN) != 0) {
        printf("ERROR: Failed to initialize LED\n");
        exit(1);
    }

    if (sensor_init() != 0) {
        printf("ERROR: Failed to initialize sensor\n");
        exit(1);
    }

    uart_printf("System initialization complete");
    printf("[SYSTEM] All peripherals initialized successfully\n");
}

void system_deinit(void) {
    printf("\n[SYSTEM] Shutting down...\n");

    sensor_deinit();
    led_deinit();
    uart_deinit();

    printf("[SYSTEM] Shutdown complete\n");
}

void run_self_test(void) {
    printf("\n[SYSTEM] Running self-test...\n");

    uart_printf("Starting self-test sequence");

    printf("[TEST] LED test...\n");
    led_blink(100);
    usleep(100000);
    led_blink(100);

    printf("[TEST] Sensor calibration...\n");
    if (sensor_calibrate() != 0) {
        printf("[ERROR] Sensor calibration failed\n");
        return;
    }

    printf("[TEST] Sensor reading test...\n");
    if (sensor_start(SENSOR_MODE_SINGLE) == 0) {
        sensor_reading_t reading;
        if (sensor_read(&reading) == 0) {
            uart_printf("Test reading: %.2f°C, %.1f%% RH",
                       reading.temperature_celsius, reading.humidity_percent);
        }
        sensor_stop();
    }

    uart_printf("Self-test completed successfully");
    printf("[SYSTEM] Self-test passed\n\n");
}

void main_loop(void) {
    uint32_t led_last_toggle = 0;
    uint32_t sensor_last_read = 0;
    uint32_t loop_count = 0;

    if (sensor_start(SENSOR_MODE_CONTINUOUS) != 0) {
        printf("ERROR: Failed to start sensor\n");
        return;
    }

    uart_printf("Entering main application loop");
    printf("[SYSTEM] Main loop started\n");

    while (running) {
        loop_count++;

        if (loop_count % (LED_BLINK_RATE_MS / 10) == 0) {
            led_toggle();
            led_last_toggle = loop_count;
        }

        if (loop_count % (SENSOR_SAMPLE_RATE_MS / 10) == 0) {
            sensor_reading_t reading;
            if (sensor_read(&reading) == 0) {
                uart_printf("Temp: %.2f°C, Humidity: %.1f%%",
                           reading.temperature_celsius, reading.humidity_percent);

                if (reading.temperature_celsius > 80.0f) {
                    uart_printf("WARNING: High temperature detected!");
                    printf("[ALERT] Temperature too high: %.2f°C\n", reading.temperature_celsius);
                }
            }
            sensor_last_read = loop_count;
        }

        if (loop_count % 5000 == 0) {
            uart_printf("System alive - uptime: %u cycles", loop_count);
        }

        usleep(10000);
    }

    sensor_stop();
    printf("[SYSTEM] Main loop ended\n");
}

int main(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("Starting demo firmware...\n");

    system_init();
    run_self_test();
    main_loop();
    system_deinit();

    printf("Firmware terminated gracefully\n");
    return 0;
}
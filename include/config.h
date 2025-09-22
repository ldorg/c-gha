#ifndef CONFIG_H
#define CONFIG_H

#define FIRMWARE_VERSION "1.0.0"
#define BOARD_NAME "Demo Board v1.0"

#define LED_PIN 13
#define UART_BAUDRATE 115200

#define SENSOR_SAMPLE_RATE_MS 1000
#define LED_BLINK_RATE_MS 500

#define MAX_TEMP_CELSIUS 85
#define MIN_TEMP_CELSIUS -40

#define ENABLE_DEBUG_OUTPUT 1

#if ENABLE_DEBUG_OUTPUT
    #define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...)
#endif

#endif
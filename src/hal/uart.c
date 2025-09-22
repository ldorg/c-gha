#include "uart.h"
#include "../../include/config.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

static uart_config_t uart_current_config;
static bool uart_initialized = false;

int uart_init(const uart_config_t *config) {
    if (config == NULL) {
        return -1;
    }

    uart_current_config = *config;
    uart_initialized = true;

    DEBUG_PRINT("UART initialized: %d baud, %d data bits, %d stop bits, parity %d",
                config->baudrate, config->data_bits, config->stop_bits, config->parity);

    printf("[UART] Interface initialized at %d baud\n", config->baudrate);
    return 0;
}

int uart_write(const uint8_t *data, size_t length) {
    if (!uart_initialized || data == NULL) {
        return -1;
    }

    printf("[UART TX] ");
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

    return (int)length;
}

int uart_read(uint8_t *buffer, size_t max_length) {
    if (!uart_initialized || buffer == NULL) {
        return -1;
    }

    static uint8_t dummy_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    size_t bytes_to_copy = (max_length < sizeof(dummy_data)) ? max_length : sizeof(dummy_data);

    memcpy(buffer, dummy_data, bytes_to_copy);

    printf("[UART RX] Received %zu bytes: ", bytes_to_copy);
    for (size_t i = 0; i < bytes_to_copy; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    return (int)bytes_to_copy;
}

int uart_printf(const char *format, ...) {
    if (!uart_initialized || format == NULL) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    printf("[UART] ");
    int result = vprintf(format, args);
    printf("\n");

    va_end(args);
    return result;
}

void uart_deinit(void) {
    uart_initialized = false;
    DEBUG_PRINT("UART deinitialized");
    printf("[UART] Interface closed\n");
}
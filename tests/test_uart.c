#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "hal/uart.h"

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
    printf("=== UART Tests ===\n");

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

    printf("All UART tests passed!\n");
    return 0;
}
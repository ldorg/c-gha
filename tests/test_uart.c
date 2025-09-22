#include "unity.h"
#include "config.h"
#include "hal/uart.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_uart_initialization(void) {
    uart_config_t config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };
    TEST_ASSERT_EQUAL(0, uart_init(&config));
    uart_deinit();
}

void test_uart_write(void) {
    uart_config_t config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };
    uart_init(&config);

    uint8_t test_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    TEST_ASSERT_EQUAL(sizeof(test_data), uart_write(test_data, sizeof(test_data)));
    uart_deinit();
}

void test_uart_read_returns_data(void) {
    uart_config_t config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };
    uart_init(&config);

    uint8_t read_buffer[10];
    int bytes_read = uart_read(read_buffer, sizeof(read_buffer));
    TEST_ASSERT_GREATER_THAN(0, bytes_read);
    uart_deinit();
}

void test_uart_printf(void) {
    uart_config_t config = {
        .baudrate = UART_BAUDRATE_115200,
        .parity = UART_PARITY_NONE,
        .data_bits = 8,
        .stop_bits = 1
    };
    uart_init(&config);

    TEST_ASSERT_GREATER_THAN(0, uart_printf("Test message: %d", 42));
    uart_deinit();
}

void test_uart_init_with_null_config(void) {
    TEST_ASSERT_NOT_EQUAL(0, uart_init(NULL));
}

void test_uart_write_with_null_data(void) {
    TEST_ASSERT_NOT_EQUAL(0, uart_write(NULL, 5));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_uart_initialization);
    RUN_TEST(test_uart_write);
    RUN_TEST(test_uart_read_returns_data);
    RUN_TEST(test_uart_printf);
    RUN_TEST(test_uart_init_with_null_config);
    RUN_TEST(test_uart_write_with_null_data);

    return UNITY_END();
}
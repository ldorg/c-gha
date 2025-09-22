#include "unity.h"
#include "config.h"
#include "hal/gpio.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_gpio_init_as_output(void) {
    TEST_ASSERT_EQUAL(0, gpio_init(5, GPIO_MODE_OUTPUT));
}

void test_gpio_write_high(void) {
    gpio_init(5, GPIO_MODE_OUTPUT);
    TEST_ASSERT_EQUAL(0, gpio_write(5, GPIO_STATE_HIGH));
}

void test_gpio_read_high_state(void) {
    gpio_init(5, GPIO_MODE_OUTPUT);
    gpio_write(5, GPIO_STATE_HIGH);
    TEST_ASSERT_EQUAL(GPIO_STATE_HIGH, gpio_read(5));
}

void test_gpio_write_low(void) {
    gpio_init(5, GPIO_MODE_OUTPUT);
    TEST_ASSERT_EQUAL(0, gpio_write(5, GPIO_STATE_LOW));
}

void test_gpio_read_low_state(void) {
    gpio_init(5, GPIO_MODE_OUTPUT);
    gpio_write(5, GPIO_STATE_LOW);
    TEST_ASSERT_EQUAL(GPIO_STATE_LOW, gpio_read(5));
}

void test_gpio_toggle(void) {
    gpio_init(5, GPIO_MODE_OUTPUT);
    gpio_write(5, GPIO_STATE_LOW);
    TEST_ASSERT_EQUAL(0, gpio_toggle(5));
}

void test_gpio_state_after_toggle(void) {
    gpio_init(5, GPIO_MODE_OUTPUT);
    gpio_write(5, GPIO_STATE_LOW);
    gpio_toggle(5);
    TEST_ASSERT_EQUAL(GPIO_STATE_HIGH, gpio_read(5));
    gpio_deinit(5);
}

void test_gpio_init_with_invalid_pin(void) {
    TEST_ASSERT_NOT_EQUAL(0, gpio_init(255, GPIO_MODE_OUTPUT));
}

void test_gpio_write_to_invalid_pin(void) {
    TEST_ASSERT_NOT_EQUAL(0, gpio_write(255, GPIO_STATE_HIGH));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_gpio_init_as_output);
    RUN_TEST(test_gpio_write_high);
    RUN_TEST(test_gpio_read_high_state);
    RUN_TEST(test_gpio_write_low);
    RUN_TEST(test_gpio_read_low_state);
    RUN_TEST(test_gpio_toggle);
    RUN_TEST(test_gpio_state_after_toggle);
    RUN_TEST(test_gpio_init_with_invalid_pin);
    RUN_TEST(test_gpio_write_to_invalid_pin);

    return UNITY_END();
}
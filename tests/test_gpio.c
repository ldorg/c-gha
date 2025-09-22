#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "hal/gpio.h"

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
    printf("=== GPIO Tests ===\n");

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

    printf("All GPIO tests passed!\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "hal/gpio.h"
#include "drivers/led.h"

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
    printf("=== LED Tests ===\n");

    TEST_ASSERT(led_init(LED_PIN) == 0, "LED initialization");
    TEST_ASSERT(led_set_state(LED_ON) == 0, "LED turn on");
    TEST_ASSERT(led_get_state() == LED_ON, "LED state check (ON)");
    TEST_ASSERT(led_set_state(LED_OFF) == 0, "LED turn off");
    TEST_ASSERT(led_get_state() == LED_OFF, "LED state check (OFF)");
    TEST_ASSERT(led_toggle() == 0, "LED toggle");
    TEST_ASSERT(led_get_state() == LED_ON, "LED state after toggle");
    TEST_ASSERT(led_blink(10) == 0, "LED blink");

    led_deinit();

    printf("All LED tests passed!\n");
    return 0;
}
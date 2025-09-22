#include "led.h"
#include "../hal/gpio.h"
#include "../../include/config.h"
#include <stdio.h>

static uint8_t led_pin = 0;
static bool led_initialized = false;

int led_init(uint8_t pin) {
    if (gpio_init(pin, GPIO_MODE_OUTPUT) != 0) {
        return -1;
    }

    led_pin = pin;
    led_initialized = true;

    gpio_write(led_pin, GPIO_STATE_LOW);

    DEBUG_PRINT("LED initialized on pin %d", pin);
    return 0;
}

int led_set_state(led_state_t state) {
    if (!led_initialized) {
        return -1;
    }

    gpio_state_t gpio_state = (state == LED_ON) ? GPIO_STATE_HIGH : GPIO_STATE_LOW;
    int result = gpio_write(led_pin, gpio_state);

    if (result == 0) {
        printf("[LED] LED %s\n", (state == LED_ON) ? "ON" : "OFF");
    }

    return result;
}

int led_toggle(void) {
    if (!led_initialized) {
        return -1;
    }

    int result = gpio_toggle(led_pin);
    if (result == 0) {
        led_state_t current_state = led_get_state();
        printf("[LED] LED toggled to %s\n", (current_state == LED_ON) ? "ON" : "OFF");
    }

    return result;
}

led_state_t led_get_state(void) {
    if (!led_initialized) {
        return LED_OFF;
    }

    gpio_state_t gpio_state = gpio_read(led_pin);
    return (gpio_state == GPIO_STATE_HIGH) ? LED_ON : LED_OFF;
}

int led_blink(uint32_t duration_ms) {
    if (!led_initialized) {
        return -1;
    }

    printf("[LED] Blinking for %u ms\n", duration_ms);

    led_set_state(LED_ON);
    for (volatile uint32_t i = 0; i < duration_ms * 1000; i++) {

    }
    led_set_state(LED_OFF);

    return 0;
}

void led_deinit(void) {
    if (led_initialized) {
        led_set_state(LED_OFF);
        gpio_deinit(led_pin);
        led_initialized = false;
        DEBUG_PRINT("LED deinitialized");
    }
}
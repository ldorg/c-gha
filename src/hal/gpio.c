#include "gpio.h"
#include "../../include/config.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PINS 32

static struct {
    gpio_mode_t mode;
    gpio_state_t state;
    bool initialized;
} gpio_pins[MAX_PINS];

int gpio_init(uint8_t pin, gpio_mode_t mode) {
    if (pin >= MAX_PINS) {
        return -1;
    }

    gpio_pins[pin].mode = mode;
    gpio_pins[pin].state = GPIO_STATE_LOW;
    gpio_pins[pin].initialized = true;

    DEBUG_PRINT("GPIO pin %d initialized in mode %d", pin, mode);
    return 0;
}

int gpio_write(uint8_t pin, gpio_state_t state) {
    if (pin >= MAX_PINS || !gpio_pins[pin].initialized) {
        return -1;
    }

    if (gpio_pins[pin].mode != GPIO_MODE_OUTPUT) {
        return -1;
    }

    gpio_pins[pin].state = state;
    DEBUG_PRINT("GPIO pin %d set to %s", pin, state ? "HIGH" : "LOW");
    return 0;
}

gpio_state_t gpio_read(uint8_t pin) {
    if (pin >= MAX_PINS || !gpio_pins[pin].initialized) {
        return GPIO_STATE_LOW;
    }

    if (gpio_pins[pin].mode == GPIO_MODE_INPUT) {
        gpio_pins[pin].state = (gpio_state_t)(rand() % 2);
    }

    return gpio_pins[pin].state;
}

int gpio_toggle(uint8_t pin) {
    if (pin >= MAX_PINS || !gpio_pins[pin].initialized) {
        return -1;
    }

    gpio_state_t new_state = gpio_pins[pin].state ? GPIO_STATE_LOW : GPIO_STATE_HIGH;
    return gpio_write(pin, new_state);
}

void gpio_deinit(uint8_t pin) {
    if (pin < MAX_PINS) {
        gpio_pins[pin].initialized = false;
        DEBUG_PRINT("GPIO pin %d deinitialized", pin);
    }
}
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_INPUT_PULLUP,
    GPIO_MODE_INPUT_PULLDOWN
} gpio_mode_t;

typedef enum {
    GPIO_STATE_LOW = 0,
    GPIO_STATE_HIGH = 1
} gpio_state_t;

int gpio_init(uint8_t pin, gpio_mode_t mode);
int gpio_write(uint8_t pin, gpio_state_t state);
gpio_state_t gpio_read(uint8_t pin);
int gpio_toggle(uint8_t pin);
void gpio_deinit(uint8_t pin);

#endif
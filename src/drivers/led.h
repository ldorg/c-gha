#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} led_state_t;

int led_init(uint8_t pin);
int led_set_state(led_state_t state);
int led_toggle(void);
led_state_t led_get_state(void);
int led_blink(uint32_t duration_ms);
void led_deinit(void);

#endif
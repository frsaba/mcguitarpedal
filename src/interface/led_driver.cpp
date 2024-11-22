#include <interface/led_driver.h>
#include <Arduino.h>

uint8_t led_state;

void setup_leds()
{
	pinMode(LED_PWM, OUTPUT);
	pinMode(LED_DATA, OUTPUT);
	pinMode(LED_SHIFT_CLOCK, OUTPUT);
	pinMode(LED_STORE_CLOCK, OUTPUT);

	analogWrite(LED_PWM, 250);
}

void write_to_shift_register(uint8_t data)
{
	led_state = data;
	digitalWrite(LED_STORE_CLOCK, 0);
	shiftOut_msbFirst(LED_DATA, LED_SHIFT_CLOCK, data);

	digitalWrite(LED_STORE_CLOCK, 1);
	digitalWrite(LED_STORE_CLOCK, 0);
}

void led_toggle(uint8_t led)
{
	write_to_shift_register(led_state ^ led);
}

void led_set(uint8_t led, bool on)
{
	if(on) write_to_shift_register(led_state | led);
	else write_to_shift_register(led_state & ~led);
}
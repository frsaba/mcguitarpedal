#include <interface/led_driver.h>
#include <Arduino.h>

void setup_leds()
{
	pinMode(LED_PWM, OUTPUT);
	pinMode(LED_DATA, OUTPUT);
	pinMode(LED_SHIFT_CLOCK, OUTPUT);
	pinMode(LED_STORE_CLOCK, OUTPUT);

	analogWrite(LED_PWM, 230);
}

void write_to_shift_register(uint8_t data)
{
	digitalWrite(LED_STORE_CLOCK, 0);
	shiftOut_lsbFirst(LED_DATA, LED_SHIFT_CLOCK, data);

	digitalWrite(LED_STORE_CLOCK, 1);
	digitalWrite(LED_STORE_CLOCK, 0);
}
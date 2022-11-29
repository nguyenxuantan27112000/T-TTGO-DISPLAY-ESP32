/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   15/09/2016
 ******************************************************************************
**/
#include "AppData.h"
#include "led.h"

//void led_init(led_t* led, pf_led_ctrl init, pf_led_ctrl on, pf_led_ctrl off) {
//	led->blink_enable   = LED_BLINK_DISABLE;
//	led->counter        = (unsigned long)0;
//	led->round		= (unsigned char)0;
//	led->pf_ctrl        = init;
//	led->pf_on          = on;
//	led->pf_off         = off;
//
//	/* init hardware for led */
//	if (led->pf_ctrl) {
//		led->pf_ctrl();
//	}
//
//	/* set default led is off */
//	if (led->pf_off) {
//		led->pf_off();
//	}
//}

void led_on(led_t* led) {
	led->status = LED_STATUS_ON;
	if (led->pf_on) {
		led->pf_on();
	}
}

void led_off(led_t* led) {
	led->status = LED_STATUS_OFF;
	if (led->pf_off) {
		led->pf_off();
	}
}

//void led_toggle(led_t* led) {
////	if (led->status == LED_STATUS_ON) {
//	if (led->status) {
//		led_off(led);
//	}
////	else if (led->status == LED_STATUS_OFF) {
//	else {
//		led_on(led);
//	}
//}

void led_blink_set(led_t* led, unsigned long freq, unsigned long duty, unsigned char repeate) {
	led->counter = (unsigned char)0;
	
	led->freq = freq;
	led->duty = duty;
	
	led->round = (unsigned char)0;
	led->repeate = repeate;

	led->blink_enable = LED_BLINK_ENABLE;
}

void led_blink_reset(led_t* led) {
	led->blink_enable = LED_BLINK_DISABLE;
	
	if (led->pf_off) {
		led->pf_off();
	}
}

unsigned char led_is_blink(led_t* led) {
	return led->blink_enable;
}

void led_blink_polling(led_t* led) {
	if (led->blink_enable) {

		led->counter += TICK_MS;

		if (led->counter <= led->duty) {
			if (!led->status) {			
				led_on(led);
			}
		}
		else if (led->counter < led->freq) {
			if (led->status) {
				led_off(led);
			}
		}
		else if (led->counter == led->freq) {
			led->counter = (unsigned char)0;
			
			if (led->repeate) {
				led->round++;
				
				if (led->round == led->repeate) {					
					led->repeate = (unsigned char)0	;
				
					led->blink_enable = LED_BLINK_DISABLE;
				}
			}
		}
	}
}

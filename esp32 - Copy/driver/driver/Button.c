#include "Button.h"

unsigned char button_init(button_t* button, unsigned long u, unsigned char id, pf_button_ctrl init, pf_button_read read, pf_button_callback callback) {
	button->enable		=	BUTTON_DISABLE;
	button->id			=	id;
	button->counter		=	0;
	button->unit		=	u;
	button->state		=	BUTTON_SW_STATE_RELEASED;

	button->init		=	init;
	button->read		=	read;
	button->callback	=	callback;

	if (button->init) {
		button->init();
	}
	else {
		return BUTTON_DRIVER_NG;
	}

	if (!button->read) {
		return BUTTON_DRIVER_NG;
	}

	if (!button->callback) {
		return BUTTON_DRIVER_NG;
	}

	return BUTTON_DRIVER_OK;
}

void button_enable(button_t* button) {
	button->enable = BUTTON_ENABLE;
}

void button_disable(button_t* button) {
	button->enable = BUTTON_DISABLE;
}

void button_timer_polling(button_t* button) {
	if (button->enable) {

		/* hard button pressed */
		if (button->read()) {

			if (button->counter_enable) {
				/* increase button counter */
				button->counter += button->unit;

				/* check long press */
				if (button->counter >= BUTTON_LONG_PRESS_TIME &&
						button->state != BUTTON_SW_STATE_LONG_PRESSED) {

					button->enable			= BUTTON_DISABLE;
					button->counter_enable	= BUTTON_DISABLE;
					button->state			= BUTTON_SW_STATE_LONG_PRESSED;
					button->callback(button);
					button->enable			= BUTTON_ENABLE;
				}
				/* check short press */
				else if (button->counter >= BUTTON_SHORT_PRESS_MIN_TIME &&
						 button->state != BUTTON_SW_STATE_PRESSED) {

					button->enable			= BUTTON_DISABLE;
					button->state			= BUTTON_SW_STATE_PRESSED;
					button->callback(button);
					button->enable			= BUTTON_ENABLE;
				}
			}
		}
		/* hard button released */
		else {
			/* check released */
			if (button->counter >= BUTTON_LONG_PRESS_TIME) {
				button->state			= BUTTON_SW_STATE_LONG_RELEASED;				
				button->enable			= BUTTON_DISABLE;
				button->callback(button);
			}
			else if (button->counter >= BUTTON_SHORT_PRESS_MIN_TIME){
				button->state			= BUTTON_SW_STATE_SHORT_RELEASED;				
				button->enable			= BUTTON_DISABLE;
				button->callback(button);
			}
			else {
				button->state			= BUTTON_SW_STATE_RELEASED;				
			}

			/* reset button */
			button->counter			= 0;
			button->counter_enable	= BUTTON_ENABLE;
			button->enable			= BUTTON_ENABLE;
		}
	}
}

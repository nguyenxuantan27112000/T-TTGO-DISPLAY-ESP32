#ifndef __BUTTON_H__
#define __BUTTON_H__

#define		BUTTON_DRIVER_OK						(0x00)
#define		BUTTON_DRIVER_NG						(0xff)

#define		BUTTON_SHORT_PRESS_MIN_TIME				(20)		/* 20ms */
#define		BUTTON_LONG_PRESS_TIME					(3000)		/* 3s */

#define		BUTTON_DISABLE							(0x00)
#define		BUTTON_ENABLE							(0xff)

/*
 * This define depend on hardware circuit.
 * Note: please change it for respective.
 */
//#define		BUTTON_HW_STATE_PRESSED					(0x00)
//#define		BUTTON_HW_STATE_RELEASED				(0x01)

#define		BUTTON_SW_STATE_PRESSED					(0x00)
#define		BUTTON_SW_STATE_LONG_PRESSED			(0x01)
#define		BUTTON_SW_STATE_SHORT_RELEASED			(0x02)
#define		BUTTON_SW_STATE_LONG_RELEASED			(0x04)
#define		BUTTON_SW_STATE_RELEASED				(0x08)

typedef void (*pf_button_ctrl)();
typedef unsigned char (*pf_button_read)();
typedef void (*pf_button_callback)(void*);

typedef struct {
	unsigned char id;
	unsigned char enable;
	unsigned char state;
	unsigned char counter_enable;

	unsigned long counter;
	unsigned long unit;

	pf_button_ctrl init;
	pf_button_read read;

	pf_button_callback callback;
} button_t;

unsigned char	button_init(button_t* button, unsigned long u, unsigned char id, pf_button_ctrl init, pf_button_read read, pf_button_callback callback);
void	button_enable(button_t* button);
void	button_disable(button_t* button);
void	button_timer_polling(button_t* button);


#endif //__BUTTON_H__

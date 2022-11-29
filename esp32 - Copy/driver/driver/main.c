// #include lib
// // Khai bao button & led
// volatile button_t button_1 = {BUT1_ID, BUTTON_ENABLE, BUTTON_SW_STATE_RELEASED, BUTTON_DISABLE, 0, 10, 0, button1Read, buttonNumberCallback};
// volatile led_t led_common = {0, 0, 0, 0, 0, 0, 0, 0, led_common_on, led_common_off};

/* 
example interrupt 10ms
void interrupt_timer()
{
    button_timer_polling(&button_1);
    led_blink_polling(&led_common);
}
*/
void main(void)
{
    // Create interrupt timer for polling callback function
    // Config button
} 

void buttonCallback(button_t* button)
{
    // check state
    
    // switch(button->state) {

    //     case: 
    // }
    
}
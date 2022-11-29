/* Deep sleep wake up example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"

#if CONFIG_IDF_TARGET_ESP32
#include "esp32/ulp.h"
#endif

#if SOC_TOUCH_SENSOR_NUM > 0
#include "soc/sens_periph.h"
#include "driver/touch_pad.h"
#endif


static RTC_DATA_ATTR struct timeval sleep_enter_time;
RTC_DATA_ATTR int dem = 0;


void app_main(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);      // Tinh thoi gian tu luc ngu den luc duoc danh thuc
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
    printf("sleep_time_ms: %d\n", sleep_time_ms);

    dem++;
    printf("dem = %d", dem);

    switch (esp_sleep_get_wakeup_cause()) {
#ifdef CONFIG_EXAMPLE_EXT1_WAKEUP
        case ESP_SLEEP_WAKEUP_EXT1: {
            uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();
            if (wakeup_pin_mask != 0) {
                int pin = __builtin_ffsll(wakeup_pin_mask) - 1;
                printf("Wake up from GPIO %d\n", pin);
            } else {
                printf("Wake up from GPIO\n");
            }
            break;
        }
#endif // CONFIG_EXAMPLE_EXT1_WAKEUP
#if SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
        case ESP_SLEEP_WAKEUP_GPIO: {
            uint64_t wakeup_pin_mask = esp_sleep_get_gpio_wakeup_status();
            if (wakeup_pin_mask != 0) {
                int pin = __builtin_ffsll(wakeup_pin_mask) - 1;
                printf("Wake up from GPIO %d\n", pin);
            } else {
                printf("Wake up from GPIO\n");
            }
            break;
        }
#endif //SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            break;
        }
#ifdef CONFIG_EXAMPLE_TOUCH_WAKEUP
        case ESP_SLEEP_WAKEUP_TOUCHPAD: {
            printf("Wake up from touch on pad %d\n", esp_sleep_get_touchpad_wakeup_status());
            break;
        }
#endif // CONFIG_EXAMPLE_TOUCH_WAKEUP
#ifdef CONFIG_EXAMPLE_ULP_TEMPERATURE_WAKEUP
#if CONFIG_IDF_TARGET_ESP32

#endif // CONFIG_IDF_TARGET_ESP32
#endif // CONFIG_EXAMPLE_ULP_TEMPERATURE_WAKEUP
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            printf("Not a deep sleep reset\n");
    }


#ifdef CONFIG_EXAMPLE_EXT1_WAKEUP
    const int ext_wakeup_pin_1 = 0;                 // GPIO 0
    const uint64_t ext_wakeup_pin_1_mask = 1ULL << ext_wakeup_pin_1;
    const int ext_wakeup_pin_2 = 4;                 // GPIO 4
    const uint64_t ext_wakeup_pin_2_mask = 1ULL << ext_wakeup_pin_2;

    printf("Enabling EXT1 wakeup on pins GPIO%d, GPIO%d\n", ext_wakeup_pin_1, ext_wakeup_pin_2);
    esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_1_mask | ext_wakeup_pin_2_mask, ESP_EXT1_WAKEUP_ALL_LOW);
#endif // CONFIG_EXAMPLE_EXT1_WAKEUP


// #ifdef CONFIG_EXAMPLE_ULP_TEMPERATURE_WAKEUP
// #if CONFIG_IDF_TARGET_ESP32
//     printf("Enabling ULP wakeup\n");
//     esp_sleep_enable_ulp_wakeup();
// #endif
// #endif

#if CONFIG_IDF_TARGET_ESP32
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    printf("Entering deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);

#ifdef CONFIG_EXAMPLE_ULP_TEMPERATURE_WAKEUP
#if CONFIG_IDF_TARGET_ESP32
    start_ulp_temperature_monitoring();
#endif
#endif

    esp_deep_sleep_start();
    
}
    

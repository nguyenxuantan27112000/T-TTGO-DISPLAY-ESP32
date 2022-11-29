#ifndef __HTTP_SERVER_APP_H__
#define __HTTP_SERVER_APP_H__

typedef void (*http_post_callback_t) (char *dta, int len);

void wifi_init_sta(void);
void start_webserver();
void stop_webserver();
void station_wifi_config();
void http_set_callback_wifinfo(void *cb);

#endif
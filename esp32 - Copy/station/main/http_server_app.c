#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http_server_app.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <esp_http_server.h>
#include "esp_wpa2.h"
#include "esp_smartconfig.h"

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

// #define EXAMPLE_ESP_WIFI_SSID      "TAN"
// #define EXAMPLE_ESP_WIFI_PASS      "12345678"
// #define EXAMPLE_ESP_MAXIMUM_RETRY  5

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

// static int s_retry_num = 0;
static const char *TAG = "example";
httpd_handle_t server = NULL;

/* Smart Config */
static const int CONNECTED_BIT = BIT2;
static const int ESPTOUCH_DONE_BIT = BIT3;

/* HTML WEB */
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

static http_post_callback_t http_post_wifi_info_callback = NULL;
static EventGroupHandle_t s_wifi_event_group;

static void smartconfig_example_task(void * parm);

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG, "Scan done");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
        ESP_LOGI(TAG, "Found channel");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        uint8_t ssid[33] = { 0 };
        uint8_t password[65] = { 0 };
        uint8_t rvd_data[33] = { 0 };

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true) {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", password);
        if (evt->type == SC_TYPE_ESPTOUCH_V2) {
            ESP_ERROR_CHECK( esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)) );
            ESP_LOGI(TAG, "RVD_DATA:");
            for (int i=0; i<33; i++) {
                printf("%02x ", rvd_data[i]);
            }
            printf("\n");
        }

        ESP_ERROR_CHECK( esp_wifi_disconnect() );
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
        esp_wifi_connect();
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
        xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
    }
}


static void initialise_wifi(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}


static void smartconfig_example_task(void * parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
    while (1) {
        uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
        if(uxBits & CONNECTED_BIT) {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }
        // if(uxBits & ESPTOUCH_DONE_BIT) {
        //     ESP_LOGI(TAG, "smartconfig over");
        //     esp_smartconfig_stop();
        //     vTaskDelete(NULL);
        // }
    }
}


static esp_err_t sw1_post_handler(httpd_req_t *req)
{
    char buf[100];

    // Read the data for the request
    httpd_req_recv(req, buf,req->content_len);
    printf("DATA: %s\n", buf);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_uri_t sw1_post_data = {
    .uri       = "/switch1",
    .method    = HTTP_POST,
    .handler   = sw1_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


static esp_err_t wifi_post_handler(httpd_req_t *req)
{
    char buf[100];

    // Read the data for the request
    httpd_req_recv(req, buf,req->content_len);
    http_post_wifi_info_callback(buf, req->content_len);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_uri_t wifi_post_data = {
    .uri       = "/Enterwifi",
    .method    = HTTP_POST,
    .handler   = wifi_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


static esp_err_t data_post_handler(httpd_req_t *req)
{
    char buf[100];

    // Read the data for the request
    httpd_req_recv(req, buf,req->content_len);
    printf("DATA: %s\n", buf);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_uri_t post_data = {
    .uri       = "/data",
    .method    = HTTP_POST,
    .handler   = data_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


static esp_err_t hello_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
    return ESP_OK;
}

httpd_uri_t get_dht11 = {
    .uri       = "/dht11",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


/* An HTTP GET handler */
static esp_err_t dht11_get_handler(httpd_req_t *req)
{
    // const char* resp_str = (const char*) "Hello !!!!";
    const char* resp_str = (const char*) "{\"temperature\": \"27.3\", \"humidity\": \"80\"}";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

httpd_uri_t get_data_dht11 = {
    .uri       = "/getdatadht11",
    .method    = HTTP_GET,
    .handler   = dht11_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/hello", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}


// static void event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         esp_wifi_connect();
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
//         if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
//             esp_wifi_connect();
//             s_retry_num++;
//             ESP_LOGI(TAG, "retry to connect to the AP");
//         } else {
//             xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
//         }
//         ESP_LOGI(TAG,"connect to the AP fail");
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
//         ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
//         s_retry_num = 0;
//         xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
//     }
// }


// void wifi_init_sta(void)
// {
//     s_wifi_event_group = xEventGroupCreate();

//     ESP_ERROR_CHECK(esp_netif_init());

//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_create_default_wifi_sta();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));


//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,
//                                                         ESP_EVENT_ANY_ID,
//                                                         &event_handler,
//                                                         NULL));
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,
//                                                         IP_EVENT_STA_GOT_IP,
//                                                         &event_handler,
//                                                         NULL));

//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = EXAMPLE_ESP_WIFI_SSID,
//             .password = EXAMPLE_ESP_WIFI_PASS,
//             /* Setting a password implies station will connect to all security modes including WEP/WPA.
//              * However these modes are deprecated and not advisable to be used. Incase your Access point
//              * doesn't support WPA2, these mode can be enabled by commenting below line */
// 	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

//             .pmf_cfg = {
//                 .capable = true,
//                 .required = false
//             },
//         },
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
//     ESP_ERROR_CHECK(esp_wifi_start() );

//     ESP_LOGI(TAG, "wifi_init_sta finished.");

//     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
//      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
//     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
//             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);

//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_CONNECTED_BIT) {
//         ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else if (bits & WIFI_FAIL_BIT) {
//         ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     }
// }


void start_webserver()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &post_data);
        httpd_register_uri_handler(server, &wifi_post_data);
        httpd_register_uri_handler(server, &sw1_post_data);
        httpd_register_uri_handler(server, &get_dht11);
        httpd_register_uri_handler(server, &get_data_dht11);       
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    else{
        ESP_LOGI(TAG, "Error starting server!");
    }
}


void stop_webserver()
{
    // Stop the httpd server
    httpd_stop(server);
}

void http_set_callback_wifiinfo(void *cb)
{
    http_post_wifi_info_callback = cb;
}

void wifi_data_callback(char *data, int len)
{
    char ssid[30] = "";
    char pass[30] = "";

    printf("->data: %s\n", data);
    char *pt = strtok(data, "@");
    if (pt)
    {
        strcpy(ssid, pt);
        pt = strtok(NULL, "@");   // dien vao chuoi con lai
    }
    if (pt)
    {
        strcpy(pass, pt);
    }    

    printf("->ssid: %s\n", ssid);
    printf("->pass: %s\n,", pass);

    // stop_webserver();
    esp_wifi_disconnect();
    esp_wifi_stop();
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,

                .pmf_cfg = {
                    .capable = true,
                    .required = false
                },
        },
    };

    strcpy((char*)wifi_config.sta.ssid,ssid);
    strcpy((char*)wifi_config.sta.password,pass);
    printf("4\n");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Check xem da dung thong tin chua
    wifi_config_t wifi_config_get;    
    esp_wifi_get_config(WIFI_IF_STA, &wifi_config_get);
    printf("->ssid: %s\n", wifi_config_get.sta.ssid);
    printf("->pass: %s\n,", wifi_config_get.sta.password);

    ESP_ERROR_CHECK(esp_wifi_start());
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                WIFI_CONNECTED_BIT,
                                pdTRUE,
                                pdFALSE,
                                portMAX_DELAY);
    // start_webserver();                           
}

void station_wifi_config()
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // wifi_init_sta();
    initialise_wifi();
    start_webserver();
    
    http_set_callback_wifiinfo(wifi_data_callback);
}


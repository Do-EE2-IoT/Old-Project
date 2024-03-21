#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_server.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "soft_ap.h"


#ifndef MIN
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#endif

char ssid[32];
char pass[32];

static http_post_wifi_callback_t http_post_wifi_callback = NULL;

/* Wifisetup_handler function to be called during POST / request */
esp_err_t wifisetup_handler(httpd_req_t *req)
{
    char buf[32];
    char *token;
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        /* Send back the same data */
        httpd_resp_send_chunk(req, buf, ret);
        remaining -= ret;
        // printf("%s and %d\n",buf,req->content_len);
        token = strtok(buf,"@");
        strcpy(ssid,token);
        token = strtok(NULL, "@");
        strcpy(pass,token);  
        http_post_wifi_callback(ssid,pass);
    }
 // End response
    httpd_resp_send_chunk(req, "NULL", 0);
    return ESP_OK;
}

/* wifi setup handler structure for POST /wifisetup */
httpd_uri_t wifisetup = {
    .uri      = "/wifisetup",
    .method   = HTTP_POST,
    .handler  = wifisetup_handler,
    .user_ctx = NULL
};

/* Function for starting the webserver */
httpd_handle_t start_webserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &wifisetup);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

/* Function for stopping the webserver */
void stop_webserver(httpd_handle_t server)
{
    if (server) {
        /* Stop the httpd server */
        httpd_stop(server);
    }
}

/*==============DEFINE ACCESS POINT CONFIG============*/
#define AP_SSID 			"ESP32"
#define AP_PASS 			"12345678"
#define AP_CHANNEL   		1
#define AP_MAX_STA_CONN  	1

static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
    // printf("%"PRIu32"\n", event_id);
    // if (event_id == WIFI_EVENT_AP_STACONNECTED) {
    //     wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
    // } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
    //     wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
    // }
}

void wifi_init_soft_ap()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = AP_SSID,
            .ssid_len = strlen(AP_SSID),
            .channel = AP_CHANNEL,
            .password = AP_PASS,
            .max_connection = AP_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                    .required = true,
            },
        },
    };
    if (strlen(AP_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    printf("wifi_init_softap finished. SSID:%s password:%s channel:%d\n",
             AP_SSID, AP_PASS, AP_CHANNEL);

    start_webserver();

}

void wifi_stop_soft_ap(){
    esp_wifi_stop();
}

void http_set_wifi_callback(void *cb){
    http_post_wifi_callback = cb;
}
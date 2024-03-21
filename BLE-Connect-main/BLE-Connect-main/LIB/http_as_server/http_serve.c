/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "http_sever_ble_app.h"
#include <esp_http_server.h>
#include <stdio.h>

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

static const char *TAG = "HTTP SERVER";
    httpd_handle_t server = NULL;

 extern const uint8_t index_htlm_start[24] asm("_binary_index_html_start");
 extern const uint8_t index_htlm_end[22] asm("_binary_index_html_end");


 static http_get_callback_t http_get_callback = NULL;
 static http_post_callback_t http_post_callback = NULL;
 static http_post_slider_t http_post_slider_callback = NULL;
 static http_post_wifi_callback_t http_post_wifi_callback = NULL;
/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req,"text/html");
    httpd_resp_send(req,(const char *)index_htlm_start, index_htlm_end - index_htlm_start);
    return ESP_OK;
}

 const httpd_uri_t get_data_dht11 = {
    .uri       = "/dht11",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};
httpd_req_t *Req;
void take_data_of_get(char *data_of_humidity, int leng){
     
     httpd_resp_send(Req,(const char *)data_of_humidity, strlen(data_of_humidity));
}
int i = 5;
static esp_err_t hello_get_handler_data(httpd_req_t *req)
{
    Req = req;
    http_get_callback();
    
 
    return ESP_OK;
}
//Uniform Resource Identifier
 const httpd_uri_t get_data_dht11_now = {
    .uri       = "/getdatadht11",
    .method    = HTTP_GET,
    .handler   = hello_get_handler_data,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};

/* An HTTP POST handler */
static esp_err_t echo_post_handler(httpd_req_t *req)
{
    char buf[100];
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

        /* Log data received */
        ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        ESP_LOGI(TAG, "%.*s", ret, buf);
        ESP_LOGI(TAG, "====================================");
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t echo = {
    .uri       = "/echo",
    .method    = HTTP_POST,
    .handler   = echo_post_handler,
    .user_ctx  = NULL
};

static esp_err_t switch_post_handler(httpd_req_t *req)
{
    char buf[100];
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

        /* Log data received */
        // ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        // ESP_LOGI(TAG, "%.*s", ret, buf);
        http_post_callback(buf,req->content_len);
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t switch1 = {
    .uri       = "/switch1",
    .method    = HTTP_POST,
    .handler   = switch_post_handler,
    .user_ctx  = NULL
};

static esp_err_t wifi_hanler_get_from_server(httpd_req_t *req){
 char buf[100];
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

        /* Log data received */
        // ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        // ESP_LOGI(TAG, "%.*s", ret, buf);
        http_post_wifi_callback(buf,req->content_len);

}
 // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t wifi = {
    .uri       = "/wifi_ssid_and_pass",
    .method    = HTTP_POST,
    .handler   = wifi_hanler_get_from_server,
    .user_ctx  = NULL
};


static esp_err_t slider_post_handler(httpd_req_t *req)
{
    char buf[100];
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

        /* Log data received */
        // ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        // ESP_LOGI(TAG, "%.*s", ret, buf);
        http_post_slider_callback(buf,req->content_len);
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t slider = {
    .uri       = "/slider",
    .method    = HTTP_POST,
    .handler   = slider_post_handler,
    .user_ctx  = NULL
};


/* This handler allows the custom error handling functionality to be
 * tested from client side. For that, when a PUT request 0 is sent to
 * URI /ctrl, the /hello and /echo URIs are unregistered and following
 * custom error handler http_404_error_handler() is registered.
 * Afterwards, when /hello or /echo is requested, this custom error
 * handler is invoked which, after sending an error message to client,
 * either closes the underlying socket (when requested URI is /echo)
 * or keeps it open (when requested URI is /hello). This allows the
 * client to infer if the custom error handler is functioning as expected
 * by observing the socket state.
 */

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/dht11", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/dht11 URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    } //else if (strcmp("/echo", req->uri) == 0) {
       // httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
        /* Return ESP_FAIL to close underlying socket */
       // return ESP_FAIL;
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

// start overhere // 
void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &switch1);
         httpd_register_uri_handler(server, &echo);
         httpd_register_uri_handler(server, &get_data_dht11);
         httpd_register_uri_handler(server,&get_data_dht11_now);
         httpd_register_uri_handler(server,&slider);
         httpd_register_uri_handler(server, &wifi);

    }
       else{ 
        ESP_LOGI(TAG, "Error starting server!");
       }
    }


 void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}


void http_set_get_dht_callback(void *cb){
    http_get_callback = cb;
}

void http_set_post_switch_callback(void *cb){
    http_post_callback = cb;
}

void http_set_post_slider_callback(void *cb){
    http_post_slider_callback = cb;
}

void http_set_wifi_callback(void *cb){
    http_post_wifi_callback = cb;
}


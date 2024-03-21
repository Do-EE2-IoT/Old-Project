#include "BLE_app.h"
#include "mqtt_client.h"

char *TAG = "BLE-Server";
uint8_t ble_addr_type;
static void ble_app_advertise(void);

 esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://white-dev.aithings.vn:1883",
    };
    esp_mqtt_client_handle_t client;

// Write data to ESP32 defined as server
static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    // printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
    int i = 0;
    char copy[100];
    if(strlen((char *)ctxt->om->om_data) > 0){
        vTaskDelay(100/portTICK_PERIOD_MS);
        strcpy(copy,(char *)ctxt->om->om_data);
        printf("copy %s\n",copy);
        esp_mqtt_client_publish(client,"/aithingsdev/a0:b7:65:dd:28:48/data", (char *)ctxt->om->om_data, strlen((char *)ctxt->om->om_data), 1, 1);
           for(i = 0; i < strlen((char *)ctxt->om->om_data); i++){
        memset(ctxt->om->om_data,0x00,strlen((char *)ctxt->om->om_data));
    }
         strcpy(copy,(char *)ctxt->om->om_data);
        printf("%s after_delete\n",copy);
         
    }
    return 0;
}


char save_for_subcribe_for_mqtt[1024];
// Read data from ESP32 defined as server
static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{   
    os_mbuf_append(ctxt->om, save_for_subcribe_for_mqtt, strlen(save_for_subcribe_for_mqtt));
    return 0;
}




// Array of pointers to other service definitions
// UUID - Universal Unique Identifier
//{0} để thể hiện rằng không còn mảng nữa
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(0x180),                 // Define UUID for device type
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(0xFEF4),           // Define UUID for reading
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = device_read},
         {.uuid = BLE_UUID16_DECLARE(0xDEAD),           // Define UUID for writing
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = device_write,
          },
         {0}}},
    {0}};

// BLE event handling
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    // Advertise if connected
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        break;
    // Advertise again after completion of the event
    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT DISCONNECTED");
        break;
    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI("GAP", "BLE GAP EVENT");
        ble_app_advertise();
        break;
    default:
        break;
    }
    return 0;
}

// Define the BLE connection
static void ble_app_advertise(void)
{
    // GAP - device name definition
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields));
    device_name = ble_svc_gap_device_name(); // Read the BLE device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&fields);

    // GAP - device connectivity definition
    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable
    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
}

// The application
static void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Determines the best address type automatically
    ble_app_advertise();                     // Define the BLE connection
}

// The infinite task
static void host_task(void *param)
{
    nimble_port_run(); // This function will return only when nimble_port_stop() is executed
}

void BLE_start(void)
{                          // 1 - Initialize NVS flash using
    esp_nimble_hci_and_controller_init();      // 2 - Initialize ESP controller
    nimble_port_init();   
                         // 3 - Initialize the host stack
    ble_svc_gap_device_name_set("ESP32_BLE_MQTT"); // 4 - Initialize NimBLE configuration - server name
    ble_svc_gap_init();                        // 4 - Initialize NimBLE configuration - gap service
    ble_svc_gatt_init();                       // 4 - Initialize NimBLE configuration - gatt service
    ble_gatts_count_cfg(gatt_svcs);            // 4 - Initialize NimBLE configuration - config gatt services
    ble_gatts_add_svcs(gatt_svcs);             // 4 - Initialize NimBLE configuration - queues gatt services.
    ble_hs_cfg.sync_cb = ble_app_on_sync;      // 5 - Initialize application
    nimble_port_freertos_init(host_task);      // 6 - Run the thread
    output_io_create(GPIO_NUM_2);
}


/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/




static const char *TAGG = "MQTT_EXAMPLE";

char take_sub[20];
static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAGG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, "/aithingsdev/a0:b7:65:dd:28:48/data", "connected from esp32", 1, 0, 0);
            ESP_LOGI(TAGG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/aithingsdev/a0:b7:65:dd:28:48/control", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAGG, "MQTT_EVENT_DISCONNECTED");
            break;
        
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAGG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAGG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAGG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAGG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            memset(save_for_subcribe_for_mqtt,0x00,strlen(save_for_subcribe_for_mqtt));
            strcpy(save_for_subcribe_for_mqtt,(char)event->topic);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAGG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

char mqtt_data[100];
void mqtt_app_start(void)
{
   
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}



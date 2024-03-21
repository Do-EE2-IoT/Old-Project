#include "BLE_app_lab.h"

char *TAG = "BLE-Server";
uint8_t ble_addr_type;
static void ble_app_advertise(void);
static int device_write_other(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg){
  int i = 0;
  if (strcmp((char *)ctxt->om->om_data, (char *)"LIGHT ON")==0)
    {
       printf("LIGHT ON x 2\n");
       for( i = 0 ; i < 20; i++){
    //    output_toggle_pin(GPIO_NUM_2);
       vTaskDelay(200/portTICK_PERIOD_MS);
       }
        for(i = 0; i < strlen((char *)ctxt->om->om_data); i++){
        memset(ctxt->om->om_data,0x00,strlen((char *)ctxt->om->om_data));
    }
    }
    else {return 0;}
    return 0;
}
// Write data to ESP32 defined as server
static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    // printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
    int i = 0;
    if (strcmp((char *)ctxt->om->om_data, (char *)"LIGHT ON")==0)
    {
       printf("LIGHT ON\n");
    //    output_set_level(GPIO_NUM_2, 1);
        for(i = 0; i < strlen((char *)ctxt->om->om_data); i++){
        memset(ctxt->om->om_data,0x00,strlen((char *)ctxt->om->om_data));
    }
    }
    else if (strcmp((char *)ctxt->om->om_data, (char *)"LIGHT OFF")==0)
    {
        printf("LIGHT OFF\n");
        // output_set_level(GPIO_NUM_2, 0);
          for(i = 0; i < strlen((char *)ctxt->om->om_data); i++){
        memset(ctxt->om->om_data,0x00,strlen((char *)ctxt->om->om_data));
    }
    }
    else if (strcmp((char *)ctxt->om->om_data, (char *)"FAN ON\0")==0)
    {
        printf("FAN ON\n");
    }
    else if (strcmp((char *)ctxt->om->om_data, (char *)"FAN OFF\0")==0)
    {
        printf("FAN OFF\n");
    }
    else{
        printf("Data from the client: %s\n", ctxt->om->om_data);
          for(i = 0; i < strlen((char *)ctxt->om->om_data); i++){
        memset(ctxt->om->om_data,0x00,strlen((char *)ctxt->om->om_data));
    }
    }
    
    
    return 0;
}

// Read data from ESP32 defined as server
static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "Data from the server", strlen("Data from the server"));
    return 0;
}

static int device_read_second_service(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "Hello world", strlen("Hello world"));
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
         {
          .uuid = BLE_UUID16_DECLARE(0x1111),           // Define UUID for writing
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = device_write_other,
         },
         {0}}},
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x200),
        .characteristics = (struct ble_gatt_chr_def[]){
            {.uuid = BLE_UUID16_DECLARE(0xAAAA),           // Define UUID for reading
             .flags = BLE_GATT_CHR_F_READ,
             .access_cb = device_read_second_service,
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
    ble_svc_gap_device_name_set("BLE NOW"); // 4 - Initialize NimBLE configuration - server name
    ble_svc_gap_init();                        // 4 - Initialize NimBLE configuration - gap service
    ble_svc_gatt_init();                       // 4 - Initialize NimBLE configuration - gatt service
    ble_gatts_count_cfg(gatt_svcs);            // 4 - Initialize NimBLE configuration - config gatt services
    ble_gatts_add_svcs(gatt_svcs);             // 4 - Initialize NimBLE configuration - queues gatt services.
    ble_hs_cfg.sync_cb = ble_app_on_sync;      // 5 - Initialize application
    nimble_port_freertos_init(host_task);      // 6 - Run the thread
    // output_io_create(GPIO_NUM_2);
}

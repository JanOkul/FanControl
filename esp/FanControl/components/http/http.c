#include "infrared.h"

#include "esp_http_server.h"
#include "http.h"
#include "esp_log.h"

// The NEC address of the fan.
const uint8_t nec_address = 0x20;

// NEC command values for each fan command.
const uint8_t power_toggle_nec_command = 0xC;
const uint8_t speed_up_nec_command = 0x16;
const uint8_t speed_down_nec_command = 0xF;
const uint8_t timer_up_nec_command = 0x6;
const uint8_t timer_down_nec_command = 0x7;
const uint8_t swing_up_nec_command = 0x014;
const uint8_t swing_side_nec_command = 0x10;

static httpd_handle_t server = NULL;
static const char* TAG = "HTTP";

// HTTP handlers for each fan command.
esp_err_t power_uri_handler(httpd_req_t *req) {
    nec_send(nec_address, power_toggle_nec_command);
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t speed_up_uri_handler(httpd_req_t *req) {
    nec_send(nec_address, speed_up_nec_command);
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t speed_down_uri_handler(httpd_req_t *req) {
    nec_send(nec_address, speed_down_nec_command);
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t timer_up_uri_handler(httpd_req_t *req) {
    nec_send(nec_address, timer_up_nec_command);
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t timer_down_uri_handler(httpd_req_t *req) {
    nec_send(nec_address, timer_down_nec_command);
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t swing_up_uri_handler(httpd_req_t *req) {
    nec_send(nec_address, swing_up_nec_command);
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t swing_side_uri_handler(httpd_req_t *req) {
    nec_send(nec_address, swing_side_nec_command);
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t no_page_handler(httpd_req_t *req) {
    httpd_resp_send_404(req);
    return ESP_OK;
}

// Registers all the HTTP handlers with the HHTP server.
void register_uri_handlers(httpd_handle_t* server) {
    const httpd_uri_t power_uri = {
        .uri      = "/power",          
        .method   = HTTP_GET,          
        .handler  = power_uri_handler,     
        .user_ctx = NULL      
    };

    const httpd_uri_t speed_up_uri = {
        .uri      = "/speed/up",          
        .method   = HTTP_GET,          
        .handler  = speed_up_uri_handler,     
        .user_ctx = NULL      
    };

    const httpd_uri_t speed_down_uri = {
        .uri      = "/speed/down",          
        .method   = HTTP_GET,          
        .handler  = speed_down_uri_handler,     
        .user_ctx = NULL      
    };

    const httpd_uri_t timer_up_uri = {
        .uri      = "/timer/up",          
        .method   = HTTP_GET,          
        .handler  = timer_up_uri_handler,     
        .user_ctx = NULL      
    };

    const httpd_uri_t timer_down_uri = {
        .uri      = "/timer/down",          
        .method   = HTTP_GET,          
        .handler  = timer_down_uri_handler,     
        .user_ctx = NULL      
    };

    const httpd_uri_t swing_up_uri = {
        .uri      = "/swing/up",          
        .method   = HTTP_GET,          
        .handler  = swing_up_uri_handler,     
        .user_ctx = NULL      
    };

    const httpd_uri_t swing_side_uri = {
        .uri      = "/swing/side",          
        .method   = HTTP_GET,          
        .handler  = swing_side_uri_handler,     
        .user_ctx = NULL      
    };

    const httpd_uri_t no_page_uri = {
        .uri      = "*",          
        .method   = HTTP_GET,          
        .handler  = no_page_handler,     
        .user_ctx = NULL      
    };

    

    httpd_register_uri_handler(server, &power_uri);
    httpd_register_uri_handler(server, &speed_up_uri);
    httpd_register_uri_handler(server, &speed_down_uri);
    httpd_register_uri_handler(server, &timer_up_uri);
    httpd_register_uri_handler(server, &timer_down_uri);
    httpd_register_uri_handler(server, &swing_up_uri);
    httpd_register_uri_handler(server, &swing_side_uri);
    httpd_register_uri_handler(server, &no_page_uri);
}

// Creates and starts HTTP server.
void start_http_server(void) {
    // Creates default HTTP server configuration.
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    
    // Starts HTTP server.
    ESP_ERROR_CHECK(httpd_start(&server, &config));
    ESP_LOGI(TAG, "HTTP server started.");
    
    // Registers all the URI handlers for remote.
    register_uri_handlers(server);
    ESP_LOGI(TAG, "URIs registered to HTTP server.");
}

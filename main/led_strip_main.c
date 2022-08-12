/* RMT example -- RGB LED Strip

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"

static const char *TAG = "example";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)

#define CONFIG_EXAMPLE_RMT_TX_GPIO 19
#define CONFIG_EXAMPLE_STRIP_LED_NUMBER 16
void breath_func(led_strip_t *strip_t);
/**
 * @brief Simple helper function, converting HSV color space to RGB color space
 *
 * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 */
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

void app_main(void)
{
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;
    uint16_t start_rgb = 0;

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_EXAMPLE_STRIP_LED_NUMBER, (led_strip_dev_t)config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    //*strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(strip->clear(strip, 100));
    // Show simple rainbow chasing pattern
    ESP_LOGI(TAG, "LED Rainbow Chase Start");
    while (true) {
        //breath_func(strip);
        for (int i = 0; i < 3; i++) {
            for (int j = i; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j += 3) {
                // Build RGB values
                hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
                led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
                // Write RGB values to strip driver
                ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
            }
            // Flush RGB values to LEDs
            ESP_ERROR_CHECK(strip->refresh(strip, 100));
            vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            strip->clear(strip, 50);
            vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        }
        start_rgb += 60;
    }
}

typedef struct
{
    uint32_t usRed;         /* color R setting  0-1000*/
    uint32_t usGreen;		/* color G setting  0-1000*/
    uint32_t usBlue;		/* color B setting  0-1000*/
}COLOR_RGB_T;

void set_all_func(led_strip_t *strip_t)
{   
    COLOR_RGB_T usColor = {1000,0,0};
    
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        // Build RGB values
        //hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
        //led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
        // Write RGB values to strip driver
        ESP_ERROR_CHECK(strip_t->set_pixel(strip_t, j, usColor.usRed, usColor.usGreen, usColor.usBlue));
    }
    // Flush RGB values to LEDs
    ESP_ERROR_CHECK(strip_t->refresh(strip_t, 100));
    //vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    // strip_t->clear(strip_t, 50);
    // vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
}

void breath_func(led_strip_t *strip_t)
{   
    COLOR_RGB_T usColor = {1000,0,0};
    
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        // Build RGB values
        //hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
        //led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
        // Write RGB values to strip driver
        ESP_ERROR_CHECK(strip_t->set_pixel(strip_t, j, usColor.usRed, usColor.usGreen, usColor.usBlue));
    }
    // Flush RGB values to LEDs
    ESP_ERROR_CHECK(strip_t->refresh(strip_t, 100));
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    strip_t->clear(strip_t, 50);
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));

}

void rainbow_func(led_strip_t *strip_t)
{   
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;
    uint16_t start_rgb = 0;
    COLOR_RGB_T usColor = {1000,0,0};
    
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        // Build RGB values
        hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
        led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
        // Write RGB values to strip driver
        ESP_ERROR_CHECK(strip_t->set_pixel(strip_t, j, red, green, blue));
    }
    // Flush RGB values to LEDs
    ESP_ERROR_CHECK(strip_t->refresh(strip_t, 100));
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    strip_t->clear(strip_t, 50);
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));

    start_rgb += 60;
}

void snake_func(led_strip_t *strip_t)
{   
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;
    uint16_t start_rgb = 0;
    COLOR_RGB_T usColor = {1000,0,0};
    
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        // Build RGB values
        hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
        led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
        // Write RGB values to strip driver
        ESP_ERROR_CHECK(strip_t->set_pixel(strip_t, j, red, green, blue));
    }
    // Flush RGB values to LEDs
    ESP_ERROR_CHECK(strip_t->refresh(strip_t, 100));
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
    strip_t->clear(strip_t, 50);
    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));

    start_rgb += 60;
}
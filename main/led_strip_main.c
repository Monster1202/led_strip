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
#include "string.h"

static const char *TAG = "example";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)

#define CONFIG_EXAMPLE_RMT_TX_GPIO 19
#define CONFIG_EXAMPLE_STRIP_LED_NUMBER 50

typedef struct
{
    uint32_t usRed;         /* color R setting  0-1000*/
    uint32_t usGreen;		/* color G setting  0-1000*/
    uint32_t usBlue;		/* color B setting  0-1000*/
}COLOR_RGB_T;

void breath_func(led_strip_t *strip_t);
void set_all_func(led_strip_t *strip_t);
void rainbow_func(led_strip_t *strip_t);
void snake_func(led_strip_t *strip_t);
void light_set_rgb_all(COLOR_RGB_T usColor,led_strip_t *strip_t);
void light_breathe_mode(COLOR_RGB_T *srcColor,uint8_t colorSum,led_strip_t *strip_t);



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
    COLOR_RGB_T color_inital[4] = {{0,100,200},{0,100,200},{0,100,200},{0,100,200}};
    //COLOR_RGB_T Color_1 = {0,0,0};
    //memcpy(&srcColor[0],&Color_1,SIZEOF(Color_1));
    while (true) {
        //set_all_func(strip);
        light_breathe_mode(color_inital,4,strip);
        //breath_func(strip);
        //rainbow_func(strip);
        //snake_func(strip);
        // for (int i = 0; i < 3; i++) {
        //     for (int j = i; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j += 3) {
        //         // Build RGB values
        //         hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
        //         led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
        //         // Write RGB values to strip driver
        //         ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
        //     }
        //     // Flush RGB values to LEDs
        //     ESP_ERROR_CHECK(strip->refresh(strip, 100));
        //     vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        //     strip->clear(strip, 50);
        //     vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        // }
        // start_rgb += 60;
    }
}



void set_all_func(led_strip_t *strip_t)
{   
    COLOR_RGB_T usColor = {0,200,300};
    
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        // Build RGB values
        //hue = j * 360 / CONFIG_EXAMPLE_STRIP_LED_NUMBER + start_rgb;
        //led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
        // Write RGB values to strip driver
        ESP_ERROR_CHECK(strip_t->set_pixel(strip_t, j, usColor.usRed, usColor.usGreen, usColor.usBlue));
    }
    // Flush RGB values to LEDs
    ESP_ERROR_CHECK(strip_t->refresh(strip_t, 100));
    vTaskDelay(pdMS_TO_TICKS(5000));
    //COLOR_RGB_T usColor = {300,200,0};
    usColor.usRed=300;
    usColor.usGreen=200;
    usColor.usBlue=0;
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        ESP_ERROR_CHECK(strip_t->set_pixel(strip_t, j, usColor.usRed, usColor.usGreen, usColor.usBlue));
    }
    ESP_ERROR_CHECK(strip_t->refresh(strip_t, 100));
    vTaskDelay(pdMS_TO_TICKS(5000));

    // strip_t->clear(strip_t, 50);
    // vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
}

void breath_func(led_strip_t *strip_t)
{   
    // int grad = 10;
    // COLOR_RGB_T usColor = {inital_r,inital_g,inital_b};
    // if(usColor.usRed >= inital_r)
    // usColor.usRed = ;
    // usColor.usGreen = ;
    // usColor.usBlue = ;
    COLOR_RGB_T usColor = {50,100,300};
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

VOID light_set_snake(COLOR_RGB_T *srcColor,uint8_t colorSum,led_strip_t *strip_t)
{
    STATIC USHORT_T i = 0;
    STATIC USHORT_T n = 0;
    COLOR_RGB_T usColor = {0,0,0};
    COLOR_RGB_T rstColor = {0,0,0};
    UINT_T led_cnt;
    
    led_cnt = get_led_sum();
    if(get_scene_reset_flag()){
        i = 0;
        n = 0;
        set_scene_reset_flag(FALSE);
    }
    i = i % colorSum;     //0 1 2 --- colorSum-1
    //PR_DEBUG("led_cnt:%d,i:%d,colorSum:%d",led_cnt,i,colorSum);
    //ClockMode = 1;
    //放入数据
    memcpy(&usColor,&srcColor[i],SIZEOF(usColor));//需要限制，特别是白光时
    //PR_NOTICE("usColor:%d,%d,%d",usColor.usRed,usColor.usGreen,usColor.usBlue);
    if(n < led_cnt){
        //注意下面两个函数的调用顺序，先移位再插入和先插入再移位是不一样的，调用顺序不一样，插入函数参数不一样
        led_shift_cycle_set(ClockMode,0xffff,0xffff,1);
        if(ClockMode == 0){
            led_insert_color_set(&usColor,0,1);
        }else{
            led_insert_color_set(&usColor,led_cnt-1,1);
        }
        light_send_color_data();//亮灯
        n++;
    }else{
        n=0;
        i++;
        //light_set_rgb_all(rstColor);//清空
    }
    // n++;
    // if(n == led_cnt + 2){//2个定时器时间复位熄灭
    //     i++;
    //     n=0;
    // }

}

const uint8_t BreatheWave[142] = {
1,     1,   1,   1,   2,   2,   2,   2,   2,   3, 
3,     3,   3,   4,   4,   5,   5,   6,   6,   7, 
8,     9,  10,  11,  12,  13,  14,  16,  18,  20,  
22,   24,  27,  30,  33,  36,  40,  45,  50,  55, 
61,   67,  75,  83,  88,  92,  97, 102, 108, 113, 119, 125, 132, 138, 146, 153, 162,
170, 179, 188, 194, 199, 208, 216, 220, 224, 231, 238, 243, 247, 255, 255, 247, 243, 238, 231, 224, 220, 216, 208, 199, 194, 188, 179, 170, 162,
153, 146, 138, 132, 125, 119, 113, 108, 102,  97, 92,   88,  83,  75,  67,  61, 
55,   50,  45,  40,  36,  33,  30,  27,  24,  22,
20,   18,  16,  14,  13,  12,  11,  10,   9,   8,
7,     6,   6,   5,   5,   4,   4,   3,   3,   3, 
3,     2,   2,   2,   2,   2,   1,   1,   1,   1
};

void light_breathe_mode(COLOR_RGB_T *srcColor,uint8_t colorSum,led_strip_t *strip_t)
{
    static uint8_t Breathe_Index;
    static uint8_t Breathe_Cnt;
    float Breathe_percentage = 0;
	static uint8_t color_cnt = 0;
    COLOR_RGB_T usColor = {0,0,0};
    //COLOR_RGB_T rstColor = {0,0,0};	

  	if(Breathe_Index > 141)
		Breathe_Index = 0;
    memcpy(&usColor,&srcColor[color_cnt],sizeof(usColor));
    //PR_DEBUG("usColor:%ld,%ld,%ld\r\n",usColor.usRed,usColor.usGreen,usColor.usBlue);
    Breathe_percentage = (float)BreatheWave[Breathe_Index]/255;
    //PR_DEBUG("Breathe_percentage:%f\r\n",Breathe_percentage);

	if(Breathe_Cnt < colorSum)
	{
		usColor.usRed = usColor.usRed*Breathe_percentage;
		usColor.usGreen = usColor.usGreen*Breathe_percentage;
		usColor.usBlue = usColor.usBlue*Breathe_percentage; 	
	}
    else{
        Breathe_Cnt = 0;
        color_cnt = 0;
    }

	if(Breathe_Index < 141)
	    Breathe_Index++;
	else     //one circle    change color
	{
	    Breathe_Index = 0;
		Breathe_Cnt++;
        color_cnt++;
	}
	// rgbData.usRed = (int)origin_rgb_buf[0][0]*1000/255;
	// rgbData.usGreen = (int)origin_rgb_buf[0][1]*1000/255;
	// rgbData.usBlue = (int)origin_rgb_buf[0][2]*1000/255;
	//PR_DEBUG("usColor:%ld,%ld,%ld\r\n",usColor.usRed,usColor.usGreen,usColor.usBlue);
	light_set_rgb_all(usColor,strip_t);		
}


void light_set_rgb_all(COLOR_RGB_T usColor,led_strip_t *strip_t)
{   

    usColor.usRed=300;
    usColor.usGreen=200;
    usColor.usBlue=0;
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        ESP_ERROR_CHECK(strip_t->set_pixel(strip_t, j, usColor.usRed, usColor.usGreen, usColor.usBlue));
    }
    ESP_ERROR_CHECK(strip_t->refresh(strip_t, 100));
    //vTaskDelay(pdMS_TO_TICKS(5000));
}

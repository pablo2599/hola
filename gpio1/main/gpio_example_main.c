/* GPIO Example

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

// ======== PANTALLA ==========================
#include <stdlib.h>
#include "esp_log.h"
#include "ssd1306.h"


char lineChar[16];

#define tag "SSD1306"
SSD1306_t dev;


//==================================




/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 */

#define GPIO_INPUT_IO_0     2    
#define GPIO_INPUT_IO_1     0
#define GPIO_INPUT_IO_2     12  
#define GPIO_INPUT_IO_3     15  
#define GPIO_INPUT_IO_4     13  
#define GPIO_INPUT_IO_5     14   
#define GPIO_INPUT_IO_6     25   
#define GPIO_INPUT_IO_7     26   
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1) | (1ULL<<GPIO_INPUT_IO_2) | (1ULL<<GPIO_INPUT_IO_3) | (1ULL<<GPIO_INPUT_IO_4) | (1ULL<<GPIO_INPUT_IO_5) | (1ULL<<GPIO_INPUT_IO_6) | (1ULL<<GPIO_INPUT_IO_7))
#define ESP_INTR_FLAG_DEFAULT 0



static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{

    uint8_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {

            switch(io_num){
                case 00: 
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s ",CONFIG_ACTIVIDAD_UNO);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,16, false);
                    printf(" %s \n",CONFIG_ACTIVIDAD_UNO);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;
                case 02: 
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s",CONFIG_ACTIVIDAD_DOS);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,strlen(lineChar), false);
                    printf(" %s \n", CONFIG_ACTIVIDAD_DOS);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;
                case 12: 
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s",CONFIG_ACTIVIDAD_TRES);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,strlen(lineChar), false);
                    printf(" %s \n",CONFIG_ACTIVIDAD_TRES);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;
                case 13: 
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s",CONFIG_ACTIVIDAD_CUATRO);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,strlen(lineChar), false);
                    printf(" %s \n", CONFIG_ACTIVIDAD_CUATRO);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;
                case 14: 
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s",CONFIG_ACTIVIDAD_CINCO);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,strlen(lineChar), false);
                    printf(" %s \n", CONFIG_ACTIVIDAD_CINCO);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;	
                case 15: 
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s",CONFIG_ACTIVIDAD_SEIS);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,16, false);
                    printf(" %s \n", CONFIG_ACTIVIDAD_SEIS);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;
                 case 25:  
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s",CONFIG_ACTIVIDAD_SIETE);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,strlen(lineChar), false);
                    printf(" %s \n", CONFIG_ACTIVIDAD_SIETE);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;
                 case 26: 
                lineChar[0] = 0x01;  
                    sprintf(lineChar, "%s",CONFIG_ACTIVIDAD_OCHO);   // imprime display
                    ssd1306_scroll_text(&dev,lineChar,strlen(lineChar), false);
                    printf(" %s \n", CONFIG_ACTIVIDAD_OCHO);
                    vTaskDelay(500 / portTICK_RATE_MS);
                break;



            }
        }
    }
}




void app_main()
{
  


    #if CONFIG_I2C_INTERFACE
      ESP_LOGI(tag, "INTERFACE is i2c");
        ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d", CONFIG_SDA_GPIO);
        ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d", CONFIG_SCL_GPIO);
        ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d", CONFIG_RESET_GPIO);
        i2c_master_init(CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    #if CONFIG_SSD1306_128x64
        ESP_LOGI(tag, "Panel is 128x64");
        i2c_init(&dev, 128, 64, 0x3C);
    #endif // CONFIG_SSD1306_128x64
    #endif // CONFIG_I2C_INTERFACE

    // =====================================================


    gpio_config_t io_conf;
    //disable interrupt
 //   io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
//  io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
//  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;  
    
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode    
   io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
 //   gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(1, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
    gpio_isr_handler_add(GPIO_INPUT_IO_2, gpio_isr_handler, (void*) GPIO_INPUT_IO_2);
    gpio_isr_handler_add(GPIO_INPUT_IO_3, gpio_isr_handler, (void*) GPIO_INPUT_IO_3);
    gpio_isr_handler_add(GPIO_INPUT_IO_4, gpio_isr_handler, (void*) GPIO_INPUT_IO_4);
    gpio_isr_handler_add(GPIO_INPUT_IO_5, gpio_isr_handler, (void*) GPIO_INPUT_IO_5); 
    gpio_isr_handler_add(GPIO_INPUT_IO_6, gpio_isr_handler, (void*) GPIO_INPUT_IO_6);
    gpio_isr_handler_add(GPIO_INPUT_IO_7, gpio_isr_handler, (void*) GPIO_INPUT_IO_7);




        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, 4, "*** COW SCAN ***", 16, true);
        vTaskDelay(1000 / portTICK_RATE_MS);
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_software_scroll(&dev, 7, 1);

    uint8_t i;
    for ( i = strlen(CONFIG_ACTIVIDAD_UNO); i<=15; i++ )
    {
    strcat (CONFIG_ACTIVIDAD_UNO, " " );



   }


}


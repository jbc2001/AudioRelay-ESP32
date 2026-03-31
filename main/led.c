#include "driver/gpio.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "config.h"
#include "led.h"

static SemaphoreHandle_t ledMutex; // Prevents concurrent LED access from multiple tasks

//Initialize the status LED (WS2812)
void InitLED() {
    ledMutex = xSemaphoreCreateMutex(); // Create before first LED call
    // Configure the LED strip (WS2812) on LED_PIN
    led_strip_config_t stripCfg = {
        .strip_gpio_num = LED_PIN,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGB
    };
    // RMT config for WS2812 timing
    led_strip_rmt_config_t rmtCfg = {
        .resolution_hz = 10 * 1000 * 1000   // 10MHz resolution for WS2812
    };
    led_strip_new_rmt_device(&stripCfg, &rmtCfg, &led);
    SetLED(0, 0, 0);
    SetLED(255, 0, 0); // Set LED to red to indicate startup
}

// Set the LED colour, scaling each channel down by LED_BRIGHTNESS_SHIFT to control brightness
void SetLED(uint32_t red, uint32_t green, uint32_t blue) {
    xSemaphoreTake(ledMutex, portMAX_DELAY);
    led_strip_set_pixel(led, 0, red >> LED_BRIGHTNESS_SHIFT, green >> LED_BRIGHTNESS_SHIFT, blue >> LED_BRIGHTNESS_SHIFT);
    led_strip_refresh(led);
    xSemaphoreGive(ledMutex);
}

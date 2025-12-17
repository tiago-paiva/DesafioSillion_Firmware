#include "gpio_input/gpio_input.h"
#include "motor/motor.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_GPIO_PIN GPIO_NUM_5
#define GPIO_ISR_FLAGS  0

static QueueHandle_t gpio_evt_queue;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task(void *arg)
{
    uint32_t gpio_num;

    while (true)
    {
        if (xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY))
        {
            if (gpio_num == BUTTON_GPIO_PIN)
                motor_turn_on();
        }
    }
}

void gpio_input_init(void)
{
    gpio_config_t cfg = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_GPIO_PIN),
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&cfg);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    gpio_install_isr_service(GPIO_ISR_FLAGS);
    gpio_isr_handler_add(
        BUTTON_GPIO_PIN,
        gpio_isr_handler,
        (void *)BUTTON_GPIO_PIN
    );
}

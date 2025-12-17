#include "motor.h"
#include "motor_uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "driver/gpio.h"

#define MOTOR_GPIO_PIN         GPIO_NUM_6
#define MOTOR_TIMEOUT_MS       10000
#define MOTOR_STATUS_PERIOD_MS 1000

static TimerHandle_t timeout_timer;
static TimerHandle_t status_timer;

static uint32_t remaining_ms;

static void status_timer_cb(TimerHandle_t timer)
{
    if (remaining_ms >= MOTOR_STATUS_PERIOD_MS)
        remaining_ms -= MOTOR_STATUS_PERIOD_MS;
    else
        remaining_ms = 0;

    char msg[64];
    snprintf(msg, sizeof(msg),
             "MOTOR ON - restante: %lu ms\n",
             remaining_ms);

    motor_uart_send(msg);
}

static void timeout_timer_cb(TimerHandle_t timer)
{
    gpio_set_level(MOTOR_GPIO_PIN, true);
    xTimerStop(status_timer, 0);

    motor_uart_send("MOTOR OFF - timeout\n");
}

void motor_init(void)
{
    gpio_config_t cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << MOTOR_GPIO_PIN),
    };
    gpio_config(&cfg);

    timeout_timer = xTimerCreate(
        "motor_timeout",
        pdMS_TO_TICKS(MOTOR_TIMEOUT_MS),
        pdFALSE,
        NULL,
        timeout_timer_cb
    );

    status_timer = xTimerCreate(
        "motor_status",
        pdMS_TO_TICKS(MOTOR_STATUS_PERIOD_MS),
        pdTRUE,
        NULL,
        status_timer_cb
    );

    assert(timeout_timer);
    assert(status_timer);
}

void motor_turn_on(void)
{
    gpio_set_level(MOTOR_GPIO_PIN, false);

    remaining_ms = MOTOR_TIMEOUT_MS;

    xTimerStop(timeout_timer, 0);
    xTimerStop(status_timer, 0);

    xTimerStart(timeout_timer, 0);
    xTimerStart(status_timer, 0);
}
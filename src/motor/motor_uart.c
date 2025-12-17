#include "motor_uart.h"

#include "esp_log.h"

#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>

#define MOTOR_UART_PORT        UART_NUM_1
#define MOTOR_UART_TX_PIN      GPIO_NUM_15
#define MOTOR_UART_RX_PIN      GPIO_NUM_16
#define MOTOR_UART_BAUDRATE    115200
#define MOTOR_UART_BUFFER_SIZE 256

void motor_uart_init(void)
{
    const uart_config_t config = {
        .baud_rate  = MOTOR_UART_BAUDRATE,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_driver_install(
        MOTOR_UART_PORT,
        MOTOR_UART_BUFFER_SIZE,
        0,
        0,
        NULL,
        0
    );

    uart_param_config(MOTOR_UART_PORT, &config);

    uart_set_pin(
        MOTOR_UART_PORT,
        MOTOR_UART_TX_PIN,
        MOTOR_UART_RX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    );
}

void motor_uart_send(const char *msg)
{
    uart_write_bytes(MOTOR_UART_PORT, msg, strlen(msg));
    ESP_LOGI("MOTOR_UART", "%s", msg);
}
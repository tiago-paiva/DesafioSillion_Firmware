#include "motor/motor.h"
#include "motor/motor_uart.h"
#include "gpio_input/gpio_input.h"

void app_main(void)
{
    motor_uart_init();
    motor_init();
    gpio_input_init();
}

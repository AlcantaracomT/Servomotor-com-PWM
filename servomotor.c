#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_SERVO 22  

#define WRAP 20000  
#define PWM_DIVISOR 125.0      

#define GRAU_0 500  
#define GRAU_90 1467  
#define GRAU_180 2399

void pwm_setup(uint gpio) 
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);  
    uint slice = pwm_gpio_to_slice_num(gpio);  
    pwm_set_clkdiv(slice, PWM_DIVISOR);  
    pwm_set_wrap(slice, WRAP);  
    pwm_set_enabled(slice, true);  
}

void servo_posicao(uint gpio, uint16_t pulse_width) 
{
    pwm_set_gpio_level(gpio, (pulse_width * WRAP) / 20000);
}

void move_Suave(uint gpio, uint16_t start, uint16_t end, uint16_t step, uint16_t delay) 
{
    for (uint16_t pos = start; (start < end) ? (pos <= end) : (pos >= end); pos += (start < end) ? step : -step) 
    {
        servo_posicao(gpio, pos);
        sleep_ms(delay);
    }
}

int main() 
{
    stdio_init_all();
    pwm_setup(PWM_SERVO);  

    while (1) 
    {
        servo_posicao(PWM_SERVO, GRAU_180);
        sleep_ms(5000);

        servo_posicao(PWM_SERVO, GRAU_90);
        sleep_ms(5000);
        
        servo_posicao(PWM_SERVO, GRAU_0);
        sleep_ms(5000);

        move_Suave(PWM_SERVO, GRAU_0, GRAU_180, 5, 10);
        move_Suave(PWM_SERVO, GRAU_180, GRAU_0, 5, 10);
    }
}

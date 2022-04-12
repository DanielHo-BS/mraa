/*
 * Author: Brendan Le Foll
 * Contributors: Alex Tereschenko <alext.mkrs@gmail.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2014 Intel Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Example usage: Configures GPIO pin for interrupt and turn LED on/off.
 *                Press Ctrl+C to exit
 */

/* standard headers */
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"
#include "mraa/led.h"

/* GPIO default */
#define GPIO_PIN1 5

/* LED default */
#define LED_NUM1 0

volatile int isr_occurred;

void int_handler() {
    fprintf(stdout, "ISR triggered\n");
    isr_occurred = 1;
}

void isr_do(mraa_led_context led1) {
    /* turn LED on/off 10 times depending on max_brightness value */
    for(int i =0; i < 10; i++) {
        mraa_led_set_brightness(led1, 1);
        usleep(50000);
        mraa_led_set_brightness(led1, 0);
        usleep(50000);
    }
}


int main(int argc, char *argv[]) {
    mraa_result_t status = MRAA_SUCCESS;
    mraa_gpio_context gpio;
    mraa_led_context led1;
    int gpio_pin1, led_num1;
    gpio_pin1 = (argc >= 2)?atoi(argv[1]):GPIO_PIN1;
    led_num1 = (argc >= 3)?atoi(argv[2]):LED_NUM1;

    
    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();

    //! [Interesting]
    /* initialize GPIO pin */
    gpio = mraa_gpio_init(GPIO_PIN1);
    /* Wait for the GPIO to initialize */
    usleep(5000);
    if (gpio == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", GPIO_PIN1);
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* initialize LED */
    led1 = mraa_led_init(led_num1);
    usleep(5000);;
    if (led1 == NULL) {
        fprintf(stderr, "Failed to initialize LED 1\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* set GPIO1 to input */
    status = mraa_gpio_dir(gpio, MRAA_GPIO_IN);
    usleep(5000);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "Failed to set GPIO1 to input\n");
        goto err_exit;
    }

    /* configure ISR for GPIO */
    status = mraa_gpio_isr(gpio, MRAA_GPIO_EDGE_RISING, &int_handler, NULL);
    /* Wait for the ISR to configure */
    usleep(5000);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "Failed to configure ISR\n");
        goto err_exit;
    }
    
    int c = 0;
    while(c < 10) { 
        if (isr_occurred){ // When ISR occurred
            isr_occurred = 0; // Clear the flag of interrupt
            isr_do(led1);
            c = c + 1;
        }
    }

    /* turn off LED*/
    mraa_led_set_brightness(led1, 0);
    usleep(5000);

    /* close GPIO */
    mraa_gpio_close(gpio);

    /* close LED */
    mraa_led_close(led1);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_SUCCESS;

err_exit:
    mraa_result_print(status);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_FAILURE;
}
/*
 * Author: Brendan Le Foll
 * Contributors: Alex Tereschenko <alext.mkrs@gmail.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2014 Intel Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Example usage: Configures GPIO pin for interrupt and waits 30 seconds for the isr to trigger
 *
 */

/* standard headers */
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"
#include "mraa/led.h"

/* GPIO default */
#define GPIO_PIN1 5
#define GPIO_PIN2 6

/* LED default value */
#define LED_NUM1   0
#define LED_NUM2   1
#define LED_ON_OFF 0

/* trigger type */
#define LED_TRIGGER "heartbeat"

volatile int isr_occurred;

void int_handler() {
    fprintf(stdout, "ISR triggered\n");
    isr_occurred = 1;
}

void isr_do(mraa_led_context led1, mraa_led_context led2, int led_on_off, int hz, int wait_idle){
    for(int i =0; i < hz; i++) {
        /* turn LED on/off depending on max_brightness value */
        mraa_led_set_brightness(led1, led_on_off);
        mraa_led_set_brightness(led2, led_on_off);
        usleep(wait_idle);
        led_on_off = !led_on_off;
    }
}


int main() {
    mraa_result_t status = MRAA_SUCCESS;
    mraa_gpio_context gpio, gpio_2;
    mraa_led_context led1, led2;
    int led_num1, led_num2, led_on_off;
    led_num1 = LED_NUM1;
    led_num2 = LED_NUM2;
    led_on_off = LED_ON_OFF;

    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();

    //! [Interesting]
    /* initialize GPIO pin */
    gpio = mraa_gpio_init(GPIO_PIN1);
    /* Wait for the GPIO to initialize*/
    usleep(5000);
    if (gpio == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", GPIO_PIN1);
        mraa_deinit();
        return EXIT_FAILURE;
    }

    gpio_2 = mraa_gpio_init(GPIO_PIN2);
    /* Wait for the GPIO to initialize*/
    usleep(5000);
    if (gpio_2 == NULL) {
        fprintf(stderr, "Failed to initialize GPIO %d\n", GPIO_PIN2);
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
    led2 = mraa_led_init(led_num2);
    usleep(5000);
    if (led2 == NULL) {
        fprintf(stderr, "Failed to initialize LED 2\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }

    fprintf(stdout,"Success to initialize LED and GPIO!\n");

    /* set GPIO1 to input */
    status = mraa_gpio_dir(gpio, MRAA_GPIO_IN);
    usleep(5000);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }
    /* set GPIO2 to output */
    status = mraa_gpio_dir(gpio_2, MRAA_GPIO_OUT);
    usleep(5000);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }
    fprintf(stdout,"Success to set dir!\n");

    /* configure ISR for GPIO */
    status = mraa_gpio_isr(gpio, MRAA_GPIO_EDGE_RISING, &int_handler, NULL);
    /* Wait for the ISR to configure*/
    usleep(5000);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }
    fprintf(stdout,"Success to configure ISR!\n");

    
    int c = 0;
    int hz = 10;
    int min_fsync_interval = 5000;
    int wait_idle = (1000000-min_fsync_interval)/hz;
    while(c < 20) { // tirger 20 times
        status = mraa_gpio_write(gpio_2, 1);
            if (status != MRAA_SUCCESS) {
                goto err_exit;
            }
        usleep(min_fsync_interval);
        status = mraa_gpio_write(gpio_2, 0);
            if (status != MRAA_SUCCESS) {
                goto err_exit;
            }
        
        if (isr_occurred){
                isr_occurred = 0;
                isr_do(led1, led2, led_on_off, hz, wait_idle);
        }
        c = c + 1;
    }
    /* turn off LED*/
    mraa_led_set_brightness(led1, 0);
    mraa_led_set_brightness(led2, 0);
    usleep(5000);
    /* close GPIO */
    mraa_gpio_close(gpio);
    mraa_gpio_close(gpio_2);

    /* close LED */
    mraa_led_close(led1);
    mraa_led_close(led2);


    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();
    sleep(1);

    return EXIT_SUCCESS;

err_exit:
    mraa_result_print(status);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_FAILURE;
}
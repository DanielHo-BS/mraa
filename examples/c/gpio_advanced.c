/*
 * Author: Brendan Le Foll
 * Contributors: Alex Tereschenko <alext.mkrs@gmail.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2014 Intel Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Example usage: Configures GPIO pin for interrupt.
 *                Press Ctrl+C to exit.
 */

/* standard headers */
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"
#include "mraa/led.h"

/* GPIO default */
#define GPIO_PIN1 5

int count = 0;

void int_handler(void *arg) {
    mraa_gpio_context gpio = (mraa_gpio_context)arg;
    count += 1;
    printf("pin %d = %d\n",mraa_gpio_get_pin(gpio),  mraa_gpio_read(gpio));
    printf("count = %d\n", count);
}

int main(int argc, char *argv[]) {
    mraa_result_t status = MRAA_SUCCESS;
    mraa_gpio_context gpio;
    int gpio_pin1;
    gpio_pin1 = (argc >= 2)?atoi(argv[1]):GPIO_PIN1;

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

    /* set GPIO1 to input */
    status = mraa_gpio_dir(gpio, MRAA_GPIO_IN);
    usleep(5000);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "Failed to set GPIO1 to input\n");
        goto err_exit;
    }

    /* configure ISR for GPIO */
    status = mraa_gpio_isr(gpio, MRAA_GPIO_EDGE_BOTH, &int_handler, (void *)gpio);
    /* Wait for the ISR to configure */
    usleep(5000);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "Failed to configure ISR\n");
        goto err_exit;
    }

    sleep(30);

    /* close GPIO */
    mraa_gpio_close(gpio);

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
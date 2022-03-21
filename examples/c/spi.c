/*
 * Author: Michael Ring <mail@michael-ring.org>
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2014 Intel Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Example usage: Display set of patterns on MAX7219 repeately.
 *                Press Ctrl+C to exit
 */

/* standard headers */
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/spi.h"

/* SPI declaration */
#define SPI_BUS 0

/* SPI frequency in Hz */
#define SPI_FREQ 100000

/* SPI bits per word*/
#define SPI_BIT 16

volatile sig_atomic_t flag = 1;

void
sig_handler(int signum)
{
    if (signum == SIGINT) {
        fprintf(stdout, "Exiting...\n");
        flag = 0;
    }
}

int
main(int argc, char** argv)
{
    mraa_result_t status = MRAA_SUCCESS;
    mraa_spi_context spi;
    int i, j;
    unsigned int tx_data[2];
    int spi_bus, spi_freq, spi_bit;
    spi_bus = (argc >= 2)?atoi(argv[1]):SPI_BUS;
    spi_freq = (argc >= 3)?atoi(argv[2]):SPI_FREQ;
    spi_bit =  (argc >= 4)?atoi(argv[3]):SPI_BIT;
    printf("Will set SPI BUS to %d, frequency to %d hz and bits per word to %d.\n", spi_bus, spi_freq, spi_bit);

    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();
    usleep(50000);

    //! [Interesting]
    /* initialize SPI bus */
    spi = mraa_spi_init(spi_bus);
    usleep(50000);
    if (spi == NULL) {
        fprintf(stderr, "Failed to initialize SPI\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* set SPI frequency */
    status = mraa_spi_frequency(spi, spi_freq);
    usleep(50000);
    if (status != MRAA_SUCCESS){
        goto err_exit;
    }
        

    /* set big endian mode */
    status = mraa_spi_lsbmode(spi, 0);
    usleep(50000);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    /* set the data in word size */
    status = mraa_spi_bit_per_word(spi, spi_bit);
    usleep(50000);
    if (status != MRAA_SUCCESS) {
        fprintf(stdout, "Failed to set SPI Device to 8Bit mode\n");
        goto err_exit;
    }
    
    /* Take MAX7219 chip with LED Matrix as example.
       We are writing 0x00 to address 0x09 on the chip to brigh the LED Matrix. 
    */
    
    printf("Enter the 2 byte data to sned without \"0x\" \n");
    printf("First Byte: ");
    scanf("%x", &tx_data[0]);
    printf("Second Byte: ");
    scanf("%x", &tx_data[1]);
    
    mraa_spi_write_word(spi, tx_data[0]);  
    usleep(50000);
    mraa_spi_write_word(spi, tx_data[1]);  
    usleep(50000);
    
    /* stop spi */
    mraa_spi_stop(spi);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_SUCCESS;

err_exit:
    mraa_result_print(status);

    /* stop spi */
    mraa_spi_stop(spi);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_FAILURE;
}

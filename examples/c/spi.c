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

uint16_t pat[] = { 0xaa01, 0x5502, 0xaa03, 0x5504, 0xaa05, 0x5506, 0xaa07, 0x5508 };
uint16_t pat_inv[] = { 0x5501, 0xaa02, 0x5503, 0xaa04, 0x5505, 0xaa06, 0x5507, 0xaa08 };
uint16_t pat_clear[] = { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008 };

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
    printf("Will set SPI BUS to %d, frequency to %d hz .\n", spi_bus, spi_freq);

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
    status = mraa_spi_bit_per_word(spi, SPI_BIT);
    usleep(50000);
    if (status != MRAA_SUCCESS) {
        fprintf(stdout, "Failed to set SPI Device to 16 Bit mode\n");
        goto err_exit;
    }

    /* do not decode bits */
    mraa_spi_write_word(spi, 0x0009);  //0x9 is for Decode mode, an 8x8 LEDs should be set to 0x00
    usleep(50000);
    
    /* brightness of LEDs */
    mraa_spi_write_word(spi, 0x050a);  //0xA is for intersity, could be set from 0x0 to 0xF and 0xF is the brightest.
    usleep(50000);
    

    /* show all scan lines */
    mraa_spi_write_word(spi, 0x070b);  //0xB is for Scan, means how many EEPROM you want to open. 
    usleep(50000);

    /* set display on */
    mraa_spi_write_word(spi, 0x010c); //0xC: set to 0x0 for shutdown, 0x1 for open.
    usleep(50000);

    /* testmode off */
    mraa_spi_write_word(spi, 0x000f); //0xF is for test mode, if set to 0x0 every LED will set bright.
    usleep(50000);

    while (flag) {
        /* set display pattern */
        for(i= 0 ; i < sizeof(pat); i++)
        {
            mraa_spi_write_word(spi, pat[i]);
            usleep(50000);
        }
        sleep(1);

        /* set inverted display pattern */
        for(i = 0; i < sizeof(pat_inv); i++)
        {
            mraa_spi_write_word(spi, pat_inv[i]);
            usleep(50000);
        }
        sleep(1);

        /* clear the LED's */
        for(i = 0; i < sizeof(pat_clear); i++)
        {
            mraa_spi_write_word(spi, pat_clear[i]);
        }

        /* cycle through all LED's */
        for (i = 1; i <= 8; i++) {
            for (j = 0; j < 8; j++) {
                mraa_spi_write_word(spi, i  + (1 << (j+8)));
                usleep(50000);
            }
            mraa_spi_write_word(spi, i << 8);
        }
    }

    /* stop spi */
    mraa_spi_stop(spi);
    usleep(50000);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();
    usleep(50000);

    return EXIT_SUCCESS;

err_exit:
    mraa_result_print(status);

    /* stop spi */
    mraa_spi_stop(spi);
    usleep(50000);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();
    usleep(50000);
    return EXIT_FAILURE;
}

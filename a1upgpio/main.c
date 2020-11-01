/*
 * a1upgpio
 *
 * Copyright 2020 Jason Nelson - print high/active pins
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */


#include <stdio.h>

#include "gpio_lib.h"

#define PIN4    SUNXI_GPB(0)
#define PIN5    SUNXI_GPB(1)
#define PIN6    SUNXI_GPB(2)
#define PIN7    SUNXI_GPB(3)
#define PIN8    SUNXI_GPB(4)
#define PIN9    SUNXI_GPB(10)
#define PIN10   SUNXI_GPB(15)
#define PIN11   SUNXI_GPB(16)

#define PIN12   SUNXI_GPC(0)
#define PIN13   SUNXI_GPC(1)
#define PIN14   SUNXI_GPC(2)
#define PIN15   SUNXI_GPC(3)
#define PIN16   SUNXI_GPC(4)
#define PIN17   SUNXI_GPC(5)
#define PIN18   SUNXI_GPC(6)
#define PIN19   SUNXI_GPC(7)
#define PIN20   SUNXI_GPC(8)
#define PIN21   SUNXI_GPC(9)
#define PIN22   SUNXI_GPC(10)
#define PIN23   SUNXI_GPC(11)
#define PIN24   SUNXI_GPC(12)
#define PIN25   SUNXI_GPC(13)
#define PIN26   SUNXI_GPC(14)
#define PIN27   SUNXI_GPC(15)
#define PIN28   SUNXI_GPC(19)

#define PIN29   SUNXI_GPE(4)
#define PIN30   SUNXI_GPE(5)
#define PIN31   SUNXI_GPE(6)
#define PIN32   SUNXI_GPE(7)
#define PIN33   SUNXI_GPE(8)
#define PIN34   SUNXI_GPE(9)
#define PIN35   SUNXI_GPE(10)
#define PIN36   SUNXI_GPE(11)

#define PIN37   SUNXI_GPG(9)
#define PIN38   SUNXI_GPG(10)
#define PIN39   SUNXI_GPG(11)

#define MISO    SUNXI_GPE(3)
#define MOSI    SUNXI_GPE(2)
#define SCK     SUNXI_GPE(1)
#define CS      SUNXI_GPE(0)

int input_pins[] = {
PIN4,
PIN5,
PIN6,
PIN7,
PIN8,
PIN9,
PIN10,
PIN11,
PIN12,
PIN13,
PIN14,
PIN15,
PIN16,
PIN17,
PIN18,
PIN19,
PIN20,
PIN21,
PIN22,
PIN23,
PIN24,
PIN25,
PIN26,
PIN27,
PIN28,
PIN29,
PIN30,
PIN31,
PIN32,
PIN33,
PIN34,
PIN35,
PIN36,

PIN37,
PIN38,
PIN39,
};

int pinstate[] = {
PIN4,
PIN5,
PIN6,
PIN7,
PIN8,
PIN9,
PIN10,
PIN11,
PIN12,
PIN13,
PIN14,
PIN15,
PIN16,
PIN17,
PIN18,
PIN19,
PIN20,
PIN21,
PIN22,
PIN23,
PIN24,
PIN25,
PIN26,
PIN27,
PIN28,
PIN29,
PIN30,
PIN31,
PIN32,
PIN33,
PIN34,
PIN35,
PIN36,

PIN37,
PIN38,
PIN39,
};

int debug = 0;

int pinValue(int gpio) {
    int result;

    if(sunxi_gpio_get_cfgpin(gpio) != SUNXI_GPIO_INPUT) {
        if(debug) printf("GPIO is not an input\r\n");
        return -1;
    }

    result = sunxi_gpio_input(gpio);

    if(result == -1) {
        if(debug) printf("Reading pin failed\r\n");
        return -1;
    }

    return result;
}

int main(int argc, char * argv[]){
    int result;
    int loop = 0;

    result = sunxi_gpio_init();
    if(result == SETUP_DEVMEM_FAIL) {
        if(debug) printf("No access to /dev/mem. Try running as root!\r\n");
        return SETUP_DEVMEM_FAIL;
    }
    else if(result == SETUP_MALLOC_FAIL) {
        if(debug) printf("malloc Failure!!\r\n");
        return SETUP_MALLOC_FAIL;
    }
    else if(result == SETUP_MMAP_FAIL) {
        if(debug) printf("Mmap failed on module import\r\n");
        return SETUP_MMAP_FAIL;
    }
    
    if (argc > 1) {
        if (argv[1] == 'l') loop = 1;
    }

    do {
        for (int i = 0; i < (sizeof(input_pins)/ sizeof(input_pins[0])); i++) {
            int pin_status = pinValue(input_pins[i]);
            if (pin_status >= 0) {
                if (loop) {
                  if (pinstate[i] != pin_status) {
                      printf("PIN%i:%i\r\n", i + 4, pin_status);
                      pinstate[i] = pin_status;
                  }
                }
                else {
                    printf("PIN%i\r\n", i + 4);
                }
            }
        }
    } 
    while(loop);

    // cleanup
    sunxi_gpio_cleanup();
}

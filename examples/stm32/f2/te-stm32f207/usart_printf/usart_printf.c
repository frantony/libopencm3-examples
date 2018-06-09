/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

int _write(int file, char *ptr, int len);

static void clock_setup(void)
{
	/* Enable clocks on all the peripherals we are going to use. */
	rcc_periph_clock_enable(RCC_USART6);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
}

static void usart_setup(void)
{
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6);
	gpio_set_af(GPIOC, GPIO_AF8, GPIO6 | GPIO7);

	/* Setup UART parameters. */
	usart_set_baudrate(USART6, 9600);
	usart_set_databits(USART6, 8);
	usart_set_stopbits(USART6, USART_STOPBITS_1);
	usart_set_parity(USART6, USART_PARITY_NONE);
	usart_set_flow_control(USART6, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART6, USART_MODE_TX);

	/* Finally enable the USART. */
	usart_enable(USART6);
}

static void gpio_setup(void)
{
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_MODE_OUTPUT, GPIO9);
	gpio_set(GPIOB, GPIO9);
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART6, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}

int main(void)
{
	int counter = 0;
	float fcounter = 0.0;
	double dcounter = 0.0;

	clock_setup();
	gpio_setup();
	usart_setup();

	/*
	 * Write Hello World an integer, float and double all over
	 * again while incrementing the numbers.
	 */
	while (1) {
		gpio_toggle(GPIOB, GPIO9);
		printf("Hello World! %i %f %f\r\n", counter, fcounter,
		       dcounter);
		counter++;
		fcounter += 0.01;
		dcounter += 0.01;
	}

	return 0;
}

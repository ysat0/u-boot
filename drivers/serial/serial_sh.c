/*
 * SuperH SCIF device driver.
 * Copyright (C) 2007,2008,2010 Nobuhiro Iwamatsu
 * Copyright (C) 2002 - 2008  Paul Mundt
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/processor.h>
#include "serial_sh.h"

#if defined(CONFIG_CONS_SCIF0)
# define SCIF_BASE	SCIF0_BASE
#elif defined(CONFIG_CONS_SCIF1)
# define SCIF_BASE	SCIF1_BASE
#elif defined(CONFIG_CONS_SCIF2)
# define SCIF_BASE	SCIF2_BASE
#elif defined(CONFIG_CONS_SCIF3)
# define SCIF_BASE	SCIF3_BASE
#elif defined(CONFIG_CONS_SCIF4)
# define SCIF_BASE	SCIF4_BASE
#elif defined(CONFIG_CONS_SCIF5)
# define SCIF_BASE	SCIF5_BASE
#else
# error "Default SCIF doesn't set....."
#endif

#if defined(CONFIG_SCI)
# define PORT_TYPE	PORT_SCI
#elif defined(CONFIG_SCIF_A)
# define PORT_TYPE	PORT_SCIFA
#else
# define PORT_TYPE	PORT_SCIF
#endif

static struct uart_port sh_sci = {
	.membase	= (unsigned char*)SCIF_BASE,
	.mapbase	= SCIF_BASE,
	.type		= PORT_TYPE,
};

/* TDRE / RDRF emulation for RX610 */
/* RX610's SCI don't have TDRE and RDRF in SSR
   This part emulate these flags of IR */
#if defined(CONFIG_CPU_RX610)

#define port_to_irq(port) ((((port)->mapbase - 0x88240) / 8) + 214)
#define read_ir(irq) __raw_readb((unsigned char *)(0x00087000 + (irq)))
#define clear_ir(irq) __raw_writeb(0, (unsigned char *)(0x00087000 + (irq)))

#define write_ier(flg, irq)			\
do { \
	unsigned char ier = __raw_readb((unsigned char *)(0x00087200 + ((irq) >> 3))); \
	ier &= ~(1 << ((irq) & 7));					\
	if (flg) \
		ier |= (1 << ((irq) & 7));			       \
	__raw_writeb(ier, (unsigned char *)(0x00087200 + ((irq) >> 3))); \
} while(0)


static unsigned int sci_SCxSR_in(struct uart_port *port)
{
	int irq = port_to_irq(port);
	unsigned char ssr;
	ssr = __raw_readb(port->membase + 4);
	ssr &= ~0xc0;
	/* map to RXI -> RDRF and TXI -> TDRE */
	ssr |= read_ir(irq + 1) << 6 | read_ir(irq + 2) << 7;
	return ssr;
}

static void sci_SCxSR_out(struct uart_port *port, unsigned int value)
{
	int irq = port_to_irq(port);
	/* clear ir */
	if ((value & 0x84) != 0x84)
		clear_ir(irq + 2);
	if ((value & 0x40) == 0)
		clear_ir(irq + 1);
	value |= 0xc0;		/* b7 and b6 is always 1 */
	value &= ~0x01;		/* b0 is always 0 */
	__raw_writeb(value, port->membase + 4);
}

static void sci_SCSCR_out(struct uart_port *port, unsigned int value)
{
	int irq = port_to_irq(port);
	write_ier(value & 0x40, irq + 1);
	write_ier(value & 0x80, irq + 2);
	/* TXI and RXI always enabled */
	__raw_writeb(value | 0xc0, port->membase + 2);
}
#endif

void serial_setbrg(void)
{
	DECLARE_GLOBAL_DATA_PTR;
	sci_out(&sh_sci, SCBRR, SCBRR_VALUE(gd->baudrate, CONFIG_SYS_CLK_FREQ));
}

int serial_init(void)
{
	sci_out(&sh_sci, SCSCR , SCSCR_INIT(&sh_sci));
	sci_out(&sh_sci, SCSCR , SCSCR_INIT(&sh_sci));
	sci_out(&sh_sci, SCSMR, 0);
	sci_out(&sh_sci, SCSMR, 0);
	sci_out(&sh_sci, SCFCR, SCFCR_RFRST|SCFCR_TFRST);
	sci_in(&sh_sci, SCFCR);
	sci_out(&sh_sci, SCFCR, 0);

	serial_setbrg();
	return 0;
}

#if defined(CONFIG_CPU_SH7760) || \
	defined(CONFIG_CPU_SH7780) || \
	defined(CONFIG_CPU_SH7785) || \
	defined(CONFIG_CPU_SH7786)
static int scif_rxfill(struct uart_port *port)
{
	return sci_in(port, SCRFDR) & 0xff;
}
#elif defined(CONFIG_CPU_SH7763)
static int scif_rxfill(struct uart_port *port)
{
	if ((port->mapbase == 0xffe00000) ||
		(port->mapbase == 0xffe08000)) {
		/* SCIF0/1*/
		return sci_in(port, SCRFDR) & 0xff;
	} else {
		/* SCIF2 */
		return sci_in(port, SCFDR) & SCIF2_RFDC_MASK;
	}
}
#elif defined(CONFIG_ARCH_SH7372)
static int scif_rxfill(struct uart_port *port)
{
	if (port->type == PORT_SCIFA)
		return sci_in(port, SCFDR) & SCIF_RFDC_MASK;
	else
		return sci_in(port, SCRFDR);
}
#elif defined(CONFIG_SCI)
static int scif_rxfill(struct uart_port *port)
{
	return (sci_in(port, SCxSR) & SCxSR_RDxF(port))?1:0;
}
#else
static int scif_rxfill(struct uart_port *port)
{
	return sci_in(port, SCFDR) & SCIF_RFDC_MASK;
}
#endif

static int serial_rx_fifo_level(void)
{
	return scif_rxfill(&sh_sci);
}

void serial_raw_putc(const char c)
{
	while (1) {
		/* Tx fifo is empty */
		if (sci_in(&sh_sci, SCxSR) & SCxSR_TEND(&sh_sci))
			break;
	}

	sci_out(&sh_sci, SCxTDR, c);
	sci_out(&sh_sci, SCxSR, sci_in(&sh_sci, SCxSR) & ~(SCxSR_TEND(&sh_sci) | SCxSR_TDxE(&sh_sci)) );
}

void serial_putc(const char c)
{
	if (c == '\n')
		serial_raw_putc('\r');
	serial_raw_putc(c);
}

void serial_puts(const char *s)
{
	char c;
	while ((c = *s++) != 0)
		serial_putc(c);
}

int serial_tstc(void)
{
	return serial_rx_fifo_level() ? 1 : 0;
}

void handle_error(void)
{
	sci_in(&sh_sci, SCxSR);
	sci_out(&sh_sci, SCxSR, SCxSR_ERROR_CLEAR(&sh_sci));
	sci_in(&sh_sci, SCLSR);
	sci_out(&sh_sci, SCLSR, 0x00);
}

int serial_getc_check(void)
{
	unsigned short status;

	status = sci_in(&sh_sci, SCxSR);

	if (status & SCxSR_ERRORS(&sh_sci))
		handle_error();
	if (sci_in(&sh_sci, SCLSR) & SCxSR_ORER(&sh_sci))
		handle_error();
	return status & (SCIF_DR | SCxSR_RDxF(&sh_sci));
}

int serial_getc(void)
{
	unsigned short status;
	char ch;

	while (!serial_getc_check())
		;

	ch = sci_in(&sh_sci, SCxRDR);
	status = sci_in(&sh_sci, SCxSR);

	sci_out(&sh_sci, SCxSR, SCxSR_RDxF_CLEAR(&sh_sci));

	if (status & SCIF_ERRORS)
			handle_error();

	if (sci_in(&sh_sci, SCLSR) & SCxSR_ORER(&sh_sci))
		handle_error();
	return ch;
}

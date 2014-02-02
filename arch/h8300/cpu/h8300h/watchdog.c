/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/processor.h>
#include <asm/io.h>

#define WDT_BASE	0xffff8c

#define WDT_WD		(1 << 6)
#define WDT_ENABLE	(1 << 5)
#define WDT_CKS4096	(7)

#if defined(CONFIG_WATCHDOG)
static inline unsigned char tcsr_read(void)
{
	return inb(WDT_BASE);
}

static inline void tcnt_write(unsigned char value)
{
	outw((unsigned short)value | 0x5A00, WDT_BASE);
}

static inline void tcsr_write(unsigned char value)
{
	outw((unsigned short)value | 0xA500, WDT_BASE);
}

void watchdog_reset(void)
{
	tcnt_write(0);
}

int watchdog_init(void)
{
	/* Set overflow time*/
	tcnt_write(0);
	/* Power on reset */
	tcsr_write(WDT_WD|WDT_ENABLE|WDT_CKS4096);

	return 0;
}

int watchdog_disable(void)
{
	tcsr_write(tcsr_read() & ~WDT_ENABLE);
	return 0;
}
#endif

void reset_cpu(unsigned long ignored)
{
	asm volatile("jmp @@0");
}

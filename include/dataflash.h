#ifdef CONFIG_SYS_NO_FLASH

/*-----------------------------------------------------------------------
 * return codes from flash_write():
 */
# define ERR_OK				0
# define ERR_TIMOUT			1
# define ERR_NOT_ERASED			2
# define ERR_PROTECTED			4
# define ERR_INVAL			8
# define ERR_ALIGN			16
# define ERR_UNKNOWN_FLASH_VENDOR	32
# define ERR_UNKNOWN_FLASH_TYPE		64
# define ERR_PROG_ERROR			128

/*-----------------------------------------------------------------------
 * Protection Flags for flash_protect():
 */
# define FLAG_PROTECT_SET		0x01
# define FLAG_PROTECT_CLEAR		0x02
# define FLAG_PROTECT_INVALID		0x03

/* DataFlash return value */
#define DATAFLASH_BUSY			0x00
#define DATAFLASH_OK			0x01
#define DATAFLASH_ERROR			0x02
#define DATAFLASH_MEMORY_OVERFLOW	0x03
#define DATAFLASH_BAD_COMMAND		0x04
#define DATAFLASH_BAD_ADDRESS		0x05

/*-----------------------------------------------------------------------
 * Set Environment according to label:
 */
# define	FLAG_SETENV		0x80
#endif /* CONFIG_SYS_NO_FLASH */

#include <asm/arch/dataflash.h>

extern int addr2ram(ulong addr);
extern int dataflash_real_protect (int flag, unsigned long start_addr, unsigned long end_addr);
extern int addr_dataflash (unsigned long addr);
extern int read_dataflash (unsigned long addr, unsigned long size, char *result);
extern int write_dataflash (unsigned long addr, unsigned long dest, unsigned long size);
extern void dataflash_print_info (void);
extern void dataflash_perror (int err);

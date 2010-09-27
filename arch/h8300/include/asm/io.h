#ifndef __ASM_H8300_IO_H
#define __ASM_H8300_IO_H

#ifdef __KERNEL__

#include <linux/types.h>
#include <asm/byteorder.h>

/*
 * Generic virtual read/write.  Note that we don't support half-word
 * read/writes.  We define __arch_*[bl] here, and leave __arch_*w
 * to the architecture specific code.
 */
#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))

#define __arch_putb(v, a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v, a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v, a)		(*(volatile unsigned int *)(a) = (v))

extern void __raw_writesb(unsigned int addr, const void *data, int bytelen);
extern void __raw_writesw(unsigned int addr, const void *data, int wordlen);
extern void __raw_writesl(unsigned int addr, const void *data, int longlen);

extern void __raw_readsb(unsigned int addr, void *data, int bytelen);
extern void __raw_readsw(unsigned int addr, void *data, int wordlen);
extern void __raw_readsl(unsigned int addr, void *data, int longlen);

#define __raw_writeb(v, a)		__arch_putb(v, a)
#define __raw_writew(v, a)		__arch_putw(v, a)
#define __raw_writel(v, a)		__arch_putl(v, a)

#define __raw_readb(a)			__arch_getb(a)
#define __raw_readw(a)			__arch_getw(a)
#define __raw_readl(a)			__arch_getl(a)

/*
 * The compiler seems to be incapable of optimising constants
 * properly.  Spell it out to the compiler in some cases.
 * These are only valid for small values of "off" (< 1<<12)
 */
#define __raw_base_writeb(val, base, off)	__arch_base_putb(val, base, off)
#define __raw_base_writew(val, base, off)	__arch_base_putw(val, base, off)
#define __raw_base_writel(val, base, off)	__arch_base_putl(val, base, off)

#define __raw_base_readb(base, off)	__arch_base_getb(base, off)
#define __raw_base_readw(base, off)	__arch_base_getw(base, off)
#define __raw_base_readl(base, off)	__arch_base_getl(base, off)

#define outb(v, p)               __raw_writeb(v, p)
#define outw(v, p)               __raw_writew(cpu_to_le16(v), p)
#define outl(v, p)               __raw_writel(cpu_to_le32(v), p)

#define inb(p)  ({ unsigned int __v = __raw_readb(p); __v; })
#define inw(p)  ({ unsigned int __v = __le16_to_cpu(__raw_readw(p)); __v; })
#define inl(p)  ({ unsigned int __v = __le32_to_cpu(__raw_readl(p)); __v; })

#define outsb(p, d, l)			__raw_writesb(p, d, l)
#define outsw(p, d, l)			__raw_writesw(p, d, l)
#define outsl(p, d, l)			__raw_writesl(p, d, l)

#define insb(p, d, l)			__raw_readsb(p, d, l)
#define insw(p, d, l)			__raw_readsw(p, d, l)
#define insl(p, d, l)			__raw_readsl(p, d, l)

#define outb_p(val, port)		outb((val), (port))
#define outw_p(val, port)		outw((val), (port))
#define outl_p(val, port)		outl((val), (port))
#define inb_p(port)			inb((port))
#define inw_p(port)			inw((port))
#define inl_p(port)			inl((port))

#define outsb_p(port, from, len)		outsb(port, from, len)
#define outsw_p(port, from, len)		outsw(port, from, len)
#define outsl_p(port, from, len)		outsl(port, from, len)
#define insb_p(port, to, len)		insb(port, to, len)
#define insw_p(port, to, len)		insw(port, to, len)
#define insl_p(port, to, len)		insl(port, to, len)

/* for U-Boot PCI */
#define out_8(port, val)	outb(val, port)
#define out_le16(port, val)	outw(val, port)
#define out_le32(port, val)	outl(val, port)
#define in_8(port)			inb(port)
#define in_le16(port)		inw(port)
#define in_le32(port)		inl(port)
/*
 * ioremap and friends.
 *
 * ioremap takes a PCI memory address, as specified in
 * linux/Documentation/IO-mapping.txt.  If you want a
 * physical address, use __ioremap instead.
 */
extern void *__ioremap(unsigned long offset, size_t size, unsigned long flags);
extern void __iounmap(void *addr);

/*
 * Generic ioremap support.
 *
 * Define:
 *  iomem_valid_addr(off,size)
 *  iomem_to_phys(off)
 */
#ifdef iomem_valid_addr
#define __arch_ioremap(off, sz, nocache)				\
 ({								\
	unsigned long _off = (off), _size = (sz);		\
	void *_ret = (void *)0;					\
	if (iomem_valid_addr(_off, _size))			\
		_ret = __ioremap(iomem_to_phys(_off), _size, 0);	\
	_ret;							\
 })

#define __arch_iounmap __iounmap
#endif

#define ioremap(off, sz)			__arch_ioremap((off), (sz), 0)
#define ioremap_nocache(off, sz)		__arch_ioremap((off), (sz), 1)
#define iounmap(_addr)			__arch_iounmap(_addr)

/*
 * DMA-consistent mapping functions.  These allocate/free a region of
 * uncached, unwrite-buffered mapped memory space for use with DMA
 * devices.  This is the "generic" version.  The PCI specific version
 * is in pci.h
 */
extern void *consistent_alloc(int gfp, size_t size, dma_addr_t *handle);
extern void consistent_free(void *vaddr, size_t size, dma_addr_t handle);
extern void consistent_sync(void *vaddr, size_t size, int rw);

/*
 * String version of IO memory access ops:
 */
extern void _memcpy_fromio(void *, unsigned long, size_t);
extern void _memcpy_toio(unsigned long, const void *, size_t);
extern void _memset_io(unsigned long, int, size_t);

static inline void sync(void)
{
}

/*
 * Given a physical address and a length, return a virtual address
 * that can be used to access the memory range with the caching
 * properties specified by "flags".
 */
#define MAP_NOCACHE     (0)
#define MAP_WRCOMBINE   (0)
#define MAP_WRBACK      (0)
#define MAP_WRTHROUGH   (0)

static inline void *
map_physmem(phys_addr_t paddr, unsigned long len, unsigned long flags)
{
	return (void *)paddr;
}

/*
 * Take down a mapping set up by map_physmem().
 */
static inline void unmap_physmem(void *vaddr, unsigned long flags)
{

}

static inline phys_addr_t virt_to_phys(void * vaddr)
{
	return (phys_addr_t)(vaddr);
}

#endif	/* __KERNEL__ */
#endif	/* __ASM_H8300_IO_H */

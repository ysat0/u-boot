#ifndef _ASM_CPU_SH2A_H_
#define _ASM_CPU_SH2A_H_

/* module enable */
#define STBCR4		0xFFFE040C
#define scif_enable(ch)					 \
do { 							 \
	if (ch < 4) { 					 \
		unsigned char mask = 1 << (7 - ch); 			 \
		writeb((readb(STBCR4) & ~mask), STBCR4); \
	} 						 \
} while (0)

#define cmt_clock_enable() \
do { 							 \
		writeb((readb(STBCR4) & ~0x04), STBCR4); \
} while (0)

#endif

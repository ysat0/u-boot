#ifndef _ASM_CPU_SH7206_H_
#define _ASM_CPU_SH7206_H_

/* Cache */
#define CCR1		0xFFFC1000
#define CCR2		0xFFFC1004
#define CCR		CCR1

#define CMNCR           0xFFFC0000
#define CS0BCR          0xFFFC0004
#define CS2BCR          0xFFFC000C
#define CS3BCR          0xFFFC0010
#define CS4BCR          0xFFFC0014
#define CS5BCR          0xFFFC0018
#define CS0WCR          0xFFFC0028
#define CS2WCR          0xFFFC0030
#define CS3WCR          0xFFFC0034
#define CS4WCR          0xFFFC0038
#define CS5WCR          0xFFFC003C
#define SDCR            0xFFFC004C
#define RTCSR           0xFFFC0050
#define RTCNT           0xFFFC0054
#define RTCOR           0xFFFC0058

/* SCIF */
#define SCSMR_0		0xFFFE8000
#define SCSMR_1		0xFFFE8800
#define SCSMR_2		0xFFFE9000
#define SCSMR_3		0xFFFE9800
#define SCIF0_BASE	SCSMR_0
#define SCIF1_BASE	SCSMR_1
#define SCIF2_BASE	SCSMR_2
#define SCIF3_BASE	SCSMR_3

/* Timer(CMT) */
#define CMSTR 		0xFFFEC000
#define CMCSR_0 	0xFFFEC002
#define CMCNT_0 	0xFFFEC004
#define CMCOR_0 	0xFFFEC006
#define CMCSR_1 	0xFFFEC008
#define CMCNT_1 	0xFFFEC00A
#define CMCOR_1		0xFFFEC00C

/* PFC */
#define	PACRH3		0xFFFE380A
#define PACRH2		0xFFFE380C
#define PACRH1		0xFFFE380E
#define PACRL4		0xFFFE3810
#define PACRL3		0xFFFE3812
#define PACRL2		0xFFFE3814
#define PACRL1		0xFFFE3816
#define PBCR3		0xFFFE3892
#define PBCR2		0xFFFE3894
#define PBCR1		0xFFFE3896
#define PCCRL1		0xFFFE3916
#define PDCRH4		0xFFFE3988
#define PDCRH3		0xFFFE398A
#define PDCRH2		0xFFFE398C
#define PDCRH1		0xFFFE398E
#define PDCRL4		0xFFFE3990
#define PDCRL3		0xFFFE3992
#define PECRH1		0xFFFE3A0E
#define PECRL4		0xFFFE3A10
#define PECRL3		0xFFFE3A12
#define PECRL2		0xFFFE3A14
#define IFCR		0xFFFE3A22

/* On chip oscillator circuits */
#define FRQCR		0xFFFE0010
#define MCLKCR		0xFFFE0410
/* Watchdog timer */
#define WTCNT		0xFFFE0002
#define WTCSR		0xFFFE0000

#endif	/* _ASM_CPU_SH7206_H_ */

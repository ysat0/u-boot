#ifndef _ASM_CPU_SH7619_H_
#define _ASM_CPU_SH7619_H_

/* Cache */
#define CCR1		0xFFFFFFEC	     
#define CCR		CCR1

#define CMNCR           0xFFFC0000
#define CS0BCR          0xF8FD0004
#define CS3BCR          0xF8FD000C
#define CS4BCR          0xF8FD0010
#define CS5BBCR         0xF8FD0018
#define CS6BBCR         0xF8FD001C
#define CS0WCR          0xF8FD0024
#define CS3WCR          0xF8FD002C
#define CS4WCR          0xF8FD0030
#define CS5BWCR         0xF8FD0038
#define CS6BWCR         0xF8FD0040
#define SDCR            0xF8FD0044
#define RTCSR           0xF8FD0048
#define RTCNT           0xF8FD004C
#define RTCOR           0xF8FD0050

/* SCIF */
#define SCSMR_0		0xF8400000
#define SCSMR_1		0xF8410000
#define SCSMR_2		0xF8420000
#define SCIF0_BASE	SCSMR_0
#define SCIF1_BASE	SCSMR_1
#define SCIF2_BASE	SCSMR_2

/* Timer(CMT) */
#define CMSTR           0xF84A0070
#define CMCSR_0         0xF84A0072
#define CMCNT_0         0xF84A0074
#define CMCOR_0         0xF84A0076
#define CMCSR_1         0xF84A0078
#define CMCNT_1         0xF84A007A
#define CMCOR_1         0xF84A007C

/* PFC */
#define PAIORH          0xF8050004
#define PACRH1          0xF8050008
#define PACRH2          0xF805000A
#define PBIORL          0xF8050016
#define PBCRL1          0xF805001C
#define PBCRL2          0xF805001E
#define PCIORH          0xF8050024
#define PCIORL          0xF8050026
#define PCCRH2          0xF805002A
#define PCCRL1          0xF805002C
#define PCCRL2          0xF805002E
#define PDIORL          0xF8050036
#define PDCRL2          0xF805003E
#define PEIORH          0xF8050044
#define PEIORL          0xF8050046
#define PECRH1          0xF8050048
#define PECRH2          0xF805004A
#define PECRL1          0xF805004C
#define PECRL2          0xF805004E
#define PADRH           0xF8050000
#define PBDRL           0xF8050012
#define PCDRH           0xF8050020
#define PCDRL           0xF8050022
#define PDDRL           0xF8050032
#define PEDRH           0xF8050040
#define PEDRL           0xF8050042

/* On chip oscillator circuits */
#define FRQCR		0xF815FF80

/* Watchdog timer */
#define WTCNT		0xF815FF84
#define WTCSR		0xF915FF86

#endif	/* _ASM_CPU_SH7619_H_ */

/*****************************************************************************/
/* FILE NAME: project.c               COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: Macro declaration for pin pads, LPU and PCTL                 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/* 1.0	  		 	             Jul 2013   Initial working version          */
/* 1.1	  K Shah			  23 Feb 2016   Code porting to S32DS			 */
/*****************************************************************************/

/************************************ File Includes *************************************/

#include "MPC5748G.h"  /* Cut 2 MPC5748G header */

#ifndef PROJECT_H_
#define PROJECT_H_

/********************************* Function Prototypes **********************************/


/************************************** Constants ***************************************/
#include "typedefs.h"

#define SYSTEM_CLOCK 	 80000000		// 80MHz set flash wait states to 3ws
#define DMA_TRANSFER_SIZE 10
/* GPIO PCRs */
#define  PA0  0
#define  PA1  1
#define  PA2  2
#define  PA3  3
#define  PA4  4
#define  PA5  5
#define  PA6  6
#define  PA7  7
#define  PA8  8
#define  PA9  9
#define  PA10  10
#define  PA11  11
#define  PA12  12
#define  PA13  13
#define  PA14  14
#define  PA15  15
#define  PB0  16
#define  PB1  17
#define  PB2  18
#define  PB3  19
#define  PB4  20
#define  PB5  21
#define  PB6  22
#define  PB7  23
#define  PB8  24
#define  PB9  25
#define  PB10  26
#define  PB11  27
#define  PB12  28
#define  PB13  29
#define  PB14  30
#define  PB15  31
#define  PC0  32
#define  PC1  33
#define  PC2  34
#define  PC3  35
#define  PC4  36
#define  PC5  37
#define  PC6  38
#define  PC7  39
#define  PC8  40
#define  PC9  41
#define  PC10  42
#define  PC11  43
#define  PC12  44
#define  PC13  45
#define  PC14  46
#define  PC15  47
#define  PD0  48
#define  PD1  49
#define  PD2  50
#define  PD3  51
#define  PD4  52
#define  PD5  53
#define  PD6  54
#define  PD7  55
#define  PD8  56
#define  PD9  57
#define  PD10  58
#define  PD11  59
#define  PD12  60
#define  PD13  61
#define  PD14  62
#define  PD15  63
#define  PE0  64
#define  PE1  65
#define  PE2  66
#define  PE3  67
#define  PE4  68
#define  PE5  69
#define  PE6  70
#define  PE7  71
#define  PE8  72
#define  PE9  73
#define  PE10  74
#define  PE11  75
#define  PE12  76
#define  PE13  77
#define  PE14  78
#define  PE15  79
#define  PF0  80
#define  PF1  81
#define  PF2  82
#define  PF3  83
#define  PF4  84
#define  PF5  85
#define  PF6  86
#define  PF7  87
#define  PF8  88
#define  PF9  89
#define  PF10  90
#define  PF11  91
#define  PF12  92
#define  PF13  93
#define  PF14  94
#define  PF15  95
#define  PG0  96
#define  PG1  97
#define  PG2  98
#define  PG3  99
#define  PG4  100
#define  PG5  101
#define  PG6  102
#define  PG7  103
#define  PG8  104
#define  PG9  105
#define  PG10  106
#define  PG11  107
#define  PG12  108
#define  PG13  109
#define  PG14  110
#define  PG15  111
#define  PH0  112
#define  PH1  113
#define  PH2  114
#define  PH3  115
#define  PH4  116
#define  PH5  117
#define  PH6  118
#define  PH7  119
#define  PH8  120
#define  PH9  121
#define  PH10  122
#define  PH11  123
#define  PH12  124
#define  PH13  125
#define  PH14  126
#define  PH15  127
#define  PI0  128
#define  PI1  129
#define  PI2  130
#define  PI3  131
#define  PI4  132
#define  PI5  133
#define  PI6  134
#define  PI7  135
#define  PI8  136
#define  PI9  137
#define  PI10  138
#define  PI11  139
#define  PI12  140
#define  PI13  141
#define  PI14  142
#define  PI15  143
#define  PJ0  144
#define  PJ1  145
#define  PJ2  146
#define  PJ3  147
#define  PJ4  148
#define  PJ5  149
#define  PJ6  150
#define  PJ7  151
#define  PJ8  152
#define  PJ9  153
#define  PJ10  154
#define  PJ11  155
#define  PJ12  156
#define  PJ13  157
#define  PJ14  158
#define  PJ15  159
#define  PK0  160
#define  PK1  161
#define  PK2  162
#define  PK3  163
#define  PK4  164
#define  PK5  165
#define  PK6  166
#define  PK7  167
#define  PK8  168
#define  PK9  169
#define  PK10  170
#define  PK11  171
#define  PK12  172
#define  PK13  173
#define  PK14  174
#define  PK15  175
#define  PL0  176
#define  PL1  177
#define  PL2  178
#define  PL3  179
#define  PL4  180
#define  PL5  181
#define  PL6  182
#define  PL7  183
#define  PL8  184
#define  PL9  185
#define  PL10  186
#define  PL11  187
#define  PL12  188
#define  PL13  189
#define  PL14  190
#define  PL15  191
#define  PM0  192
#define  PM1  193
#define  PM2  194
#define  PM3  195
#define  PM4  196
#define  PM5  197
#define  PM6  198
#define  PM7  199
#define  PM8  200
#define  PM9  201
#define  PM10  202
#define  PM11  203
#define  PM12  204
#define  PM13  205
#define  PM14  206
#define  PM15  207
#define  PN0  208
#define  PN1  209
#define  PN2  210
#define  PN3  211
#define  PN4  212
#define  PN5  213
#define  PN6  214
#define  PN7  215
#define  PN8  216
#define  PN9  217
#define  PN10  218
#define  PN11  219
#define  PN12  220
#define  PN13  221
#define  PN14  222
#define  PN15  223
#define  PO0  224
#define  PO1  225
#define  PO2  226
#define  PO3  227
#define  PO4  228
#define  PO5  229
#define  PO6  230
#define  PO7  231
#define  PO8  232
#define  PO9  233
#define  PO10  234
#define  PO11  235
#define  PO12  236
#define  PO13  237
#define  PO14  238
#define  PO15  239
#define  PP0  240
#define  PP1  241
#define  PP2  242
#define  PP3  243
#define  PP4  244
#define  PP5  245
#define  PP6  246
#define  PP7  247
#define  PP8  248
#define  PP9  249
#define  PP10  250
#define  PP11  251
#define  PP12  252
#define  PP13  253
#define  PP14  254
#define  PP15  255
#define  PQ0  256
#define  PQ1  257
#define  PQ2  258
#define  PQ3  259
#define  PQ4  260
#define  PQ5  261
#define  PQ6  262
#define  PQ7  263

/* GPIO available in LPU_RUN and Pad Keeper in STANDBY */
#define LPU_A0	0
#define LPU_A1	1
#define LPU_A2	2
#define LPU_A4	4
#define LPU_B12	12
#define LPU_B13	13
#define LPU_B14	14
#define LPU_B15	15
#define LPU_B0	16
#define LPU_B1	17
#define LPU_B3	19
#define LPU_C10	26
#define LPU_C11	27
#define LPU_C12	28
#define LPU_C0	32
#define LPU_C1	33
#define LPU_C6	38
#define LPU_C7	39
#define LPU_D9	41
#define LPU_D10	42
#define LPU_D11	43
#define LPU_E12	60
#define LPU_E13	61
#define LPU_E0	64
#define LPU_E3	67
#define LPU_E5	69
#define LPU_F9	73
#define LPU_F11	75
#define LPU_F0	80
#define LPU_F1	81
#define LPU_F2	82
#define LPU_F3	83
#define LPU_F4	84
#define LPU_F5	85
#define LPU_F6	86
#define LPU_F7	87
#define LPU_G8	88
#define LPU_G9	89
#define LPU_G10	90
#define LPU_G11	91
#define LPU_G12	92
#define LPU_G13	93
#define LPU_G3	99
#define LPU_G5	101
#define LPU_G6	102
#define LPU_G7	103
#define LPU_H8	104
#define LPU_H9	105
#define LPU_I9	121
#define LPU_I10	122
#define LPU_I1	129
#define LPU_I3	131
#define LPU_J9	137
#define LPU_J10	138
#define LPU_J14	142
#define LPU_J15	143
#define LPU_J0	144
#define LPU_J1	145
#define LPU_J2	146
#define LPU_J3	147
#define LPU_J5	149
#define LPU_J6	150
#define LPU_J7	151
#define LPU_K8	152
#define LPU_K9	153
#define LPU_K10	154
#define LPU_K11	155
#define LPU_K12	156
#define LPU_K13	157
#define LPU_K14	158
#define LPU_K15	159
#define LPU_K0	160
#define LPU_K1	161
#define LPU_K2	162
#define LPU_K3	163
#define LPU_K4	164
#define LPU_K5	165
#define LPU_K6	166
#define LPU_K7	167
#define LPU_L8	168
#define LPU_M15	207
#define LPU_M0	208
#define LPU_M1	209
#define LPU_M2	210
#define LPU_M3	211
#define LPU_M4	212
#define LPU_M5	213
#define LPU_M6	214
#define LPU_M7	215
#define LPU_N8	216
#define LPU_N9	217
#define LPU_N10	218
#define LPU_N11	219
#define LPU_N12	220
#define LPU_N13	221

#define PCTL_BCTU	0
#define PCTL_EMIOS_0	1
#define PCTL_EMIOS_1	2
#define PCTL_EMIOS_2	3
#define PCTL_USB	4
#define PCTL_USB_SPH	5
#define PCTL_MLB	6
#define PCTL_SDHC	7
#define PCTL_SAI0	10
#define PCTL_SAI1	11
#define PCTL_SAI2	12
#define PCTL_ENET	15
#define PCTL_CMP_0	20
#define PCTL_CMP_1	21
#define PCTL_CMP_2	22
#define PCTL_ADC_0	24
#define PCTL_ADC_1	25
#define PCTL_FLEXRAY	28
#define PCTL_IIC_0	30
#define PCTL_IIC_1	31
#define PCTL_IIC_2	32
#define PCTL_IIC_3	33
#define PCTL_DSPI_0	40
#define PCTL_DSPI_1	41
#define PCTL_DSPI_2	42
#define PCTL_DSPI_3	43
#define PCTL_LIN_0	50
#define PCTL_LIN_1	51
#define PCTL_LIN_2	52
#define PCTL_LIN_3	53
#define PCTL_LIN_4	54
#define PCTL_LIN_5	55
#define PCTL_LIN_6	56
#define PCTL_LIN_7	57
#define PCTL_LIN_8	58
#define PCTL_LIN_9	59
#define PCTL_LIN_10	60
#define PCTL_LIN_11	61
#define PCTL_LIN_12	62
#define PCTL_LIN_13	63
#define PCTL_LIN_14	64
#define PCTL_LIN_15	65
#define PCTL_LIN_16	66
#define PCTL_LIN_17	67
#define PCTL_FLEXCAN_0	70
#define PCTL_FLEXCAN_1	71
#define PCTL_FLEXCAN_2	72
#define PCTL_FLEXCAN_3	73
#define PCTL_FLEXCAN_4	74
#define PCTL_FLEXCAN_5	75
#define PCTL_FLEXCAN_6	76
#define PCTL_FLEXCAN_7	77
#define PCTL_HSM	80
#define PCTL_TDM	81
#define PCTL_JDC	82
#define PCTL_MEMU_0	83
#define PCTL_CRC	84
#define PCTL_DMAMUX	90
#define PCTL_PIT_RTI_0	91
#define PCTL_WKPU	93
#define PCTL_SIU 	94
#define PCTL_SPI0	96
#define PCTL_SPI1	97
#define PCTL_SPI2	98
#define PCTL_SPI3	99
#define PCTL_SPI4	100
#define PCTL_SPI5	101
#define PCTL_RTC_API	102
#define PCTL_PASS	104

/* ???LSB??????????????????????????? ??????count=2????????????0x00000003U*/
#define MASK_BITS_FROM_LSB(count) (~((0xffffffffU >> (count)) << (count)))

#define COUNT_OF(x) (sizeof(x)/sizeof(x[0]))
#define _BIT_(x) (1U<<(x))

// #define DEBUG_WDG_OFF  // ????????????
//  #define DEBUG_REALTIME // ???????????????
#if defined(DEBUG_WDG_OFF)
#define NONE_BOOTLD_MODE // ???BootLoader??????
#endif
#if defined(DEBUG_REALTIME)
// #define DEBUG_UDP_REALTIME
#endif


#define USE_GPRMC_FROM_UART  // ???????????????GPRMC


#else

#endif /* PROJECT_H_ */

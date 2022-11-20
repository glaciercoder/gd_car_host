/*
 * can.c
 *
 *  Created on: 2019骞�1鏈�4鏃�
 *      Author: Hjian
 */
#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "can.h"



#define BIT(X)  (0x1<<X)

// #define CAN0_MB_CNT 32
#define CAN1_MB_CNT (32+8)
#define CAN2_MB_CNT 32
#define CAN3_MB_CNT 32
#define CAN4_MB_CNT 16
#define CAN5_MB_CNT 16
#define CAN6_MB_CNT 16
#define CAN7_MB_CNT 16

#define CAN0_BASE_ADDRESS ((unsigned int)&CAN_0)
#define CAN1_BASE_ADDRESS ((unsigned int)&CAN_1)

static int flexCanGpioInit(can_com_t instance);
static int flexCanRXBufferInit(can_com_t instance);
#define can3_num  2
#define can1_num  2


CanTSET_buff can_testbuff;

//uint32_t can3_id[8]={0x210,0x7E0,0x500,0x6A0,0x6B0,0x6C0,0x6D0,0x5F0};
uint32_t can3_id[can3_num] = {0x5c8,0x5c9};
uint32_t can1_id[can1_num] = {0x223,0x224};
/**********************************************************************
 description : FlexCan鍒濆鍖栭厤缃嚱鏁�
 @argument instance			: FlexCan 0,1,2,3....
 @argument can_baudrate_t 	: 娉㈢壒鐜�
 @argument priority			: 鎺ュ彈涓柇浼樺厛绾э紝0-15 (15涓烘渶楂樹紭鍏堢骇)
 @return : none
 **********************************************************************/
int flexCanInitConfig(can_com_t instance,can_baudrate_t baudrate,unsigned char priority)
{
	int i = 0;
	 switch(instance)
	{
	 case CAN_COM_0:
		 break;
	 case CAN_COM_1:
		flexCanGpioInit(CAN_COM_1);
		/* 鍒濆鍖朚B锛屽叧闂� */
		for (i = 0; i < CAN1_MB_CNT; i ++)
		{
			CAN_1.MB[i].CS.B.CODE = 0;
		}

		/* 鍓�32涓狹B鐢ㄤ簬鎺ユ敹 */
		CAN_1.IMASK1.R			= 0;

		CAN_1.MCR.B.MDIS 		= 0;
		while(CAN_1.MCR.B.SOFTRST == 1){};
		CAN_1.MCR.B.RFEN 		= 0;
		CAN_1.MCR.B.IRMQ 		= 0;
		CAN_1.MCR.B.SRXDIS 		= 1;
		CAN_1.MCR.B.MAXMB 		= CAN1_MB_CNT - 1;
		CAN_1.CTRL1.B.CLKSRC 	= 0;

		CAN_1.CTRL1.B.PRESDIV 	= 40*1000/(baudrate * 16) - 1;
		CAN_1.CTRL1.B.RJW 		= 1;
		CAN_1.CTRL1.B.PSEG1 	= 3;
		CAN_1.CTRL1.B.PSEG2 	= 3;
		CAN_1.CTRL1.B.PROPSEG 	= 6;

		CAN_1.RXMGMASK.R = 0x1FFFFFFF;
		CAN_1.MCR.B.HALT 		= 0;

		flexCanRXBufferInit(CAN_COM_1);
		CAN_1.MCR.B.FRZ 		= 0;

		 break;
	 case CAN_COM_2:
		flexCanGpioInit(CAN_COM_2);
		/* 鍒濆鍖朚B锛屽叧闂� */
		for (i = 0; i < CAN2_MB_CNT; i ++)
		{
			CAN_2.MB[i].CS.B.CODE = 0;
		}
		CAN_2.IMASK1.R 			= 0;

		CAN_2.MCR.B.MDIS 		= 0;
		while(CAN_2.MCR.B.SOFTRST == 1){};
		CAN_2.MCR.B.RFEN 		= 0;
		CAN_2.MCR.B.IRMQ 		= 0;
		CAN_2.MCR.B.SRXDIS 		= 1;
		CAN_2.MCR.B.MAXMB 		= CAN2_MB_CNT - 1;
		CAN_2.CTRL1.B.CLKSRC 	= 0;
		CAN_2.CTRL1.B.PRESDIV 	= 40*1000/(baudrate * 16) - 1;
		CAN_2.CTRL1.B.RJW 		= 1;
		CAN_2.CTRL1.B.PSEG1 	= 3;
		CAN_2.CTRL1.B.PSEG2 	= 3;
		CAN_2.CTRL1.B.PROPSEG 	= 6;

		CAN_2.RXMGMASK.R = 0x1FFFFFFF;
		CAN_2.MCR.B.HALT 		= 0;

		flexCanRXBufferInit(CAN_COM_2);
		CAN_2.MCR.B.FRZ 		= 0;

		 break;
	 case CAN_COM_3:
		flexCanGpioInit(CAN_COM_3);
		/* 鍒濆鍖朚B锛屽叧闂� */
		for (i = 0; i < CAN3_MB_CNT; i ++)
		{
			CAN_3.MB[i].CS.B.CODE = 0;
		}
		CAN_3.IMASK1.R			   = 0;

		 CAN_3.MCR.B.MDIS 		= 0;
		 while(CAN_3.MCR.B.SOFTRST == 1){};
		 CAN_3.MCR.B.RFEN 		= 0;
		 CAN_3.MCR.B.IRMQ 		= 0;
		 CAN_3.MCR.B.SRXDIS 		= 1;
		 CAN_3.MCR.B.MAXMB 		= CAN3_MB_CNT - 1;
		 CAN_3.CTRL1.B.CLKSRC 	= 0;
		 CAN_3.CTRL1.B.PRESDIV 	= 40*1000/(baudrate * 16) - 1;
		 CAN_3.CTRL1.B.RJW 		= 1;
		 CAN_3.CTRL1.B.PSEG1 	= 3;
		 CAN_3.CTRL1.B.PSEG2 	= 3;
		 CAN_3.CTRL1.B.PROPSEG 	= 6;

		 CAN_3.RXMGMASK.R = 0x1FFFFFFF;
		 CAN_3.MCR.B.HALT 		= 0;

		 flexCanRXBufferInit(CAN_COM_3);
		 CAN_3.MCR.B.FRZ 		= 0;

		 break;
	 case CAN_COM_4:
		
		 break;
	 case CAN_COM_5:
		
		 break;
	 case CAN_COM_6:
		
		 break;
	 case CAN_COM_7:
		
		 break;
	 default:
		 break;
	}

	return 0;
}


/**********************************************************************
 description : FlexCan GPIO鍒濆鍖栧嚱鏁�
 @argument instance	: FlexCan 0,1,2,3....
 @return : none
 **********************************************************************/
static int flexCanGpioInit(can_com_t instance)
{
	switch(instance)
	{
	case CAN_COM_0:
		SIUL2.MSCR[16].B.SSS = 1;    /* Pad PB0: Source signal is CAN0_TX */
		SIUL2.MSCR[16].B.OBE = 1;    /* Pad PB0: Output Buffer Enable */
		SIUL2.MSCR[16].B.SRC = 3;    /* Pad PB0: Maximum slew rate */

		SIUL2.MSCR[17].B.IBE = 1;    /* Pad PB1: Enable pad for input - CAN0_RXD */
		SIUL2.IMCR[188].B.SSS = 2;     /* CAN_0_RXD: connected to pad PB1 */
		break;
	case CAN_COM_1:
		SIUL2.MSCR[42].B.SSS = 1;
		SIUL2.MSCR[42].B.OBE = 1;
		SIUL2.MSCR[42].B.SRC = 3;

		SIUL2.MSCR[43].B.IBE = 1;
		SIUL2.IMCR[189].B.SSS = 3;
		break;
	case CAN_COM_2:
		SIUL2.MSCR[PE8].B.SSS = 1;
		SIUL2.MSCR[PE8].B.OBE = 1;
		SIUL2.MSCR[PE8].B.SRC = 3;

		SIUL2.MSCR[PE9].B.IBE = 1;
		SIUL2.IMCR[190].B.SSS = 1;
		break;
	case CAN_COM_3:
		SIUL2.MSCR[PF8].B.SSS = 1;
		SIUL2.MSCR[PF8].B.OBE = 1;
		SIUL2.MSCR[PF8].B.SRC = 3;

		SIUL2.MSCR[PF9].B.IBE = 1;
		SIUL2.IMCR[191].B.SSS = 4;
		break;
	case CAN_COM_4:
		SIUL2.MSCR[PC2].B.SSS = 2;
		SIUL2.MSCR[PC2].B.OBE = 1;
		SIUL2.MSCR[PC2].B.SRC = 3;

		SIUL2.MSCR[PC3].B.IBE = 1;
		SIUL2.IMCR[192].B.SSS = 1;
		break;
	case CAN_COM_5:
		SIUL2.MSCR[PE1].B.SSS = 2;
		SIUL2.MSCR[PE1].B.OBE = 1;
		SIUL2.MSCR[PE1].B.SRC = 3;

		SIUL2.MSCR[PE0].B.IBE = 1;
		SIUL2.IMCR[193].B.SSS = 1;
		break;
	case CAN_COM_6:
		SIUL2.MSCR[PJ14].B.SSS = 4;
		SIUL2.MSCR[PJ14].B.OBE = 1;
		SIUL2.MSCR[PJ14].B.SRC = 3;

		SIUL2.MSCR[PJ13].B.IBE = 1;
		SIUL2.IMCR[194].B.SSS = 2;
		break;
	case CAN_COM_7:
		SIUL2.MSCR[PG8].B.SSS = 4;
		SIUL2.MSCR[PG8].B.OBE = 1;
		SIUL2.MSCR[PG8].B.SRC = 3;

		SIUL2.MSCR[PG9].B.IBE = 1;
		SIUL2.IMCR[195].B.SSS = 2;
		break;
	default:break;
	}
	return 0;
}


/**********************************************************************
 description : FlexCan RX 鍒濆鍖栧嚱鏁�
 @argument instance	: FlexCan 0,1,2,3....
 @return : none
 **********************************************************************/
static int flexCanRXBufferInit(can_com_t instance)
{
    uint8_t i;
	switch(instance)
	{
	case CAN_COM_0:
		break;
	case CAN_COM_1:
		/* 鍓�32涓狹B鐢ㄤ簬鎺ユ敹 */
		for(i=0;i<COUNT_OF(can1_id);i++){
			CAN_1.MB[i].CS.B.CODE = 0x0;
			CAN_1.MB[i].CS.B.IDE = 0;
			CAN_1.MB[i].DATA.W[1] = 0x00000000;
			CAN_1.MB[i].DATA.W[0] = 0x00000000;
			CAN_1.MB[i].CS.B.CODE = 0x4;
			CAN_1.IMASK1.B.BUF31TO0M |= (1 << i);
		}

		for(i = 0;i < COUNT_OF(can1_id);i++)
		{
			CAN_1.MB[i].ID.B.ID_STD = can1_id[i];
		}


		    INTC.PSR[580].B.PRC_SELN0 = 1;
		    INTC.PSR[580].B.PRIN = 10;
		    INTC.PSR[581].B.PRC_SELN0 = 1;
		    INTC.PSR[581].B.PRIN = 10;
		    INTC.PSR[582].B.PRC_SELN0 = 1;
		    INTC.PSR[582].B.PRIN = 10;
		    INTC.PSR[583].B.PRC_SELN0 = 1;
		    INTC.PSR[583].B.PRIN = 10;
		    INTC.PSR[584].B.PRC_SELN0 = 1;
		    INTC.PSR[584].B.PRIN = 10;

            CAN_1.IFLAG1.R = MASK_BITS_FROM_LSB(COUNT_OF(can1_id)); /* Clear CAN_1 MB  flag */
			break;
	case CAN_COM_2:
			break;
	case CAN_COM_3:
		for(i=0; i < COUNT_OF(can3_id); i++)
		{
		  CAN_3.MB[i].CS.B.CODE = 0x0;
		  CAN_3.MB[i].CS.B.IDE = 0;
		  CAN_3.MB[i].DATA.W[1] = 0x00000000;
		  CAN_3.MB[i].DATA.W[0] = 0x00000000;
		  CAN_3.MB[i].CS.B.CODE = 0x4;
		  CAN_3.IMASK1.B.BUF31TO0M |= (1 << i);
		}

		for(i = 0; i < COUNT_OF(can3_id); i++)
		{
			CAN_3.MB[i].ID.B.ID_STD = can3_id[i];
		}

			INTC.PSR[604].B.PRC_SELN0 = 1;
			INTC.PSR[604].B.PRIN = 10;
			
			CAN_3.IFLAG1.R = MASK_BITS_FROM_LSB(COUNT_OF(can3_id));

			break;
	case CAN_COM_4:
			
			break;
	case CAN_COM_5:
			
			break;
	case CAN_COM_6:
			
			break;
	case CAN_COM_7:
			
			break;
	default:
		break;
	}
	return 0;
}

/**********************************************************************
 description : FlexCan 鎶ユ枃鍙戦�� 鍑芥暟锛涘彂閫侀偖绠变负缁熶竴涓篗B15
 @argument instance	: FlexCan 0,1,2,3....
 @argument msg		: 灏嗚鍙戦�佺殑CAN鎶ユ枃锛屾敞鎰忛渶瑕佸厛鍦╩sg閲岄潰璁剧疆濂介�氶亾鍜屾姤鏂囩被鍨嬶紙鎵╁睍銆佹爣鍑嗭級
 @return : none
 **********************************************************************/
int flexCanMsgTransmit(can_frame_t* msg)
{
	int i = 0;
	unsigned char channel = msg->send_com;
	static unsigned char send_cnt = 0;
	unsigned j;
	
	switch(channel)
	{
	case CAN_COM_0:
		CAN_0.MB[15].CS.B.CODE = 0x8;

		if(msg->type == CAN_STD)
		{
			CAN_0.MB[15].CS.B.IDE = 0;			  //send  STD ID
			CAN_0.MB[15].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_0.MB[15].CS.B.IDE = 1;			//set EXT ID
			CAN_0.MB[15].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_0.MB[15].DATA.B[i] = msg->data[i];
		}
		CAN_0.MB[15].CS.B.DLC = 8;
		CAN_0.MB[15].CS.B.RTR = 0;
		CAN_0.MB[15].CS.B.CODE = 0xC;
//		while(!(CAN_0.IFLAG1.R & BIT(15))) {};
//		CAN_0.IFLAG1.R |= BIT(15);
		break;
	case CAN_COM_1:
		/* MB[32]~MB[37]鐢ㄤ簬鍙戦�侊紝鍏�8涓� */
		j = (send_cnt % 8) + 32;
		send_cnt++;
		CAN_1.MB[j].CS.B.CODE = 0x8;
		if(msg->type == CAN_STD)
		{
			CAN_1.MB[j].CS.B.IDE = 0;			  //send  STD ID
			CAN_1.MB[j].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_1.MB[j].CS.B.IDE = 1;			//set EXT ID
			CAN_1.MB[j].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_1.MB[j].DATA.B[i] = msg->data[i];
		}

		CAN_1.MB[j].CS.B.DLC = 8;
		CAN_1.MB[j].CS.B.RTR = 0;
		CAN_1.MB[j].CS.B.CODE = 0xC;
//		while(!(CAN_1.IFLAG1.R & BIT(15))){};
//		CAN_1.IFLAG1.R |= BIT(15);
		break;
	case CAN_COM_2:
		CAN_2.MB[15].CS.B.CODE = 0x8;
		if(msg->type == CAN_STD)
		{
			CAN_2.MB[15].CS.B.IDE = 0;			  //send  STD ID
			CAN_2.MB[15].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_2.MB[15].CS.B.IDE = 1;			//set EXT ID
			CAN_2.MB[15].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_2.MB[15].DATA.B[i] = msg->data[i];
		}

		CAN_2.MB[15].CS.B.DLC = 8;
		CAN_2.MB[15].CS.B.RTR = 0;
		CAN_2.MB[15].CS.B.CODE = 0xC;
//		while(!(CAN_2.IFLAG1.R & BIT(15))){};
//		CAN_2.IFLAG1.R |= BIT(15);
		break;
	case CAN_COM_3:
		CAN_3.MB[15].CS.B.CODE = 0x8;
		if(msg->type == CAN_STD)
		{
			CAN_3.MB[15].CS.B.IDE = 0;			  //send  STD ID
			CAN_3.MB[15].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_3.MB[15].CS.B.IDE = 1;			//set EXT ID
			CAN_3.MB[15].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_3.MB[15].DATA.B[i] = msg->data[i];
		}

		CAN_3.MB[15].CS.B.DLC = 8;
		CAN_3.MB[15].CS.B.RTR = 0;
		CAN_3.MB[15].CS.B.CODE = 0xC;
//		while(!(CAN_3.IFLAG1.R & BIT(15))){};
//		CAN_3.IFLAG1.R |= BIT(15);
			break;
	case CAN_COM_4:
		CAN_4.MB[15].CS.B.CODE = 0x8;
		if(msg->type == CAN_STD)
		{
			CAN_4.MB[15].CS.B.IDE = 0;			  //send  STD ID
			CAN_4.MB[15].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_4.MB[15].CS.B.IDE = 1;			//set EXT ID
			CAN_4.MB[15].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_4.MB[15].DATA.B[i] = msg->data[i];
		}

		CAN_4.MB[15].CS.B.DLC = 8;
		CAN_4.MB[15].CS.B.RTR = 0;
		CAN_4.MB[15].CS.B.CODE = 0xC;
//		while(!(CAN_4.IFLAG1.R & BIT(15))){};
//		CAN_4.IFLAG1.R |= BIT(15);
			break;
	case CAN_COM_5:
		CAN_5.MB[15].CS.B.CODE = 0x8;
		if(msg->type == CAN_STD)
		{
			CAN_5.MB[15].CS.B.IDE = 0;			  //send  STD ID
			CAN_5.MB[15].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_5.MB[15].CS.B.IDE = 1;			//set EXT ID
			CAN_5.MB[15].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_5.MB[15].DATA.B[i] = msg->data[i];
		}

		CAN_5.MB[15].CS.B.DLC = 8;
		CAN_5.MB[15].CS.B.RTR = 0;
		CAN_5.MB[15].CS.B.CODE = 0xC;
//		while(!(CAN_5.IFLAG1.R & BIT(15))){};
//		CAN_5.IFLAG1.R |= BIT(15);
		break;
	case CAN_COM_6:
		CAN_6.MB[15].CS.B.CODE = 0x8;
		if(msg->type == CAN_STD)
		{
			CAN_6.MB[15].CS.B.IDE = 0;			  //send  STD ID
			CAN_6.MB[15].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_6.MB[15].CS.B.IDE = 1;			//set EXT ID
			CAN_6.MB[15].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_6.MB[15].DATA.B[i] = msg->data[i];
		}

		CAN_6.MB[15].CS.B.DLC = 8;
		CAN_6.MB[15].CS.B.RTR = 0;
		CAN_6.MB[15].CS.B.CODE = 0xC;
//		while(!(CAN_6.IFLAG1.R & BIT(15))){};
//		CAN_6.IFLAG1.R |= BIT(15);
			break;
	case CAN_COM_7:
		CAN_7.MB[15].CS.B.CODE = 0x8;
		if(msg->type == CAN_STD)
		{
			CAN_7.MB[15].CS.B.IDE = 0;			  //send  STD ID
			CAN_7.MB[15].ID.R = (msg->id)<<18;
		}
		else if(msg->type == CAN_EXT)
		{
			CAN_7.MB[15].CS.B.IDE = 1;			//set EXT ID
			CAN_7.MB[15].ID.R = msg->id;
		}
		else
		{
			return -1;
		}
		for(i = 0;i<8;i++)
		{
			CAN_7.MB[15].DATA.B[i] = msg->data[i];
		}

		CAN_7.MB[15].CS.B.DLC = 8;
		CAN_7.MB[15].CS.B.RTR = 0;
		CAN_7.MB[15].CS.B.CODE = 0xC;
//		while(!(CAN_7.IFLAG1.R & BIT(15))){};
//		CAN_7.IFLAG1.R |= BIT(15);
			break;
		default:
		break;
	}
	return 0;
}





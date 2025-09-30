// --------------------------------------------------------------------
// Copyright (c) 2010 by Terasic Technologies Inc. 
// --------------------------------------------------------------------
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------

#include "terasic_includes.h"
#include "sd_protocol.h"
#include "crc7.h"


 
#ifdef DEBUG_SDCARD
	#include "debug.h"
    #define SDCARD_TRACE(x)    {DEBUG(("[SD_PROTOCAL]")); DEBUG(x);}  
    #define SDCARD_DEBUG(x)    {DEBUG(("[SD_PROTOCAL]")); DEBUG(x);}  
#else
    #define SDCARD_TRACE(x)
    #define SDCARD_DEBUG(x)
#endif

//============= constant definition ===================================

// card status
#define SD_BIT_APP_CMD              (0x01 << 5)
#define SD_BIT_READY_FOR_DATA       (0x01 << 8)

#define SD_BITS_CURRENT_STATE       (0x0F << 9)

#define SD_CURRENT_STATE_DECODE(x)      ((x >> 9) & 0x0F)
#define SD_CURRENT_STATE_ENCODE(x)      ((x & 0x0F) << 9)



#define CM8_TEST_PATTERN    0xAA


// internal function
alt_u32 ArrayToU32(alt_u8 szData[]);

//============================================================


#define REG_CSR   		0 // control & status register
#define REG_CMD   		4 // command
#define REG_RSP_DATA  	30 // response & data read/write

// transaction type
#define TXN_NONE			0
#define TXN_DUMMY_CYCLE		1
#define TXN_CMD				2
#define TXN_CMD_RESPONE48	3
#define TXN_CMD_RESPONE136	4
#define TXN_BLOCK_READ		5
#define TXN_BLOCK_WRITE		6

// error code
#define ERR_SUCCESS    			0
#define ERR_TIMEOUT    			1
#define ERR_CRC    				2
#define ERR_WRITE    			3
#define ERR_INVALID_END_BIT  	4
#define ERR_OTHER    			5

void RTL_SetupCmd(alt_u8 *pData, int nLen){
	int i;

	SDCARD_TRACE(("RTL_SetupCmd\r\n"));
	for(i=0;i<nLen;i++)
		IOWR_8DIRECT(TERASIC_SDCARD_BASE+REG_CMD, i, *(pData +i));
	//memset(TERASIC_SDCARD_BASE+REG_CMD, pData, nLen)
}

void RTL_SetupWriteData(alt_u8 *pData){
	int i;

	SDCARD_TRACE(("RTL_SetupWriteData\r\n"));
	for(i=0;i<512;i++)
		IOWR_8DIRECT(TERASIC_SDCARD_BASE+REG_RSP_DATA, i, *(pData +i));
	//memset(TERASIC_SDCARD_BASE+REG_CMD, pData, nLen)
}

bool RTL_Execute(alt_u8  txn_type, alt_u8 *pResponse){
	bool bSuccess = false, bDone = false;
	alt_u8 status, error_code;
	int i, nLen;
	char szInfo[32];

	SDCARD_TRACE(("RTL_Execute: start (txn_type=%d)\r\n", txn_type));

	IOWR_8DIRECT(TERASIC_SDCARD_BASE+REG_CSR, 0x00, txn_type << 1); // set txn_type first
	IOWR_8DIRECT(TERASIC_SDCARD_BASE+REG_CSR, 0x00, 0x01 | (txn_type << 1)); // start
	IOWR_8DIRECT(TERASIC_SDCARD_BASE+REG_CSR, 0x00, 0x00);

	// wait done

	SDCARD_TRACE(("RTL_Execute: wait done\r\n"));
	while(!bDone){
		status = IORD_8DIRECT(TERASIC_SDCARD_BASE+REG_CSR, 0x00);
		error_code = (status >> 1 ) & 0x0F;
		if (status & 0x01){ // check bit0 for done
			bDone = true;
			if (error_code == 0x00){ // check bit1/2/3/4 for error
				bSuccess = true;
			}else{
				switch(error_code){
					case 1: strcpy(szInfo, "timeout"); break;
					case 2: strcpy(szInfo, "crc error"); break;
					case 3: strcpy(szInfo, "write error"); break;
					case 4: strcpy(szInfo, "invalid end bit"); break;
					default: strcpy(szInfo, "unknown");
				}
				SDCARD_TRACE(("........... done with error code %d (%s)\r\n", error_code, szInfo));
			}
		}

	}

	SDCARD_TRACE(("RTL_Execute: read cmd/data response\r\n"));

	// read response
	if ((txn_type == TXN_CMD_RESPONE48) || (txn_type == TXN_CMD_RESPONE136) || (txn_type == TXN_BLOCK_READ)){
		nLen = (txn_type == TXN_BLOCK_READ)?512:((txn_type == TXN_CMD_RESPONE48)?6:17);
		for(i=0;i<nLen;i++){\
			status = IORD_8DIRECT(TERASIC_SDCARD_BASE+REG_RSP_DATA, i);
			*(pResponse + i) = status;
		}
	}

	SDCARD_TRACE(("RTL_Execute: done %s\r\n",bSuccess?"bSuccess":"NG"));
    return bSuccess;


}








void SD_CMD0(void){
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD0\r\n"));
    // build packet    
    szCmd[0] |= 0X00;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;
    
    RTL_SetupCmd(szCmd, sizeof(szCmd));
    RTL_Execute(TXN_CMD, NULL);
    
    SDCARD_TRACE(("CMD0_END\r\n"));

}


bool SD_CMD8(alt_u8 VolId, alt_u8 TestPattern){
    bool bSuccess;
    alt_u8 szResponse[6];
    const alt_u8 Cmd = 8;
   // int x;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD8\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[3] |= (VolId & 0x0F);
    szCmd[4] = TestPattern;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;

    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    // check response content
    if (bSuccess){
        // check command
        if (bSuccess && szResponse[0] != Cmd){
            SDCARD_DEBUG(("CMD%d Response: Invalid command index. szResponse[0]=%08xh\r\n", Cmd, szResponse[0]));
            bSuccess = false;
        }
        
        // check voltage supplied        
        if (bSuccess && szResponse[3] != VolId){
            SDCARD_DEBUG(("CMD%d Response: Invalid Void Id, szResponse[3]=%08xh\r\n", Cmd, szResponse[3]));
            bSuccess = false;
        }        

        // check test pattern        
        if (bSuccess && szResponse[4] != TestPattern){
            SDCARD_DEBUG(("CMD%d Response: Invalid Test pattern, szResponse[4]=%08xh\r\n", Cmd, szResponse[4]));
            bSuccess = false;
        }
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
        
         
    }
    
    SDCARD_TRACE(("CMD8_END\r\n"));


    return bSuccess;
    
}

bool SD_CMD55(alt_u16 RCA16, alt_u8 ExpectedState){
    bool bSuccess;
    alt_u8 szResponse[6];
    const alt_u8 Cmd = 55;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD55\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] = RCA16 >> 8;
    szCmd[2] = RCA16 & 0xFF;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;
    
    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    // check response content
    if (bSuccess){
        alt_u32 CardStatus, ExpectedStatus;  // define in 4.10
        // check command
        if (bSuccess && szResponse[0] != Cmd){
            SDCARD_DEBUG(("CMD%d Response: Invalid command index. szResponse[0]=%08xh\r\n", Cmd, szResponse[0]));
            bSuccess = false;
        }
        
        // [1]~[4]: 32-bits card status
        CardStatus = ArrayToU32(&szResponse[1]);
        ExpectedStatus = SD_BIT_APP_CMD | SD_BIT_READY_FOR_DATA | SD_CURRENT_STATE_ENCODE(ExpectedState);
        if (CardStatus != ExpectedStatus){
            SDCARD_DEBUG(("CMD%d Response: unexpected stauts. response:%08xh, expected: %08xh\r\n", Cmd, CardStatus, ExpectedStatus));
            bSuccess = false;
        }
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
    SDCARD_TRACE(("CMD55_END\r\n"));


    return bSuccess;
    
}

// Response: R3
bool SD_ACMD41(alt_u32 HostOCR32, alt_u32 *pOCR32){
    alt_u32 OCR;
    bool bSuccess;
    
    alt_u8 szResponse[6];  // Response: R1, see section 4.9.4 (OCR Register)
    const alt_u8 Cmd = 41;
    //alt_u8 szCmd[6]  = {0x40,0x0f,0xf0,0x00,0x00, 0x00}, crc;
    //alt_u8 szCmd[6]  = {0x40,0x00,0xff,0x80,0x00, 0x00}, crc;  // 2.7~3.6V
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;  
    
    SDCARD_TRACE(("ACMD41\r\n"));
    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] |= (HostOCR32>> 24) & 0x40;
    szCmd[2] = (HostOCR32 >> 16) & 0xFF;
    szCmd[3] = (HostOCR32 >> 8) & 0xFF;
    szCmd[4] = HostOCR32 & 0xFF;
    /*szCmd[0] = 0x69;
    szCmd[1] = 0x40;
    szCmd[2] = 0xFF;
    szCmd[3] = 0x80;
    szCmd[4] = 0x00;*/
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;
    
    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    if (bSuccess){
        int i;
        // check command
        if (bSuccess && szResponse[0] != 0x3F){
            SDCARD_DEBUG(("ACMD%d Response: Invalid reserved bits\r\n", Cmd));
            bSuccess = false;
        }
        
        // [1]~[4]: 32-bits OCR (exampe: 0xFF8000)
        OCR = 0;
        for(i=0;i<4;i++){
            OCR <<= 8;
            OCR |= szResponse[i+1];
        }           
        
        //!!!!!!!!!! note. bit31(Card power up status bit) is LOW when the card is busy.
        if ((OCR & 0x80000000) != 0x80000000){
            // busy
            SDCARD_DEBUG(("ACMD%d Response: card is busy\r\n", Cmd));
            bSuccess = false;
        } 
        
        // check reserved bits and end-bits
        if (bSuccess && (szResponse[5] != 0xFF)){
            SDCARD_DEBUG(("ACMD%d Response: Invalid reserved & end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }      
    
   if (bSuccess)
      *pOCR32 = OCR;  

    return bSuccess;
}

// function: Asks any card to send the CID numbers on the CMD line (4.7.4)
// response: 4.9.3 R2 (CMD2 & CMD10)
// CID(Card IDentification): 128 bits = 128/8 = 16 bytes
bool SD_CMD2(alt_u8 szCID[], int nBufLen){
    bool bSuccess;
    alt_u8 szResponse[17];  // 136/8 = 17
    const alt_u8 Cmd = 2;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD2\r\n"));
    // build packet    
    szCmd[0] |= Cmd;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;
    
    //
 //   sd_Ncc();
    //for(x=0;x<40;x++)
    //    sd_Ncr();
    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE136, szResponse);
    
    // check response content
    if (bSuccess){
        // check command
        if (bSuccess && szResponse[0] != 0x3F){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }
        
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, &szResponse[1], 15);
            if (crc != (szResponse[16] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[16] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
    if (bSuccess){
        int nCopyLen, x;
        // [1]~[16]: CIS or CSD register
        nCopyLen = nBufLen;
        if (nCopyLen > 16)
            nCopyLen = 16;
        for(x=0;x<nCopyLen;x++)
            szCID[x] = szResponse[x+1];
        
    }
    
    return bSuccess;
    
}

//
bool SD_CMD3(alt_u16 *pRCA16){
    bool bSuccess;
    alt_u16 RCA16;
    alt_u8 szResponse[6];
    const alt_u8 Cmd = 3;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD3\r\n"));
    // build packet    
    szCmd[0] |= Cmd;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;


    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    // check response content
    if (bSuccess){
        alt_u16 Status16;
        alt_u8  State8;
        // check command
        if (bSuccess && szResponse[0] != Cmd){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }
        
        // New published RCA[31:16] of the card
        RCA16 = (szResponse[1] << 8) | szResponse[2];
        
        // [15:0] card status bits (16 bits: 23, 22, 19, 12~0) 0x500   
        Status16 = (szResponse[3] << 8) | szResponse[4];
        State8 = SD_CURRENT_STATE_DECODE(Status16);
        if (State8 != SD_STATE_IDENT){
           SDCARD_DEBUG(("CMD%d Response: Unexpected CURRENT_STATE\r\n", Cmd));
           bSuccess = false;
        }
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
    if (bSuccess)
        *pRCA16 = RCA16;
    
    return bSuccess;
    
}

bool SD_CMD9(alt_u16 RCA16, alt_u8 szCSD[], int nBufLen){
    bool bSuccess;
    alt_u8 szResponse[17];
    const alt_u8 Cmd = 9;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD9\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] |= RCA16 >> 8;
    szCmd[2] |= RCA16 & 0xFF;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;

    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE136, szResponse);
    
    // check response content
    if (bSuccess){
        // check command
        if (bSuccess && szResponse[0] != 0x3F){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, &szResponse[1], 15);
            if (crc != (szResponse[16] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[16] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
    if (bSuccess){
        int nCopyLen, x;
        if (nCopyLen > 16)
            nCopyLen = 16;
        for(x=0;x<nCopyLen;x++)
            szCSD[x] = szResponse[x+1];           
    }        
    
    return bSuccess;
}

bool SD_CMD10(alt_u16 RCA16, alt_u8 szCID[], int nBufLen){
    bool bSuccess;
    alt_u8 szResponse[17];
    const alt_u8 Cmd = 10;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD10\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] |= RCA16 >> 8;
    szCmd[2] |= RCA16 & 0xFF;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;

    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE136, szResponse);
    
    // check response content
    if (bSuccess){
        // check command
        if (bSuccess && szResponse[0] != 0x3F){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, &szResponse[1], 15);
            if (crc != (szResponse[16] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[16] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
    if (bSuccess){
        int nCopyLen, x;
        if (nCopyLen > 16)
            nCopyLen = 16;
        for(x=0;x<nCopyLen;x++)
            szCID[x] = szResponse[x+1];           
    }        
    
    return bSuccess;
}


bool SD_CMD7(alt_u16 RCA16){
    bool bSuccess;
    alt_u8 szResponse[6];
    const alt_u8 Cmd = 7;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD7\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] |= RCA16 >> 8;
    szCmd[2] |= RCA16 & 0xFF;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;

    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    // check response content
    if (bSuccess){
        alt_u32 CardStatus, ExpectedStatus;
        // check command
        if (bSuccess && szResponse[0] != Cmd){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }

        // [1]~[4]: 32-bits card status
        CardStatus = ArrayToU32(&szResponse[1]);
        ExpectedStatus = SD_BIT_READY_FOR_DATA | SD_CURRENT_STATE_ENCODE(SD_STATE_STBY);
        if (CardStatus != ExpectedStatus){
            SDCARD_DEBUG(("CMD%d Response: unexpected stauts\r\n", Cmd));
            bSuccess = false;
        }        
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
     
    
    return bSuccess;
}

bool SD_CMD16(alt_u32 BlockSize){
    bool bSuccess;
    alt_u8 szResponse[6];
    const alt_u8 Cmd = 16;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD16\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] |= BlockSize >> 24;
    szCmd[2] |= BlockSize >> 16;
    szCmd[3] |= BlockSize >> 8;
    szCmd[4] |= BlockSize & 0xFF;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;

    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    // check response content
    if (bSuccess){
        alt_u32 CardStatus, ExpectedStatus;
        // check command
        if (bSuccess && szResponse[0] != Cmd){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }

        // [1]~[4]: 32-bits card status
        CardStatus = ArrayToU32(&szResponse[1]);
        ExpectedStatus = SD_BIT_READY_FOR_DATA | SD_CURRENT_STATE_ENCODE(SD_STATE_TRAN);
        if (CardStatus != ExpectedStatus){
            SDCARD_DEBUG(("CMD%d Response: unexpected stauts\r\n", Cmd));
            bSuccess = false;
        }        
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
     
    
    return bSuccess;
}

// SET_BUS_WIDTH
// Response: R1
bool SD_ACMD6(bool b4BitsBus, alt_u8 ExpectedState){

    bool bSuccess;
    
    alt_u8 szResponse[6];  // Response: R1, see section 4.9.4 (OCR Register)
    const alt_u8 Cmd = 6;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc; 

    SDCARD_TRACE(("ACMD6\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[4] |= b4BitsBus?2:0;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;
    
    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    if (bSuccess){
        alt_u32 CardStatus, ExpectedStatus;
        // check command
        if (bSuccess && szResponse[0] != Cmd){
            SDCARD_DEBUG(("CMD%d Response: Invalid reserved bits\r\n", Cmd));
            bSuccess = false;
        }
        
        // [1]~[4]: CardStatus
        CardStatus = ArrayToU32(&szResponse[1]);
        ExpectedStatus = SD_BIT_APP_CMD | SD_BIT_READY_FOR_DATA | SD_CURRENT_STATE_ENCODE(ExpectedState);
        if (CardStatus != ExpectedStatus){
            SDCARD_DEBUG(("CMD%d Response: unexpected stauts\r\n", Cmd));
            bSuccess = false;
        }
                
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }      
    

    return bSuccess;
}

// SET_CLR_CARD_DETECT
// Response: R1
bool SD_ACMD42(bool b4BitsBus, alt_u8 ExpectedState){

    bool bSuccess;
    
    alt_u8 szResponse[6];  // Response: R1, see section 4.9.4 (OCR Register)
    const alt_u8 Cmd = 42;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc; 

    SDCARD_TRACE(("ACMD42\r\n"));

    // build packet    
    szCmd[0] |= Cmd;
    szCmd[4] |= b4BitsBus?1:0;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;
    
    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    if (bSuccess){
        alt_u32 CardStatus, ExpectedStatus;
        // check command
        if (bSuccess && szResponse[0] != Cmd){
            SDCARD_DEBUG(("CMD%d Response: Invalid reserved bits\r\n", Cmd));
            bSuccess = false;
        }
        
        // [1]~[4]: CardStatus
        CardStatus = ArrayToU32(&szResponse[1]);
        ExpectedStatus = SD_BIT_APP_CMD | SD_BIT_READY_FOR_DATA | SD_CURRENT_STATE_ENCODE(ExpectedState);
        if (CardStatus != ExpectedStatus){
            SDCARD_DEBUG(("CMD%d Response: unexpected stauts\r\n", Cmd));
            bSuccess = false;
        }
                
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }      
    

    return bSuccess;
}


// READ_SINGLE_BLOCK
// Response R1
bool SD_CMD17(alt_u32 Addr, alt_u8 ExpectedStatus){
    bool bSuccess;
    alt_u8 szResponse[6];
    const alt_u8 Cmd = 17;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD17\r\n"));
    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] |= (Addr >> 24) & 0xFF;
    szCmd[2] |= (Addr >> 16) & 0xFF;
    szCmd[3] |= (Addr >> 8) & 0xFF;
    szCmd[4] |= Addr & 0xFF;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;

    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);
    
    // check response content
    if (!bSuccess){
        SDCARD_DEBUG(("CMD%d failed to get Response\r\n", Cmd));
    }else{
        alt_u32 CardStatus, ExpectedStatus;
        // check command
        if (bSuccess && szResponse[0] != Cmd){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }

        // [1]~[4]: 32-bits card status
        CardStatus = ArrayToU32(&szResponse[1]);
        ExpectedStatus = SD_BIT_READY_FOR_DATA | SD_CURRENT_STATE_ENCODE(SD_STATE_TRAN);
        if (CardStatus != ExpectedStatus){
            SDCARD_DEBUG(("CMD%d Response: unexpected stauts\r\n", Cmd));
            bSuccess = false;
        }        
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
     
    
    return bSuccess;
}

// WRITE_BLOCK (Single)
// Response R1
bool SD_CMD24(alt_u32 Addr, alt_u8 ExpectedStatus){
    bool bSuccess;
    alt_u8 szResponse[6];
    const alt_u8 Cmd = 24;
    alt_u8 szCmd[6]  = {0x40,0x00,0x00,0x00,0x00, 0x00}, crc;

    SDCARD_TRACE(("CMD24\r\n"));
    // build packet    
    szCmd[0] |= Cmd;
    szCmd[1] |= (Addr >> 24) & 0xFF;
    szCmd[2] |= (Addr >> 16) & 0xFF;
    szCmd[3] |= (Addr >> 8) & 0xFF;
    szCmd[4] |= Addr & 0xFF;
    crc = crc7(0, szCmd, 5);
    szCmd[5] = (crc << 1) | 0x01;

    RTL_SetupCmd(szCmd, sizeof(szCmd));
    bSuccess = RTL_Execute(TXN_CMD_RESPONE48, szResponse);

    // check response content
    if (bSuccess){
        alt_u32 CardStatus, ExpectedStatus;
        // check command
        if (bSuccess && szResponse[0] != Cmd){ 
            SDCARD_DEBUG(("CMD%d Response: Invalid strat+tx+reserved bit\r\n", Cmd));
            bSuccess = false;
        }

        // [1]~[4]: 32-bits card status
        CardStatus = ArrayToU32(&szResponse[1]);
        ExpectedStatus = SD_BIT_READY_FOR_DATA | SD_CURRENT_STATE_ENCODE(SD_STATE_TRAN);
        if (CardStatus != ExpectedStatus){
            SDCARD_DEBUG(("CMD%d Response: unexpected stauts\r\n", Cmd));
            bSuccess = false;
        }        
        
        // check crc
        if (bSuccess){
            alt_u8 crc;
            crc = crc7(0, szResponse, 5);
            if (crc != (szResponse[5] >> 1)){
                SDCARD_DEBUG(("CMD%d Response: Invalid crc7\r\n", Cmd));
                bSuccess = false;
            }
        }            
        
        // check end-bits
        if (bSuccess && ((szResponse[5] & 0x01) != 0x01)){
            SDCARD_DEBUG(("CMD%d Response: Invalid end-bits\r\n", Cmd));
            bSuccess = false;
        }
          
    }
    
     
    
    return bSuccess;
}


bool SD_ReadData512(alt_u8 szBuf[]){
    bool bSuccess;

    SDCARD_TRACE(("SD_ReadData512\r\n"));
    bSuccess = RTL_Execute(TXN_BLOCK_READ, szBuf);
    return bSuccess;
}

bool SD_WriteData512(alt_u8 szDataWrite[]){
    bool bSuccess;
    SDCARD_TRACE(("SD_WriteData512\r\n"));
    RTL_SetupWriteData(szDataWrite);
   	bSuccess = RTL_Execute(TXN_BLOCK_WRITE, szDataWrite);
    return bSuccess;
}


alt_u32 ArrayToU32(alt_u8 szData[]){
    alt_u32 Value32 = 0;
    int i;
    
    for(i=0;i<4;i++){
        Value32 <<= 8;
        Value32 |= szData[i];
    }
    return Value32;
}


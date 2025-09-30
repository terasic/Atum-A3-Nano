module I2C_HDMI_Config (	//	Host Side
					iCLK,
					iRST_N,
					//	I2C Side
					I2C_SCLK,
					I2C_SDAT,
					HDMI_TX_INT,
					READY
					 );
//	Host Side
input				iCLK;
input				iRST_N;
//	I2C Side
output			I2C_SCLK;
inout				I2C_SDAT;
input				HDMI_TX_INT;
output READY ; 

//	Internal Registers/Wires
reg	[15:0]	mI2C_CLK_DIV;
reg	[23:0]	mI2C_DATA;
reg				mI2C_CTRL_CLK;
reg				mI2C_GO;
wire				mI2C_END;
wire				mI2C_ACK;
reg	[15:0]	LUT_DATA;
reg	[5:0]		LUT_INDEX;
reg	[3:0]		mSetup_ST;
reg READY ; 
reg				config_done;

//	Clock Setting
parameter	CLK_Freq	=	50000000;	//	50	MHz
parameter	I2C_Freq	=	20000;		//	20	KHz
//	LUT Data Number
parameter	LUT_SIZE	=	3;

/////////////////////	I2C Control Clock	////////////////////////
always@(posedge iCLK or negedge iRST_N)
begin
	if(!iRST_N)
	begin
		mI2C_CTRL_CLK	<=	0;
		mI2C_CLK_DIV	<=	0;
	end
	else
	begin
		if( mI2C_CLK_DIV	< (CLK_Freq/I2C_Freq) )
			mI2C_CLK_DIV	<=	mI2C_CLK_DIV+1;
		else
		begin
			mI2C_CLK_DIV	<=	0;
			mI2C_CTRL_CLK	<=	~mI2C_CTRL_CLK;
		end
	end
end
////////////////////////////////////////////////////////////////////
I2C_Controller 	u0	(	.CLOCK(mI2C_CTRL_CLK),	//	Controller Work Clock
						.I2C_SCLK(I2C_SCLK),				//	I2C CLOCK
 	 	 	 	 	 	.I2C_SDAT(I2C_SDAT),				//	I2C DATA
						.I2C_DATA(mI2C_DATA),			//	DATA:[SLAVE_ADDR,SUB_ADDR,DATA]
						.GO(mI2C_GO),						//	GO transfor
						.END(mI2C_END),					//	END transfor 
						.ACK(mI2C_ACK),					//	ACK
						.RESET(iRST_N & !config_done)	);
////////////////////////////////////////////////////////////////////
//////////////////////	Config Control	////////////////////////////
always@(posedge mI2C_CTRL_CLK or negedge iRST_N)
begin
	if(!iRST_N)
	begin
	READY<=0;
		LUT_INDEX	<=	0;
		mSetup_ST	<=	0;
		mI2C_GO		<=	0;
		config_done <= 0;
	end
	else
	begin
		if(LUT_INDEX<LUT_SIZE)
		begin
		READY<=0;
			case(mSetup_ST)
			0:	begin
					mI2C_DATA	<=	{8'h78,LUT_DATA};
					mI2C_GO		<=	1;
					mSetup_ST	<=	1;
				end
			1:	begin
					if(mI2C_END)
					begin
						if(!mI2C_ACK)
						mSetup_ST	<=	2;
						else
						mSetup_ST	<=	0;							
						mI2C_GO		<=	0;
					end
				end
			2:	begin
					LUT_INDEX	<=	LUT_INDEX+6'd1;
					mSetup_ST	<=	0;
				end
			default : mSetup_ST <= mSetup_ST;
			endcase
		end
		else
		begin
		   READY<=1; 
			mSetup_ST <= 3;
			mI2C_GO <= 0;
		   LUT_INDEX <= LUT_INDEX;
			config_done <= 1;
			
		end
	end
end
////////////////////////////////////////////////////////////////////
/////////////////////	Config Data LUT	  //////////////////////////	
always@*
begin
	case(LUT_INDEX)
	
	
//    Video Config Data for TFP410 (Device Addr: 0x78)
//    SubAddr : Data
	2    :    LUT_DATA    <=    16'h0837;    // CTL_1_MODE: Normal Op, 24-bit, Rising Edge, H/V Sync En, TMDS En
	1    :    LUT_DATA    <=    16'h0A80;    // CTL_3_MODE: De-skew disabled, CTL[2:1]=0 (Sesuai reset value)
	0    :    LUT_DATA    <=    16'h3330;    // DE_CTL: DE Gen Disabled, VS_POL=1 (Active High), HS_POL=1 (Active High)
	default:		LUT_DATA	<=	16'h0837;
	endcase
end
////////////////////////////////////////////////////////////////////
endmodule
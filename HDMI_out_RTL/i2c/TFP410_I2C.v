
module TFP410_I2C (
   output reg        READY , 
   input             clk50,
   input             reset_n,
   output            i2c_clk,
   inout             i2c_dat,
   input [7:0]       SLAVE_ADDR_R,
	output reg [15:0] VEN_ID,
	output reg [15:0] DEV_ID,

	//----test 
	output reg [7:0]  BYTE ,
	output reg [7:0]  SLAVE_ADDR , 
   output reg [7:0]  ST ,
   output reg [2:0]  CNT,
	output reg [9:0]  WCNT,
   output reg [7:0]  W_WORD_DATA  ,
   output reg [7:0]  W_POINTER_REG,
	
	output            W_WORD_END ,
   output reg        W_WORD_GO ,
	output            W_POINTER_END,
   output reg        W_POINTER_GO,
   output            R_END,
   output reg        R_GO,
   output     [15:0] R_DATA ,
	output            SDAO,
	output            SCLO,
	output            CLK_400K

);

//----400KHZ generater 
CLOCKMEM K0(  .CLK      (clk50), .CLK_FREQ (125), .CK_1HZ   (CLK_400K) ) ;	

//=======================================================
//  WIRE /REGISETER 
//=======================================================

//--I2C Main Controller--- 
reg  [31:0] DELY;

//-----I2C-BUS-I/O----

wire        W_WORD_SCL; 
wire        W_WORD_SDAO;  
wire        W_POINTER_SCL; 
wire        W_POINTER_SDAO; 
wire        R_SCL; 
wire        R_SDAO;  

//reg  [12:0] FAN_RPS;
//reg  [ 7:0] KTACH

//=======================================================
//  Parameter  
//=======================================================

// read /write MAX 
parameter WRITE_MAX             = 1; 
parameter READ_MAX              = 2; 
        
//read Register
parameter    P_VEN_ID           =8'h0;
parameter    P_DEV_ID           =8'h2;

//write Register 
parameter    P_CTL_1_MODE       =8'h08;// reset = 0xBE

//------------------------------------
//=======================================================
//  Structural coding
//======================================================= 
																 
//--I2C Main Controller--- 
always @(negedge reset_n or posedge CLK_400K )begin 
if (!reset_n  ) begin 
    ST           <=0; //<-------read
 	 W_POINTER_GO <=1;
    R_GO         <=1 ;		 
	 W_WORD_GO    <=1;
	 WCNT         <=0;  
	 CNT          <=0;
	 DELY         <=0 ;
	 READY        <=0;
end 
else begin
case (ST)
0: begin 
    ST      <=31; //---First , to do  I2C write. 
	 W_POINTER_GO <=1;
    R_GO  <=1 ;		 
	 W_WORD_GO <=1;
	 WCNT <=0;  
	 CNT  <=0;
	 DELY <=0 ;
   end
//------------- READ -------	
1: begin 
    ST<=2; 
	end	
2: begin
        // READ ID
	          if ( CNT==0)      {SLAVE_ADDR[7:0] ,W_POINTER_REG,BYTE}<= {SLAVE_ADDR_R[7:0] ,P_VEN_ID  ,8'h2 };
	          if ( CNT==1)      {SLAVE_ADDR[7:0] ,W_POINTER_REG,BYTE}<= {SLAVE_ADDR_R[7:0] ,P_DEV_ID  ,8'h2 };
	if ( W_POINTER_END ) begin  
	   W_POINTER_GO  <=0; 
		ST<=3 ; 
		DELY<=0;  
	 end
	end                
	//------- Write Pointer
3: begin 
    DELY  <=DELY +1;
    if ( DELY ==2 ) begin 
      W_POINTER_GO  <=1;
      ST<=4 ; 
	 end
	end       
4: begin 
    if  ( W_POINTER_END ) ST<=5 ; 	
	end              
5: begin 
    ST<=6 ; 
	end 
	//------- Read DATA  		 
6: begin 
	 if ( R_END ) begin  
	  R_GO  <=0; 
	  ST<=7 ; 
	  DELY<=0; 
	 end
	end                
7: begin 
    DELY  <=DELY +1;
    if ( DELY ==2 ) begin 	 
      R_GO  <=1;
      ST<=8 ; 
	 end
	end       
8: begin 
     ST<=9 ; 
	end       
9: begin 
   if  ( R_END ) 
	 begin 		
    //READ DATA 	 
		             if ( CNT==0) 	VEN_ID     ={R_DATA[7:0],R_DATA[15:8]};
		        else if ( CNT==1) 	DEV_ID     ={R_DATA[7:0],R_DATA[15:8]};
	      ST  <=10 ; 
		   CNT <= CNT +1; 	
	 end 
  end	
10: begin   
     if ( CNT == READ_MAX) ST  <=11; else  ST<=2; 		
		  DELY <=0;
	     W_POINTER_GO <=1;
        R_GO         <=1 ;		 
	     W_WORD_GO    <=1; 		    		 		  
	 end 
11:begin 
	    ST            <=11;
		 CNT           <=0;
       READY         <=1;		 
end	 
//---- READ END ----


//---- WRITE--------
31: begin 
		   {SLAVE_ADDR[7:0] ,W_POINTER_REG,W_WORD_DATA,BYTE}<= {SLAVE_ADDR_R[7:0] ,P_CTL_1_MODE ,8'hbf,8'h1 };
	     if (  W_WORD_END ) begin  
	     W_WORD_GO  <=0; 
		  ST   <=32 ;  
		  DELY <=0;  
	 end
	end           
32: begin 
    DELY  <=DELY +1;
	   if (  DELY ==5 )  begin
        W_WORD_GO  <=1;
        ST<=33 ; 
	 end
	 
	end       
33: begin 
    ST<=34 ; 
	end       	
34: begin 
     if  ( W_WORD_END )  begin 	
			 WCNT<=WCNT+1 ;			 
			 ST  <=35 ; 
	  end
	end              
35: begin 
    if ( WCNT != WRITE_MAX ) ST<=31 ;else ST <= 36; 
	 end 
36: begin
	    ST   <=1; //to Read
		 CNT  <=0;
	 end 
endcase
end
end
//-----------------------------MAIN-ST END ------------------------------------------

//============inout  i2c  q15(a10) example ===============  
// wire const_zero_sig /* synthesis keep */;
// assign const_zero_sig = 1\'b0;
// assign TRI_PIN = enable? const_zero_sig : \'bz;
//========================================================
wire   const_zero_sig/* synthesis keep */ ; 
assign const_zero_sig = 0 ; 
assign i2c_dat        = (SDAO)?1'bz :0;
assign i2c_clk        = (SCLO)?1'bz :0;
assign SCLO           = W_POINTER_SCL  & R_SCL   & W_WORD_SCL;
assign SDAO           = W_POINTER_SDAO & R_SDAO  & W_WORD_SDAO;


//==== I2C WRITE WORD ===
I2C_WRITE  I2C_WRITE_i(
   .BYTE_NUM     ( BYTE   ),
   .RESET_N      ( reset_n),
	.PT_CK        ( CLK_400K),
	.GO           ( W_WORD_GO),
	.POINTER      ( W_POINTER_REG),
   .WDATA	     ( W_WORD_DATA),
	.SLAVE_ADDRESS( SLAVE_ADDR ),
	.SDAI         ( i2c_dat),
	.SDAO         ( W_WORD_SDAO),
	.SCLO         ( W_WORD_SCL ),
	.END_OK       ( W_WORD_END)
);

//==== I2C WRITE POINTER ===
I2C_WRITE_POINTER  I2C_WRITE_POINTER_i(
   .RESET_N      (reset_n   ),
	.PT_CK        ( CLK_400K ),
	.GO           ( W_POINTER_GO ),
	.POINTER      ( W_POINTER_REG),
	.SLAVE_ADDRESS( SLAVE_ADDR ),
	.SDAI         ( i2c_dat    ),
	.SDAO         ( W_POINTER_SDAO),
	.SCLO         ( W_POINTER_SCL ),
	.END_OK       ( W_POINTER_END )
);
//-----I2C TO READ---- 
I2C_READ_DATA I2C_READ_DATA_i( 
   .RESET_N      ( reset_n    ),
	.PT_CK        ( CLK_400K   ),
	.GO           ( R_GO       ),
	.SLAVE_ADDRESS( SLAVE_ADDR ),
 	.BYTE_NUM     ( BYTE       ) , // I2C read 8 bit data 
	.SDAI         ( i2c_dat),
	.SDAO         ( R_SDAO ),
	.SCLO         ( R_SCL  ),
	.END_OK       ( R_END  ),
	.DATA         ( R_DATA )
);	


endmodule





































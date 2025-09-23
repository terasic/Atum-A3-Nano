module RESET_DELY ( 
   input  BUTTON , 
	input  OSC_50 , 
   output RESET_n  
) ; 

parameter SEC  = 50_000_000 ; 
parameter SEC1 = 1*SEC  ; 
parameter SEC2 = 2*SEC  ; 
parameter SEC10 = 10*SEC  ; 
parameter SEC0d5 = SEC/2  ; 

//=======================================================
//  REG/WIRE declarations
//=======================================================
reg [31:0] DELAY_CNT  ; 
  
//=======================================================
//  Structural coding
//=======================================================	

assign RESET_n =( ( DELAY_CNT>=0 ) &&  ( DELAY_CNT< SEC0d5) )?0:1 ; 

always @(negedge  BUTTON or posedge OSC_50  ) 
 if (  !BUTTON) begin 
     DELAY_CNT<=0;
 end
 else  begin 
    if  ( DELAY_CNT < SEC) DELAY_CNT <= DELAY_CNT+1; 
 end 
 
endmodule 
#**************************************************************
# This .sdc file is created by Terasic Tool.
# Users are recommended to modify this file to match users logic.
#**************************************************************

#**************************************************************
# Create Clock
#**************************************************************
create_clock -period "50.0 MHz" [get_ports CLOCK0_50]
create_clock -period "50.0 MHz" [get_ports CLOCK1_50]
create_clock -period "50.0 MHz" [get_ports CLOCK2_50]
create_clock -period "50.0 MHz" [get_ports CLOCK3_50]

#SDRAM CLK
create_generated_clock -source [get_pins {u0|iopll_0|iopll_0|tennm_ph2_iopll|out_clk[1]}] \
                      -name clk_dram_ext [get_ports {DRAM_CLK}]
#HDMI CLK
create_generated_clock -source [get_pins {u0|iopll_0|iopll_0|tennm_ph2_iopll|out_clk[2]}] \
                      -name clk_hdmi_ext [get_ports {HDMI_TX_CLK_p}]

#D8M                             
create_clock -period "25.0 MHz" -name MIPI_PIXEL_CLK [get_ports MIPI_PIXEL_CLK]
create_clock -period "25.0 MHz" -name MIPI_PIXEL_CLK_ext  

#**************************************************************
# Create Generated Clock
#**************************************************************
derive_pll_clocks



#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************
derive_clock_uncertainty



#**************************************************************
# Set Input Delay
#**************************************************************
# tpd  min 1ns ,max 6ns
# suppose +- 100 ps skew
# Board Delay (Data) + Propagation Delay - Board Delay (Clock)

# D8M                         
# tpd  min 1ns ,max 6ns
set_input_delay -max 6.1 -clock  MIPI_PIXEL_CLK_ext  [get_ports {MIPI_PIXEL_VS MIPI_PIXEL_HS MIPI_PIXEL_D[*]}]
set_input_delay -min 0.9 -clock  MIPI_PIXEL_CLK_ext  [get_ports {MIPI_PIXEL_VS MIPI_PIXEL_HS MIPI_PIXEL_D[*]}]

# SDRAM
# max 5.5(max) +0.4(trace delay) +0.1  = 6.0
# min 2.7(min) +0.4(trace delay) -0.1  = 3.0
set_input_delay -max -clock clk_dram_ext 6.0 [get_ports DRAM_DQ*]
set_input_delay -min -clock clk_dram_ext 3.0 [get_ports DRAM_DQ*]

#shift-window
set_multicycle_path -from [get_clocks {clk_dram_ext}] \
                    -to [get_clocks {u6|iopll_0|tennm_ph2_iopll|out_clk[0]}] \
                    -setup 2   

#**************************************************************
# Set Output Delay
#**************************************************************
# suppose +- 100 ps skew
# max : Board Delay (Data) - Board Delay (Clock) + tsu (External Device)
# min : Board Delay (Data) - Board Delay (Clock) - th (External Device)

#HDMI
# max  0.2+0.1 =  0.3
# min -1.5-0.1 = -1.6
set_output_delay -max -clock clk_hdmi_ext  0.3 [get_ports {HDMI_TX_D* HDMI_TX_DE}]
set_output_delay -min -clock clk_hdmi_ext -1.6 [get_ports {HDMI_TX_D* HDMI_TX_DE}]

#SDRAM
# max  1.5+0.1 =  1.6
# min -1.0-0.1 = -1.1
set_output_delay -max -clock clk_dram_ext 1.6  [get_ports {DRAM_DQ* DRAM_DQM*}]
set_output_delay -min -clock clk_dram_ext -1.1 [get_ports {DRAM_DQ* DRAM_DQM*}]
set_output_delay -max -clock clk_dram_ext 1.6  [get_ports {DRAM_ADDR* DRAM_BA* DRAM_RAS_N DRAM_CAS_N DRAM_WE_N DRAM_CKE DRAM_CS_N}]
set_output_delay -min -clock clk_dram_ext -1.1 [get_ports {DRAM_ADDR* DRAM_BA* DRAM_RAS_N DRAM_CAS_N DRAM_WE_N DRAM_CKE DRAM_CS_N}]

#**************************************************************
# Set Clock Groups
#**************************************************************



#**************************************************************
# Set False Path
#**************************************************************



#**************************************************************
# Set Multicycle Path
#**************************************************************



#**************************************************************
# Set Maximum Delay
#**************************************************************



#**************************************************************
# Set Minimum Delay
#**************************************************************



#**************************************************************
# Set Input Transition
#**************************************************************



#**************************************************************
# Set Load
#**************************************************************




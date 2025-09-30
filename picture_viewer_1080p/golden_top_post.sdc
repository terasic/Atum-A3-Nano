#**************************************************************
# This .sdc file is created by Terasic Tool.
# Users are recommended to modify this file to match users logic.
#**************************************************************



#**************************************************************
# Create Generated Clock
#**************************************************************
# SDRAM CLK
create_generated_clock -source [get_pins {nios_system_Inst|iopll|iopll|tennm_ph2_iopll|out_clk[1]}] \
                      -name clk_dram_ext [get_ports {DRAM_CLK}]

# HDMI CLK
create_generated_clock -source [get_pins {nios_system_Inst|iopll_vga|iopll_0|tennm_ph2_iopll|out_clk[1]}] \
                      -name hdmi_tx_clk [get_ports {HDMI_TX_CLK_p}]


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
# max: Board Delay (Data) + Board Delay (Clock) + tsu (External Device)
# min: Board Delay (Data) + Board Delay (Clock) + th (External Device)
# max(CAS = 2): 8(tAC2) + 0.3(clock board delay) + 0.28(data board delay) = 8.58
# max(CAS = 3): 5.5(tAC3) + 0.3(clock board delay) + 0.28(data board delay) = 6.08
# min: 2.5(tOH) + 0.3(clock trace delay) + 0.27(data trace delay) = 3.07
set_input_delay -max -clock clk_dram_ext 8.58 [get_ports DRAM_DQ[*]]
set_input_delay -min -clock clk_dram_ext 3.07 [get_ports DRAM_DQ[*]]

#shift-window
set_multicycle_path -from [get_clocks {clk_dram_ext}] \
                    -to [get_clocks {nios_system_Inst|iopll|iopll_outclk2}] \
                    -setup 2


#**************************************************************
# Set Output Delay
#**************************************************************
# max: Board Delay (Data/Cmd) - Board Delay (Clock) + tsu (External Device)
# min: Board Delay (Data/Cmd) - Board Delay (Clock) - th (External Device)
# max: 0.28(data board delay) - 0.3(clock board delay) + 1.5(tDS) = 1.48
# min: 0.27(data board delay) - 0.3(clock board delay) - 1.0(tDH) = -1.03
# max: 0.33(cmd board delay) - 0.3(clock board delay) + 1.5(tDS) = 1.53
# min: 0.28(cmd board delay) - 0.3(clock board delay) - 1.0(tDH) = -1.02
set_output_delay -max -clock clk_dram_ext 1.48  [get_ports {DRAM_DQ* DRAM_DQM*}]
set_output_delay -min -clock clk_dram_ext -1.03 [get_ports {DRAM_DQ* DRAM_DQM*}]
set_output_delay -max -clock clk_dram_ext 1.53  [get_ports {DRAM_ADDR* DRAM_BA* DRAM_RAS_n DRAM_CAS_n DRAM_WE_n DRAM_CKE DRAM_CS_n}]
set_output_delay -min -clock clk_dram_ext -1.02 [get_ports {DRAM_ADDR* DRAM_BA* DRAM_RAS_n DRAM_CAS_n DRAM_WE_n DRAM_CKE DRAM_CS_n}]

set clk_name "hdmi_tx_clk"
set fpga_output_ports [get_ports {HDMI_TX_D[*] HDMI_TX_DE HDMI_TX_HS HDMI_TX_VS}]
set max_output_delay [expr {1.2 + 0.0}] ; # 1.2ns t_su + 0.0ns board_delay_max
set min_output_delay [expr {-1.3 + 0.0}] ; # -1.3ns t_h + 0.0ns board_delay_min
set_output_delay -clock $clk_name -max $max_output_delay $fpga_output_ports
set_output_delay -clock $clk_name -min $min_output_delay $fpga_output_ports







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




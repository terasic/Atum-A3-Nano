/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'intel_niosv_m' in SOPC Builder design 'system'
 *
 * Generated: Thu Jan 08 18:10:54 CST 2026
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "intel_niosv_m"
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_DATA_ADDR_WIDTH 0x20
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HAS_CSR_SUPPORT 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_ICACHE_LINE_SIZE 0
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_ICACHE_SIZE 0
#define ALT_CPU_INST_ADDR_WIDTH 0x20
#define ALT_CPU_INT_MODE 0
#define ALT_CPU_MTIME_OFFSET 0x00050880
#define ALT_CPU_NAME "intel_niosv_m"
#define ALT_CPU_NIOSV_CORE_VARIANT 1
#define ALT_CPU_NUM_GPR 32
#define ALT_CPU_RESET_ADDR 0x00000000
#define ALT_CPU_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define ALT_CPU_TIMER_DEVICE_TYPE 2


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define ABBOTTSLAKE_CPU_FREQ 100000000u
#define ABBOTTSLAKE_DATA_ADDR_WIDTH 0x20
#define ABBOTTSLAKE_DCACHE_LINE_SIZE 0
#define ABBOTTSLAKE_DCACHE_LINE_SIZE_LOG2 0
#define ABBOTTSLAKE_DCACHE_SIZE 0
#define ABBOTTSLAKE_HAS_CSR_SUPPORT 1
#define ABBOTTSLAKE_HAS_DEBUG_STUB
#define ABBOTTSLAKE_ICACHE_LINE_SIZE 0
#define ABBOTTSLAKE_ICACHE_LINE_SIZE_LOG2 0
#define ABBOTTSLAKE_ICACHE_SIZE 0
#define ABBOTTSLAKE_INST_ADDR_WIDTH 0x20
#define ABBOTTSLAKE_INT_MODE 0
#define ABBOTTSLAKE_MTIME_OFFSET 0x00050880
#define ABBOTTSLAKE_NIOSV_CORE_VARIANT 1
#define ABBOTTSLAKE_NUM_GPR 32
#define ABBOTTSLAKE_RESET_ADDR 0x00000000
#define ABBOTTSLAKE_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define ABBOTTSLAKE_TIMER_DEVICE_TYPE 2


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_I2C
#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __INTEL_LW_UART
#define __INTEL_NIOSV_M
#define __INTEL_ONCHIP_MEMORY
#define __INTEL_VVP_SCALER
#define __INTEL_VVP_VFW
#define __ONE_AI_INTERFACE
#define __TERASIC_AUTO_FOCUS


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "AGILEX3"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x50978
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x50978
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x50978
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSID_BASE SYSID_QSYS_BASE
#define ALT_SYSID_ID SYSID_QSYS_ID
#define ALT_SYSTEM_NAME "system"
#define ALT_SYS_CLK_TICKS_PER_SEC ALT_CPU_TICKS_PER_SEC
#define ALT_TIMESTAMP_CLK_TIMER_DEVICE_TYPE ALT_CPU_TIMER_DEVICE_TYPE


/*
 * hal2 configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK INTEL_NIOSV_M
#define ALT_TIMESTAMP_CLK INTEL_NIOSV_M
#define INTEL_FPGA_DFL_START_ADDRESS 0xffffffffffffffff
#define INTEL_FPGA_USE_DFL_WALKER 0


/*
 * i2c_camera configuration
 *
 */

#define ALT_MODULE_CLASS_i2c_camera altera_avalon_i2c
#define I2C_CAMERA_BASE 0x50800
#define I2C_CAMERA_FIFO_DEPTH 4
#define I2C_CAMERA_FREQ 100000000
#define I2C_CAMERA_IRQ 3
#define I2C_CAMERA_IRQ_INTERRUPT_CONTROLLER_ID 0
#define I2C_CAMERA_NAME "/dev/i2c_camera"
#define I2C_CAMERA_SPAN 64
#define I2C_CAMERA_TYPE "altera_avalon_i2c"
#define I2C_CAMERA_USE_AV_ST 0


/*
 * i2c_mipi configuration
 *
 */

#define ALT_MODULE_CLASS_i2c_mipi altera_avalon_i2c
#define I2C_MIPI_BASE 0x50840
#define I2C_MIPI_FIFO_DEPTH 4
#define I2C_MIPI_FREQ 100000000
#define I2C_MIPI_IRQ 4
#define I2C_MIPI_IRQ_INTERRUPT_CONTROLLER_ID 0
#define I2C_MIPI_NAME "/dev/i2c_mipi"
#define I2C_MIPI_SPAN 64
#define I2C_MIPI_TYPE "altera_avalon_i2c"
#define I2C_MIPI_USE_AV_ST 0


/*
 * intel_niosv_m_dm_agent configuration
 *
 */

#define ALT_MODULE_CLASS_intel_niosv_m_dm_agent intel_niosv_m
#define INTEL_NIOSV_M_DM_AGENT_BASE 0x40000
#define INTEL_NIOSV_M_DM_AGENT_CPU_FREQ 100000000u
#define INTEL_NIOSV_M_DM_AGENT_DATA_ADDR_WIDTH 0x20
#define INTEL_NIOSV_M_DM_AGENT_DCACHE_LINE_SIZE 0
#define INTEL_NIOSV_M_DM_AGENT_DCACHE_LINE_SIZE_LOG2 0
#define INTEL_NIOSV_M_DM_AGENT_DCACHE_SIZE 0
#define INTEL_NIOSV_M_DM_AGENT_HAS_CSR_SUPPORT 1
#define INTEL_NIOSV_M_DM_AGENT_HAS_DEBUG_STUB
#define INTEL_NIOSV_M_DM_AGENT_ICACHE_LINE_SIZE 0
#define INTEL_NIOSV_M_DM_AGENT_ICACHE_LINE_SIZE_LOG2 0
#define INTEL_NIOSV_M_DM_AGENT_ICACHE_SIZE 0
#define INTEL_NIOSV_M_DM_AGENT_INST_ADDR_WIDTH 0x20
#define INTEL_NIOSV_M_DM_AGENT_INT_MODE 0
#define INTEL_NIOSV_M_DM_AGENT_IRQ -1
#define INTEL_NIOSV_M_DM_AGENT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define INTEL_NIOSV_M_DM_AGENT_MTIME_OFFSET 0x00050880
#define INTEL_NIOSV_M_DM_AGENT_NAME "/dev/intel_niosv_m_dm_agent"
#define INTEL_NIOSV_M_DM_AGENT_NIOSV_CORE_VARIANT 1
#define INTEL_NIOSV_M_DM_AGENT_NUM_GPR 32
#define INTEL_NIOSV_M_DM_AGENT_RESET_ADDR 0x00000000
#define INTEL_NIOSV_M_DM_AGENT_SPAN 65536
#define INTEL_NIOSV_M_DM_AGENT_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define INTEL_NIOSV_M_DM_AGENT_TIMER_DEVICE_TYPE 2
#define INTEL_NIOSV_M_DM_AGENT_TYPE "intel_niosv_m"


/*
 * intel_niosv_m_hal_driver configuration
 *
 */

#define NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND 1000


/*
 * intel_niosv_m_timer_sw_agent configuration
 *
 */

#define ALT_MODULE_CLASS_intel_niosv_m_timer_sw_agent intel_niosv_m
#define INTEL_NIOSV_M_TIMER_SW_AGENT_BASE 0x50880
#define INTEL_NIOSV_M_TIMER_SW_AGENT_CPU_FREQ 100000000u
#define INTEL_NIOSV_M_TIMER_SW_AGENT_DATA_ADDR_WIDTH 0x20
#define INTEL_NIOSV_M_TIMER_SW_AGENT_DCACHE_LINE_SIZE 0
#define INTEL_NIOSV_M_TIMER_SW_AGENT_DCACHE_LINE_SIZE_LOG2 0
#define INTEL_NIOSV_M_TIMER_SW_AGENT_DCACHE_SIZE 0
#define INTEL_NIOSV_M_TIMER_SW_AGENT_HAS_CSR_SUPPORT 1
#define INTEL_NIOSV_M_TIMER_SW_AGENT_HAS_DEBUG_STUB
#define INTEL_NIOSV_M_TIMER_SW_AGENT_ICACHE_LINE_SIZE 0
#define INTEL_NIOSV_M_TIMER_SW_AGENT_ICACHE_LINE_SIZE_LOG2 0
#define INTEL_NIOSV_M_TIMER_SW_AGENT_ICACHE_SIZE 0
#define INTEL_NIOSV_M_TIMER_SW_AGENT_INST_ADDR_WIDTH 0x20
#define INTEL_NIOSV_M_TIMER_SW_AGENT_INT_MODE 0
#define INTEL_NIOSV_M_TIMER_SW_AGENT_IRQ -1
#define INTEL_NIOSV_M_TIMER_SW_AGENT_IRQ_INTERRUPT_CONTROLLER_ID -1
#define INTEL_NIOSV_M_TIMER_SW_AGENT_MTIME_OFFSET 0x00050880
#define INTEL_NIOSV_M_TIMER_SW_AGENT_NAME "/dev/intel_niosv_m_timer_sw_agent"
#define INTEL_NIOSV_M_TIMER_SW_AGENT_NIOSV_CORE_VARIANT 1
#define INTEL_NIOSV_M_TIMER_SW_AGENT_NUM_GPR 32
#define INTEL_NIOSV_M_TIMER_SW_AGENT_RESET_ADDR 0x00000000
#define INTEL_NIOSV_M_TIMER_SW_AGENT_SPAN 64
#define INTEL_NIOSV_M_TIMER_SW_AGENT_TICKS_PER_SEC NIOSV_INTERNAL_TIMER_TICKS_PER_SECOND
#define INTEL_NIOSV_M_TIMER_SW_AGENT_TIMER_DEVICE_TYPE 2
#define INTEL_NIOSV_M_TIMER_SW_AGENT_TYPE "intel_niosv_m"


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x50978
#define JTAG_UART_IRQ 0
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * key configuration
 *
 */

#define ALT_MODULE_CLASS_key altera_avalon_pio
#define KEY_BASE 0x50930
#define KEY_BIT_CLEARING_EDGE_REGISTER 0
#define KEY_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEY_CAPTURE 0
#define KEY_DATA_WIDTH 2
#define KEY_DO_TEST_BENCH_WIRING 0
#define KEY_DRIVEN_SIM_VALUE 0
#define KEY_EDGE_TYPE "NONE"
#define KEY_FREQ 100000000
#define KEY_HAS_IN 1
#define KEY_HAS_OUT 0
#define KEY_HAS_TRI 0
#define KEY_IRQ -1
#define KEY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define KEY_IRQ_TYPE "NONE"
#define KEY_NAME "/dev/key"
#define KEY_RESET_VALUE 0
#define KEY_SPAN 16
#define KEY_TYPE "altera_avalon_pio"


/*
 * led configuration
 *
 */

#define ALT_MODULE_CLASS_led altera_avalon_pio
#define LED_BASE 0x50960
#define LED_BIT_CLEARING_EDGE_REGISTER 0
#define LED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_CAPTURE 0
#define LED_DATA_WIDTH 4
#define LED_DO_TEST_BENCH_WIRING 0
#define LED_DRIVEN_SIM_VALUE 0
#define LED_EDGE_TYPE "NONE"
#define LED_FREQ 100000000
#define LED_HAS_IN 0
#define LED_HAS_OUT 1
#define LED_HAS_TRI 0
#define LED_IRQ -1
#define LED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LED_IRQ_TYPE "NONE"
#define LED_NAME "/dev/led"
#define LED_RESET_VALUE 0
#define LED_SPAN 16
#define LED_TYPE "altera_avalon_pio"


/*
 * lw_uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_lw_uart_0 intel_lw_uart
#define LW_UART_0_BASE 0x508e0
#define LW_UART_0_BAUD 115200
#define LW_UART_0_DATA_BITS 8
#define LW_UART_0_FIXED_BAUD 1
#define LW_UART_0_FREQ 100000000
#define LW_UART_0_IRQ 2
#define LW_UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define LW_UART_0_NAME "/dev/lw_uart_0"
#define LW_UART_0_PARITY 'N'
#define LW_UART_0_READ_DEPTH 2048
#define LW_UART_0_SIM_TRUE_BAUD 0
#define LW_UART_0_SPAN 32
#define LW_UART_0_STOP_BITS 1
#define LW_UART_0_SYNC_REG_DEPTH 2
#define LW_UART_0_TYPE "intel_lw_uart"
#define LW_UART_0_USE_CTS_RTS 0
#define LW_UART_0_USE_EOP_REGISTER 0
#define LW_UART_0_WRITE_DEPTH 2048


/*
 * mipi_pwdn_n configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_pwdn_n altera_avalon_pio
#define MIPI_PWDN_N_BASE 0x50940
#define MIPI_PWDN_N_BIT_CLEARING_EDGE_REGISTER 0
#define MIPI_PWDN_N_BIT_MODIFYING_OUTPUT_REGISTER 0
#define MIPI_PWDN_N_CAPTURE 0
#define MIPI_PWDN_N_DATA_WIDTH 1
#define MIPI_PWDN_N_DO_TEST_BENCH_WIRING 0
#define MIPI_PWDN_N_DRIVEN_SIM_VALUE 0
#define MIPI_PWDN_N_EDGE_TYPE "NONE"
#define MIPI_PWDN_N_FREQ 100000000
#define MIPI_PWDN_N_HAS_IN 0
#define MIPI_PWDN_N_HAS_OUT 1
#define MIPI_PWDN_N_HAS_TRI 0
#define MIPI_PWDN_N_IRQ -1
#define MIPI_PWDN_N_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_PWDN_N_IRQ_TYPE "NONE"
#define MIPI_PWDN_N_NAME "/dev/mipi_pwdn_n"
#define MIPI_PWDN_N_RESET_VALUE 0
#define MIPI_PWDN_N_SPAN 16
#define MIPI_PWDN_N_TYPE "altera_avalon_pio"


/*
 * mipi_reset_n configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_reset_n altera_avalon_pio
#define MIPI_RESET_N_BASE 0x50920
#define MIPI_RESET_N_BIT_CLEARING_EDGE_REGISTER 0
#define MIPI_RESET_N_BIT_MODIFYING_OUTPUT_REGISTER 0
#define MIPI_RESET_N_CAPTURE 0
#define MIPI_RESET_N_DATA_WIDTH 1
#define MIPI_RESET_N_DO_TEST_BENCH_WIRING 0
#define MIPI_RESET_N_DRIVEN_SIM_VALUE 0
#define MIPI_RESET_N_EDGE_TYPE "NONE"
#define MIPI_RESET_N_FREQ 100000000
#define MIPI_RESET_N_HAS_IN 0
#define MIPI_RESET_N_HAS_OUT 1
#define MIPI_RESET_N_HAS_TRI 0
#define MIPI_RESET_N_IRQ -1
#define MIPI_RESET_N_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_RESET_N_IRQ_TYPE "NONE"
#define MIPI_RESET_N_NAME "/dev/mipi_reset_n"
#define MIPI_RESET_N_RESET_VALUE 0
#define MIPI_RESET_N_SPAN 16
#define MIPI_RESET_N_TYPE "altera_avalon_pio"


/*
 * onchip_memory configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory intel_onchip_memory
#define ONCHIP_MEMORY_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY_BASE 0x0
#define ONCHIP_MEMORY_CONTENTS_INFO ""
#define ONCHIP_MEMORY_DUAL_PORT 0
#define ONCHIP_MEMORY_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_INIT_CONTENTS_FILE "system_intel_onchip_memory_system_intel_onchip_memory"
#define ONCHIP_MEMORY_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY_IRQ -1
#define ONCHIP_MEMORY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY_NAME "/dev/onchip_memory"
#define ONCHIP_MEMORY_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY_SIZE_VALUE 200000
#define ONCHIP_MEMORY_SPAN 200000
#define ONCHIP_MEMORY_TYPE "intel_onchip_memory"
#define ONCHIP_MEMORY_WRITABLE 1


/*
 * one_ai_interface_0 configuration
 *
 */

#define ALT_MODULE_CLASS_one_ai_interface_0 one_ai_interface
#define ONE_AI_INTERFACE_0_BASE 0x50000
#define ONE_AI_INTERFACE_0_IRQ -1
#define ONE_AI_INTERFACE_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONE_AI_INTERFACE_0_NAME "/dev/one_ai_interface_0"
#define ONE_AI_INTERFACE_0_SPAN 1024
#define ONE_AI_INTERFACE_0_TYPE "one_ai_interface"


/*
 * sw configuration
 *
 */

#define ALT_MODULE_CLASS_sw altera_avalon_pio
#define SW_BASE 0x50950
#define SW_BIT_CLEARING_EDGE_REGISTER 0
#define SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SW_CAPTURE 0
#define SW_DATA_WIDTH 2
#define SW_DO_TEST_BENCH_WIRING 0
#define SW_DRIVEN_SIM_VALUE 0
#define SW_EDGE_TYPE "NONE"
#define SW_FREQ 100000000
#define SW_HAS_IN 1
#define SW_HAS_OUT 0
#define SW_HAS_TRI 0
#define SW_IRQ -1
#define SW_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SW_IRQ_TYPE "NONE"
#define SW_NAME "/dev/sw"
#define SW_RESET_VALUE 0
#define SW_SPAN 16
#define SW_TYPE "altera_avalon_pio"


/*
 * sysid_qsys configuration
 *
 */

#define ALT_MODULE_CLASS_sysid_qsys altera_avalon_sysid_qsys
#define SYSID_QSYS_BASE 0x50970
#define SYSID_QSYS_ID 0
#define SYSID_QSYS_IRQ -1
#define SYSID_QSYS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_QSYS_NAME "/dev/sysid_qsys"
#define SYSID_QSYS_SPAN 8
#define SYSID_QSYS_TIMESTAMP 0
#define SYSID_QSYS_TYPE "altera_avalon_sysid_qsys"


/*
 * terasic_auto_focus_0 configuration
 *
 */

#define ALT_MODULE_CLASS_terasic_auto_focus_0 terasic_auto_focus
#define TERASIC_AUTO_FOCUS_0_BASE 0x508c0
#define TERASIC_AUTO_FOCUS_0_IRQ -1
#define TERASIC_AUTO_FOCUS_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TERASIC_AUTO_FOCUS_0_NAME "/dev/terasic_auto_focus_0"
#define TERASIC_AUTO_FOCUS_0_SPAN 32
#define TERASIC_AUTO_FOCUS_0_TYPE "terasic_auto_focus"


/*
 * timer configuration
 *
 */

#define ALT_MODULE_CLASS_timer altera_avalon_timer
#define TIMER_ALWAYS_RUN 0
#define TIMER_BASE 0x50900
#define TIMER_COUNTER_SIZE 32
#define TIMER_FIXED_PERIOD 0
#define TIMER_FREQ 100000000
#define TIMER_IRQ 1
#define TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_LOAD_VALUE 99999
#define TIMER_MULT 0.001
#define TIMER_NAME "/dev/timer"
#define TIMER_PERIOD 1
#define TIMER_PERIOD_UNITS "ms"
#define TIMER_RESET_OUTPUT 0
#define TIMER_SNAPSHOT 1
#define TIMER_SPAN 32
#define TIMER_TICKS_PER_SEC 1000
#define TIMER_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TIMER_DEVICE_TYPE 1
#define TIMER_TYPE "altera_avalon_timer"


/*
 * vid_buff configuration
 *
 */

#define ALT_MODULE_CLASS_vid_buff intel_onchip_memory
#define VID_BUFF_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define VID_BUFF_BASE 0x60000
#define VID_BUFF_CONTENTS_INFO ""
#define VID_BUFF_DUAL_PORT 0
#define VID_BUFF_GUI_RAM_BLOCK_TYPE "AUTO"
#define VID_BUFF_INIT_CONTENTS_FILE "vid_buff_vid_buff"
#define VID_BUFF_INIT_MEM_CONTENT 1
#define VID_BUFF_INSTANCE_ID "NONE"
#define VID_BUFF_IRQ -1
#define VID_BUFF_IRQ_INTERRUPT_CONTROLLER_ID -1
#define VID_BUFF_NAME "/dev/vid_buff"
#define VID_BUFF_NON_DEFAULT_INIT_FILE_ENABLED 0
#define VID_BUFF_RAM_BLOCK_TYPE "AUTO"
#define VID_BUFF_READ_DURING_WRITE_MODE "DONT_CARE"
#define VID_BUFF_SINGLE_CLOCK_OP 0
#define VID_BUFF_SIZE_MULTIPLE 1
#define VID_BUFF_SIZE_VALUE 102400
#define VID_BUFF_SPAN 102400
#define VID_BUFF_TYPE "intel_onchip_memory"
#define VID_BUFF_WRITABLE 1


/*
 * vvp_scaler_0 configuration
 *
 */

#define ALT_MODULE_CLASS_vvp_scaler_0 intel_vvp_scaler
#define VVP_SCALER_0_BASE 0x50600
#define VVP_SCALER_0_IRQ -1
#define VVP_SCALER_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define VVP_SCALER_0_NAME "/dev/vvp_scaler_0"
#define VVP_SCALER_0_SPAN 512
#define VVP_SCALER_0_TYPE "intel_vvp_scaler"


/*
 * vvp_vfw_0 configuration
 *
 */

#define ALT_MODULE_CLASS_vvp_vfw_0 intel_vvp_vfw
#define VVP_VFW_0_BASE 0x50400
#define VVP_VFW_0_IRQ 5
#define VVP_VFW_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define VVP_VFW_0_NAME "/dev/vvp_vfw_0"
#define VVP_VFW_0_SPAN 512
#define VVP_VFW_0_TYPE "intel_vvp_vfw"

#endif /* __SYSTEM_H_ */

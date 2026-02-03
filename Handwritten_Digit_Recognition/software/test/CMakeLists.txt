cmake_minimum_required(VERSION 3.14)

add_subdirectory(../test_bsp test_bsp)

include(../test_bsp/toolchain.cmake)

project(test)

enable_language(ASM)
enable_language(C)
enable_language(CXX)

add_executable(test.elf)

target_sources(test.elf
    PRIVATE
        main.cpp
        auto_focus.cpp
        I2C.cpp
        mipi_bridge_config.cpp
        mipi_camera_config.cpp
        queue.cpp
)

target_include_directories(test.elf
    PRIVATE
    PUBLIC
)

target_link_libraries(test.elf
    PRIVATE
        -T "${BspLinkerScript}" -nostdlib
        "${ExtraArchiveLibraries}"
        -Wl,--start-group "${BspLibraryName}" -lc -lstdc++ -lgcc -lm -Wl,--end-group
)

# Create objdump from ELF.
set(objdump test.elf.objdump)
add_custom_command(
    OUTPUT "${objdump}"
    DEPENDS test.elf
    COMMAND "${ToolchainObjdump}" "${ToolchainObjdumpFlags}" test.elf >
            "${objdump}"
    COMMENT "Creating ${objdump}."
    VERBATIM
)
add_custom_target(create-objdump ALL DEPENDS "${objdump}")

# Report space free for stack + heap. Note that the file below is never created
# so the report is always output on build.
set(stack_report_file test.elf.stack_report)
add_custom_command(
    OUTPUT "${stack_report_file}"
    DEPENDS test.elf
    COMMAND "D:/intelFPGA/25.1pro/niosv/bin/niosv-stack-report" -p "${ToolchainPrefix}" test.elf
    COMMENT "Reporting memory available for stack + heap in test.elf."
    VERBATIM
)
add_custom_target(niosv-stack-report ALL DEPENDS "${stack_report_file}")

# Generate HEX file(s) from test.elf using elf2hex tool.
# Note : If ECC Full is enabled, width of 39 is set for NiosV TCM. Otherwise, 32.
add_custom_command(
    OUTPUT "intel_onchip_memory.hex"
    DEPENDS test.elf
    COMMAND "D:/intelFPGA/25.1pro/niosv/bin/elf2hex" test.elf -o intel_onchip_memory.hex -b 0x00000000 -w 32 -e 0x0007CFFF -r 4
    COMMENT "Creating intel_onchip_memory.hex."
    VERBATIM
)
add_custom_target(create-hex ALL DEPENDS "intel_onchip_memory.hex")

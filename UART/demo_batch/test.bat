@echo off
echo FPGA Configure...

set project_sof=golden_top.sof

REM ########################################################################
REM # Download sof file
REM ########################################################################
@ set QUARTUS_BIN=%QUARTUS_ROOTDIR%\bin
@ if not exist "%QUARTUS_BIN%" set QUARTUS_BIN=%QUARTUS_ROOTDIR%\bin64

%QUARTUS_BIN%\quartus_pgm.exe -m jtag -c 1 -o "p;%project_sof%"

pause



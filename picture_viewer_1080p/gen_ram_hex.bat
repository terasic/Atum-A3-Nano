@ set QUARTUS_BIN=%QUARTUS_ROOTDIR%\bin
@ if not exist "%QUARTUS_BIN%" set QUARTUS_BIN=%QUARTUS_ROOTDIR%\bin64

%QUARTUS_ROOTDIR%\..\niosv\bin\elf2hex.exe software/nios_app/build/Debug/nios_app.elf -o ram.hex -b 0x00100000 -w 32 -e 0x0017fff7
pause
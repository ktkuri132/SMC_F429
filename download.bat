@echo off
setlocal

REM 设置OpenOCD路径
set OPENOCD_PATH=C:\Program Files (x86)\xpack-openocd-0.12.0-6\bin

REM 设置OpenOCD配置文件路径
set INTERFACE=C:\Program Files (x86)\xpack-openocd-0.12.0-6\openocd\scripts\interface
set TARGET=C:\Program Files (x86)\xpack-openocd-0.12.0-6\openocd\scripts\target
REM 设置要烧录的ELF文件路径
set ELF_FILE=.\\build\\SMC.elf

REM 检查ELF文件是否存在
if not exist "%ELF_FILE%" (
    echo 错误: 找不到ELF文件 %ELF_FILE%
    exit /b 1
)

REM 运行OpenOCD并烧录ELF文件
"%OPENOCD_PATH%\openocd.exe" -f "%INTERFACE%\cmsis-dap.cfg" -f "%TARGET%\stm32f4x.cfg" -c "program %ELF_FILE% verify reset exit"

if %errorlevel% neq 0 (
    echo failed
    exit /b 1
)

echo success
exit /b 0
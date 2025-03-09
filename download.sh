#!/bin/bash

# 设置 OpenOCD 配置文件路径
OPENOCD_CFG="./Config/cmsis_dap.cfg"

# 设置要烧录的二进制文件路径
BIN_FILE="build/SMC.elf"

# 运行 OpenOCD 烧录命令
openocd -f $OPENOCD_CFG -c "program $BIN_FILE verify reset exit"


#翻译编辑不易---> https://github.com/ktkuri132

#这个东西，看清楚了在调整添加，不要看都没看就开始乱改，改错了死都编译不过去，到时候又说这东西不行

# 芯片类型（按需选择）
CPU = -mcpu=cortex-m4

# 选择软/硬件浮点版本（这个不用管）
FPU = -mfpu=fpv4-sp-d16

# 浮点运算类型（按需选择）
FLOAT-ABI = -mfloat-abi=hard

# 定义生成的二进制文件的名字
TARGET = temp

# 是否支持调试，支持调试则编译产生文件大小会更大，建议在最终定夺的版本中关闭调试
DEBUG = 1
# 选择优化选项，有些稀奇的bug就从这里产生，需按情况调节
OPT = -Og

#编译产生的中间文件放哪里
BUILD_DIR = build

# 链接脚本的路径，就是xxx.ld文件
LDSCRIPT = CORE/STM32F429IGTx_FLASH.ld

# 汇编启动文件路径，若使用汇编作为启动文件，则需加入，使用其他C文件最为启动文件则不需
# ASM_SOURCES =  CORE/startup_stm32f429_439xx.s


# 其他的汇编源文件，没有就不管
ASMM_SOURCES = 



# C 语言的宏定义，在编译器编译时，直接在这里定义的宏对整个项目起效
#C_DEFS =  -DUSE_STDPERIPH_DRIVER 
#C_DEFS += -DSTM32F429_439xx


# C 源文件路径，如果暂时没有这个路径也无需删除，编译器自己会忽略的
C_SOURCES = $(wildcard USER/*.c)

C_SOURCES += $(wildcard CORE/*.c)

C_SOURCES += $(wildcard BSP/*.c)
C_SOURCES += $(wildcard BSP/bootload/*.c)
C_SOURCES += $(wildcard BSP/gpio/*.c)
C_SOURCES += $(wildcard BSP/systick/*.c)
C_SOURCES += $(wildcard BSP/usart/*.c)
C_SOURCES += $(wildcard BSP/i2c/*.c)
C_SOURCES += $(wildcard BSP/spi/*.c)

C_SOURCES += $(wildcard Project/BSP/PWM/*.c)
C_SOURCES += $(wildcard Project/BSP/TB6612/*.c)
C_SOURCES += $(wildcard Project/BSP/HW_201/*.c)
C_SOURCES += $(wildcard Project/BSP/LED/*.c)
C_SOURCES += $(wildcard Project/BSP/ADC/*.c)
C_SOURCES += $(wildcard Project/LIB/Control/*.c)
C_SOURCES += $(wildcard Project/LIB/PID/*.c)
C_SOURCES += $(wildcard Project/LIB/Tools/*.c)

C_SOURCES += $(wildcard HARDWARE/OLED/*.c)
C_SOURCES += $(wildcard HARDWARE/MPU6050/*.c)
C_SOURCES += $(wildcard HARDWARE/LCD/*.c)
C_SOURCES += $(wildcard HARDWARE/LCD/TOUCH/*.c)
C_SOURCES += $(wildcard HARDWARE/LCD/GUI/*.c)
C_SOURCES += $(wildcard HARDWARE/SDRAM/*.c)

C_SOURCES += $(wildcard RTOS/*.c)
C_SOURCES += $(wildcard RTOS/portable/ARM_CM4F/*.c)
C_SOURCES += $(wildcard RTOS/portable/MemMang/*.c)
#C_SOURCES += $(wildcard SYSTEM/delay/*.c)
#C_SOURCES += $(wildcard SYSTEM/sys/*.c)
#C_SOURCES += $(wildcard SYSTEM/usart/*.c)

#C_SOURCES += $(wildcard HADRWARE/LCD/*.c)
#C_SOURCES += $(wildcard HADRWARE/SDRAM/*.c)
#C_SOURCES += $(wildcard HADRWARE/TOUCH/*.c)
#C_SOURCES += $(wildcard HADRWARE/JY61_USART/*.c)

#C_SOURCES += $(wildcard Lib/src/*.c)

# C++ 源文件路径
CXX_SOURCES = $(wildcard HARDWARE/LCD/GUI/*.cpp)
CXX_SOURCES += $(wildcard Project/LIB/Control/*.cpp)
CXX_SOURCES += $(wildcard CORE/*.cpp)
CXX_SOURCES += $(wildcard USER/*.cpp)

# C 头文件路径
C_INCLUDES =  -ICORE/
C_INCLUDES += -IUSER/

C_INCLUDES += -IBSP/
C_INCLUDES += -IBSP/bootload/
C_INCLUDES += -IBSP/gpio/
C_INCLUDES += -IBSP/systick/
C_INCLUDES += -IBSP/usart/
C_INCLUDES += -IBSP/i2c/
C_INCLUDES += -IBSP/spi/

C_INCLUDES += -IProject/BSP/
C_INCLUDES += -IProject/BSP/PWM/
C_INCLUDES += -IProject/BSP/TB6612/
C_INCLUDES += -IProject/BSP/HW_201/
C_INCLUDES += -IProject/BSP/LED/
C_INCLUDES += -IProject/BSP/ADC/
C_INCLUDES += -IProject/LIB/Control/
C_INCLUDES += -IProject/LIB/PID/
C_INCLUDES += -IProject/LIB/Tools/
C_INCLUDES += -IProject/

C_INCLUDES += -IHARDWARE/OLED/
C_INCLUDES += -IHARDWARE/MPU6050/
C_INCLUDES += -IHARDWARE/LCD/
C_INCLUDES += -IHARDWARE/LCD/TOUCH/
C_INCLUDES += -IHARDWARE/LCD/GUI/
C_INCLUDES += -IHARDWARE/SDRAM/


C_INCLUDES += -IRTOS/
C_INCLUDES += -IRTOS/portable/ARM_CM4F/
C_INCLUDES += -IRTOS/include/

#C_INCLUDES += -ILibraries/CMSIS/include/
#C_INCLUDES += -ILibraries/STM32F4xx_StdPeriph_Driver/inc/

#C_INCLUDES += -ISYSTEM/delay/
#C_INCLUDES += -ISYSTEM/sys/
#C_INCLUDES += -ISYSTEM/usart/

#C_INCLUDES += -IHADRWARE/LCD/
#C_INCLUDES += -IHADRWARE/SDRAM/
#C_INCLUDES += -IHADRWARE/TOUCH/
#C_INCLUDES += -IHADRWARE/JY61_USART/

#C_INCLUDES += -ILib/inc/

# C++ 头文件路径
CXX_INCLUDES = -IHARDWARE/LCD/GUI/
CXX_INCLUDES += -IProject/LIB/Control/
CXX_INCLUDES += -IProject/


# 汇编头文件
# 汇编文件的宏定义（一般不需要管）
AS_DEFS = 



######################################################下边的是编译规则，若无特殊情况，千万不要随便瞎动#######################################

#工具链名称
PREFIX = arm-none-eabi-
#GCC工具链的环境变量一定要加入，在下载好ARM—GCC交叉编译工具链后，将bin目录加入到系统环境变量
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX= $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX= $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 

# mcu配置
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# 编译器编译选项


# 汇编编译选项
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
# c编译选项
CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
# C++编译选项
CXXFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CXXFLAGS += $(CFLAGS) $(CXX_INCLUDES)
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
CXXFLAGS += -g -gdwarf-2
endif


# 产生依赖信息
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# 库  
LIBS = -lc -lm -lnosys 
LIBDIR = 
# 这里可选择是否使用他的标准库，这里默认是不选
LDFLAGS = $(MCU) -specs=nosys.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# 默认编译所有选定的文件
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# 构建所需的依赖项
#######################################
# 目标C文件列表
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
#
# 目标C++文件列表
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))
#
# 目标汇编文件列表
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

# 开始编译C文件
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
# 开始编译C++文件
$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@
# 链接C文件和汇编文件
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
# 链接C++文件和汇编文件
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CXXFLAGS) $< -o $@
# 将两个链接文件最终链接成.elf文件
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
# 将.elf转译成.hex文件
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
# 将.elf转译成.bin文件
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
# 创建编译目录（如果没有提前创建）
$(BUILD_DIR):
	mkdir $@		

################################################################好，从这里截断，上面的不能随便动#######################################

##############################################下面的是烧录器选择和芯片选择，聪明的小伙伴可能已经知道选芯片和烧录器要改哪里了吧
clean:
	-rm -fR $(BUILD_DIR)/*.o
	-rm -fR $(BUILD_DIR)/*.d
	-rm -fR $(BUILD_DIR)/*.lst


dol:
	echo "this is a test function"  
#	温馨提示：改下面这句话       这里      和 				这里                       
	openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg -c init -c "reset halt; wait_halt; flash write_image erase ./${BUILD_DIR}/$(TARGET).bin 0x08000000" -c reset -c shutdown

rebuild_run:
	make clean
	make -j16 
	make dol
	arm-none-eabi-objdump -D -S -m cortex-m4 ./${BUILD_DIR}/$(TARGET).elf > ./${BUILD_DIR}/$(TARGET).dis
	echo "program is running......"
build_run:
	make -j16 
	make dol
	echo "program is running......"
#######################################
# 包含所有依赖项
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
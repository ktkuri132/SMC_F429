#参考CubeMX生成的CMakeLists.txt文件进行修改和添加注释,若有疑问,请联系作者邮箱3457532883@qq.com
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.28)

# 指定交叉编译工具链arm-none-eabi-gcc,添加到环境变量中,或者在此处直接指定绝对路径
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 项目名称SMC
project(SMC C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# 添加关于硬件浮点的支持的相关编译选项
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

# 若不支持硬件浮点,则使用软件浮点
#add_compile_options(-mfloat-abi=soft)

# 指定芯片型号的相关编译选项
add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# 此处忽略C++17中的操作绝对地址(寄存器)的警告
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# 进行汇编文件的预编译
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

# 提供编译选项,根据不同的编译类型进行不同的优化,常见的编译类型有Debug,Release,RelWithDebInfo,MinSizeRel
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()


# 添加子目录Cmake文件
add_subdirectory(BSP)
add_subdirectory(CORE)
add_subdirectory(HARDWARE)
add_subdirectory(Project)
add_subdirectory(RTOS)
add_subdirectory(USER)

# 添加链接脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/CORE/STM32F429IGTx_FLASH.ld)

# 添加链接选项,生成map文件
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

# 添加宏定义
# add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx)

# 生成可执行文件
add_executable(${PROJECT_NAME}.elf ${CMAKE_SOURCE_DIR}/USER/main.c ${LINKER_SCRIPT})

# 忽略warning
# target_compile_options(${PROJECT_NAME}.elf PRIVATE -W)

# 将可执行文件链接项目其他库
target_link_libraries(${PROJECT_NAME}.elf BSP_LIB CORE_LIB HARDWARE_LIB Project_LIB RTOS_LIB USER_LIB)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

# 添加头文件路径
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${ROOT_DIR})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${ROOT_DIR}/CORE)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${ROOT_DIR}/Project)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${ROOT_DIR}/BSP)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${ROOT_DIR}/HARDWARE)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${ROOT_DIR}/RTOS)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${ROOT_DIR}/USER)

# 定义其他类型的可执行文件
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
# 生成hex和bin文件
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
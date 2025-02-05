# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR}/CORE)
include_directories(${CMAKE_SOURCE_DIR}/BSP)
include_directories(${CMAKE_SOURCE_DIR}/RTOS)
include_directories(${CMAKE_SOURCE_DIR}/HARDWARE)
include_directories(${CMAKE_SOURCE_DIR}/USER)
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source
        Dev/ADC/*.c
        Dev/HW_201/*.c
        Dev/KEY/*.c
        Dev/LED/*.c
        Dev/PWM/*.c
        Dev/TB6612/*.c
        LIB/Control/*.c
        LIB/PID/*.c
        LIB/Tools/*.c
)


add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_LIB BSP_LIB CORE_LIB RTOS_LIB HARDWARE_LIB USER_LIB)

# 添加当前头文件路径
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})



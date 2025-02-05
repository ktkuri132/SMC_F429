include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/CORE)
include_directories(${CMAKE_SOURCE_DIR}/Project)
include_directories(${CMAKE_SOURCE_DIR}/HARDWARE)
include_directories(gpio i2c spi usart)
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source
        ./*.c
        gpio/*.c
        i2c/*.c
        spi/*.c
        usart/*.c
)

add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})


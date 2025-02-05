include_directories(${CMAKE_SOURCE_DIR}/BSP)
include_directories(${CMAKE_SOURCE_DIR}/CORE)

# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source
        ./*.c
        LCD/*.c
        LCD/GUI/*.c
#        LCD/GUI/*.cpp
        LCD/TOUCH/*.c
        MPU6050/*.c
        OLED/*.c
        SDRAM/*.c
)

# 生成库文件
add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_LIB BSP_LIB CORE_LIB)

target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LCD)



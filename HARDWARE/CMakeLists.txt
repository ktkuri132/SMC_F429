include_directories(${CMAKE_SOURCE_DIR}/BSP)
include_directories(${CMAKE_SOURCE_DIR}/CORE)

# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_subdirectory(LCD)
add_subdirectory(MPU6050)
add_subdirectory(OLED)
add_subdirectory(SDRAM)

# 生成库文件
add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_LIB LCD_LIB MPU6050_LIB OLED_LIB SDRAM_LIB)

target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LCD)



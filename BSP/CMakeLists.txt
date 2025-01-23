INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/CORE)
include_directories(${CMAKE_SOURCE_DIR}/Project)
include_directories(${CMAKE_SOURCE_DIR}/HARDWARE)
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_subdirectory(gpio)
add_subdirectory(i2c)
add_subdirectory(spi)
add_subdirectory(usart)

add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_LIB gpio_LIB i2c_LIB spi_LIB usart_LIB)

target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gpio)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/i2c)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/spi)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/usart)

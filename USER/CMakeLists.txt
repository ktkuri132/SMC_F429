
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ./*.c)
# 去除main.c文件
#list(REMOVE_ITEM ${DIR_NAME}_Source ../main.c)

# 生成库文件
add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})
target_link_libraries(${DIR_NAME}_LIB BSP_LIB CORE_LIB HARDWARE_LIB)

target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_SOURCE_DIR}/CORE)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_SOURCE_DIR}/BSP)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_SOURCE_DIR}/HARDWARE)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_SOURCE_DIR}/Project)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_SOURCE_DIR}/RTOS/include)
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
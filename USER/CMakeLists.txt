include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
# 去除main.c文件
list(REMOVE_ITEM ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

# 生成库文件
add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_LIB Project_BSP_LIB Project_LIB_LIB)
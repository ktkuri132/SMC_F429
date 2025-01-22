include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_subdirectory(Control)
add_subdirectory(PID)
add_subdirectory(Tools)

add_library(Project_${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(Project_${DIR_NAME}_LIB Control_LIB PID_LIB Tools_LIB)


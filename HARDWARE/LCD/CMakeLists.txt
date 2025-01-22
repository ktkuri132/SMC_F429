include_directories(${CMAKE_SOURCE_DIR}/BSP)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_subdirectory(GUI)
add_subdirectory(TOUCH)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(${DIR_NAME}_lib ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_lib GUI_lib TOUCH_lib)
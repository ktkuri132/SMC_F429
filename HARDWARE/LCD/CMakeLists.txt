include_directories(${CMAKE_SOURCE_DIR}/BSP)
include_directories(${CMAKE_SOURCE_DIR}/CORE)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_subdirectory(GUI)
add_subdirectory(TOUCH)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_LIB GUI_LIB TOUCH_LIB)

target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


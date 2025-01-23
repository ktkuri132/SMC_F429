# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR}/CORE)
include_directories(${CMAKE_SOURCE_DIR}/BSP)
include_directories(${CMAKE_SOURCE_DIR}/RTOS)
include_directories(${CMAKE_SOURCE_DIR}/HARDWARE)
include_directories(${CMAKE_SOURCE_DIR}/USER)
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_subdirectory(Dev)
add_subdirectory(LIB)

add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(${DIR_NAME}_LIB Project_Dev_LIB Project_LIB_LIB)

# 添加当前头文件路径
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${DIR_NAME}_LIB PRIVATE ${CMAKE_SOURCE_DIR}/RTOS)



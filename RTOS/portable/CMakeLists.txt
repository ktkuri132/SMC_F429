
include_directories(../include)
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c 
                                        ${CMAKE_CURRENT_SOURCE_DIR}/ARM_CM4F/*.c 
                                        ${CMAKE_CURRENT_SOURCE_DIR}/MemMang/*.c)

add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})


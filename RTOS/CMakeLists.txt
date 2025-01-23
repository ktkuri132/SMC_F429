include_directories(${CMAKE_SOURCE_DIR}/CORE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/comment)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/portable)
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
# 添加子目录
add_subdirectory(portable)
add_subdirectory(comment)
# 生成库文件
add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})
# 链接库文件
target_link_libraries(${DIR_NAME}_LIB portable_LIB comment_LIB)



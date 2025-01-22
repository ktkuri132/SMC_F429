
# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加源文件
file(GLOB_RECURSE ${DIR_NAME}_Source ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_subdirectory(ADC)
add_subdirectory(HW_201)
add_subdirectory(KEY)
add_subdirectory(LED)
add_subdirectory(PWM)
add_subdirectory(TB6612)

add_library(Project_${DIR_NAME}_LIB ${${DIR_NAME}_Source})

target_link_libraries(Project_${DIR_NAME}_LIB ADC_LIB HW_201_LIB KEY_LIB LED_LIB PWM_LIB TB6612_LIB)


include_directories(${CMAKE_SOURCE_DIR}/BSP)

set(
    BSP_Source
    ${CMAKE_SOURCE_DIR}/BSP/*.c
)

add_subdirectory(usart)

add_library(BSP_LIB STRAT ${BSP_Source} ${usart_LIB})
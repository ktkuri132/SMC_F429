include_directories(${CMAKE_SOURCE_DIR}/BSP/usart)

set(
    usart_Source
    ${CMAKE_SOURCE_DIR}/BSP/usart/*.c
)

add_library(usart_LIB STRAT ${usart_Source})
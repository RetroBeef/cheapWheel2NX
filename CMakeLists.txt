cmake_minimum_required(VERSION 3.13)
include (pico_sdk_import.cmake)
project(pio_usb)

pico_sdk_init()

set(PICO_PIO_USB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/")
add_subdirectory(${PICO_PIO_USB_DIR} pico_pio_usb)

add_subdirectory(src)

cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-firmware C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

#common for both single keyboards and left/right splits
add_library(pico-firmware-common INTERFACE)
target_sources(pico-firmware-common INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyboardState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/InputEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/InputEventPool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wire/WireHandler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wire/TinyUSBWireHandler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin/KeyPlugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin/PicoRebootPlugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin/FnPlugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/input/InputDevice.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/input/MatrixInput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_descriptors.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wire/I2CWireHandler.cpp
)
target_link_libraries(pico-firmware-common INTERFACE 
        pico_stdlib tinyusb_device tinyusb_board hardware_i2c
)
target_include_directories(pico-firmware-common INTERFACE
        ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src   
)

target_compile_options(pico-firmware-common INTERFACE -DPICO_ENTER_USB_BOOT_ON_EXIT=1)
target_compile_definitions(pico-firmware-common INTERFACE PICO)

add_executable(pico-firmware-left)
target_link_libraries(pico-firmware-left pico-firmware-common)
target_compile_definitions(pico-firmware-left PRIVATE LEFT_HAND_SIDE)
target_compile_definitions(pico-firmware-left PRIVATE PRIMARY)
pico_enable_stdio_uart(pico-firmware-left 0)
pico_enable_stdio_usb(pico-firmware-left 1)
pico_add_extra_outputs(pico-firmware-left)


add_executable(pico-firmware-right)
target_link_libraries(pico-firmware-right pico-firmware-common)
target_compile_definitions(pico-firmware-right PRIVATE RIGHT_HAND_SIDE)
target_compile_definitions(pico-firmware-right PRIVATE PRIMARY)
pico_enable_stdio_uart(pico-firmware-right 0)
pico_enable_stdio_usb(pico-firmware-right 1)
pico_add_extra_outputs(pico-firmware-right)

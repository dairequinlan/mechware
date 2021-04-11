cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-firmware C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Initialize the SDK
pico_sdk_init()

add_executable(pico-firmware
        src/main.cpp
        src/KeyboardState.cpp
        src/InputEvent.cpp
        src/InputEventPool.cpp
        src/wire/WireHandler.cpp
        src/wire/TinyUSBWireHandler.cpp
        src/plugin/KeyPlugin.cpp
        src/plugin/PicoRebootPlugin.cpp
        src/input/InputDevice.cpp
        src/input/MatrixInput.cpp
        src/usb_descriptors.c
        )

# Pull in our pico_stdlib which pulls in commonly used features

#we want this /src include addition here so that tinyusb can pick up 
#the tusb_config.h file, otherwise throws an error
target_include_directories(pico-firmware PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(pico-firmware PRIVATE pico_stdlib tinyusb_device tinyusb_board)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico-firmware 1)
pico_enable_stdio_uart(pico-firmware 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico-firmware)


add_definitions(-DPICO_ENTER_USB_BOOT_ON_EXIT=1)
add_compile_definitions(PICO)
add_compile_options(-CFG_TUSB_CONFIG_FILE=src/tusb_config.h)

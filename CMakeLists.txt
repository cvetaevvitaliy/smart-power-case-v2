SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

####################################################################################################
# Software version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

set(VERSION_FW "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
add_definitions(-DMINOR=\"${VERSION_MAJOR}\")
add_definitions(-DMAJOR=\"${VERSION_MINOR}\")
add_definitions(-DPATCH=\"${VERSION_PATCH}\")

####################################################################################################
# Get git hash ang git branch name
find_package(Git)
if(GIT_FOUND)
    message("git found: ${GIT_EXECUTABLE} in version ${GIT_VERSION_STRING}")
    # get git branch name
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD COMMAND cut -c1-10 OUTPUT_VARIABLE GIT_BRANCH WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    string(STRIP "${GIT_BRANCH}" GIT_BRANCH)

    # get git hash
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short=8 HEAD OUTPUT_VARIABLE GIT_HASH WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    string(STRIP "${GIT_HASH}" GIT_HASH)

else()
   set(GIT_HASH "000000")
   set(GIT_BRANCH "none")
endif()

message(STATUS "Git branch - ${GIT_BRANCH}")
add_definitions(-DGIT_BRANCH=\"${GIT_BRANCH}\")
message(STATUS "Git hash - ${GIT_HASH}")
add_definitions(-DGIT_HASH=\"${GIT_HASH}\")

####################################################################################################

# specify cross compilers and tools
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/stm32/ld_script/STM32F411CEUx_FLASH.ld)

# Hardware floating point
SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")

SET(COMMON_FLAGS
        "-mcpu=cortex-m4 ${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections \
    -g -O2 -fno-common -fmessage-length=0 -specs=nosys.specs -specs=rdimon.specs")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections,--print-memory-usage -T ${LINKER_SCRIPT}")

PROJECT(smart-power-case-v2 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)

add_definitions(-D__weak=__attribute__\(\(weak\)\) -D__packed=__attribute__\(\(__packed__\)\) -DUSE_HAL_DRIVER -DSTM32F411xE)
add_definitions(-DMCU=\"STM32F411CEU6\")

#add_subdirectory(shell)
#set(INC_SHELL ${CMAKE_CURRENT_SOURCE_DIR}/shell ${CMAKE_CURRENT_SOURCE_DIR}/shell/shell)

###########################################################################################

# FreeRTOS sources
set(FreeRTOS_SRC
        "src/components/FreeRTOS/*.*"
        )

set(FreeRTOS_INC
        src/components/FreeRTOS/Source/CMSIS_RTOS
        src/components/FreeRTOS/Source/include
        src/components/FreeRTOS/Source/portable/GCC/ARM_CM4F
        )

# Source files
file(GLOB_RECURSE SOURCES
        "src/stm32/startup/startup_stm32f411xe.s"
        ${FreeRTOS_SRC}

        "src/stm32/STM32F4xx_HAL_Driver/Src/*.c"
        "src/stm32/STM32_USB_Device_Library/Class/CDC/Src/*.c"
        "src/stm32/STM32_USB_Device_Library/Core/Src/*.*"
        "src/device/stm32_hal_config/stm32f4xx_hal_timebase_TIM.c"

        "src/device/stm32_hal_config/system_stm32f4xx.c"
        "src/device/stm32_hal_config/stm32f4xx_hal_msp.c"
        "src/device/stm32_hal_config/stm32f4xx_it.c"

        "src/components/delay/delay.c"

        "src/device/usbd/usb_device.c"
        "src/device/usbd/usbd_desc.c"
        "src/device/usbd/usbd_cdc_if.c"
        "src/device/usbd/usbd_conf.c"
        "src/device/usbd/usbd_desc.c"

        "src/device/dfu/dfu.c"
        "src/device/eeprom/eeprom.c"

        "src/device/stm32_init/stm32_init.c"
        "src/device/stm32_init/charger.c"
        "src/components/bq25895/bq2589x_charger.c"

        "src/components/bq27441/bq27441.c"

        "src/components/cli/lib/cli_input.c"
        "src/components/cli/lib/cli_log.c"
        "src/components/cli/lib/cli_queue.c"
        "src/components/cli/lib/cli_time.c"
        "src/components/cli/cli.c"
        "src/components/cli/cli_io.c"
        "src/components/cli/ulog/ulog.c"
        "src/components/tinyprintf/tinyprintf.c"
        "src/components/tinystring/tinystring.c"

        "src/device/battery/battery.c"
        "src/device/button/button.c"
        "src/device/display/display.c"
        "src/device/power/power.c"
        "src/device/hardware.c"

        "src/components/st7735s/st7735s.c"
        "src/components/st7735s/fonts/*.*"
        "src/components/st7735s/picts/*.*"

        "src/device/display/lv_demo_keypad_encoder.c"
        "src/device/gui/*.c"
        "src/device/gui/icon/*.c"
        "src/device/gui/screens/*.c"

        "src/device/imu/*.c"

        "src/main.c")


# Include dirs
include_directories(
        ${FreeRTOS_INC}
        src/stm32/CMSIS/Include
        src/stm32/CMSIS/Device/ST/STM32F4xx/Include
        src/stm32/STM32_USB_Device_Library/Core/Inc
        src/stm32/STM32_USB_Device_Library/Class/CDC/Inc
        src/stm32/STM32F4xx_HAL_Driver/Inc
        src/stm32/STM32F4xx_HAL_Driver/Inc/Legacy

        src/components/delay

        src/device/stm32_hal_config
        src/device/usbd

        src/device/dfu
        src/device/eeprom

        src/components/tinystring
        src/components/tinyprintf
        src/components/cli/lib
        src/components/cli/ulog
        src/components/cli

        src/device/stm32_init

        src/components/bq25895
        src/components/bq27441

        src/device/battery
        src/device/button
        src/device/display
        src/device/power
        src/device
        src/device/gui
        src/device/gui/icon
        src/device/gui/screens

        src/device/imu

        src/components/st7735s
        src/components/st7735s/fonts
        src/components/st7735s/picts

        src)

###########################################################################################


add_definitions(-DBUILD_NAME=\"Smart\ power\ V2\")
add_definitions(-DCLI -DDEBUG)

#_WHERE_BUILD
if (_WHERE_BUILD STREQUAL Cloud)
    add_definitions(-D_WHERE_BUILD=\"Cloud\ build\")
else()
    add_definitions(-D_WHERE_BUILD=\"Local\ build\")
endif()



add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

add_subdirectory(src/components/lis3dh)
include_directories(src/components/lis3dh)

add_subdirectory(src/components/gui/lvgl)
include_directories(src/components/gui
        src/components/gui/lvgl
        src/components/gui/lvgl/src
        src/components/gui/lvgl/src/lv_core
        src/components/gui/lvgl/src/lv_draw
        src/components/gui/lvgl/src/lv_font
        src/components/gui/lvgl/src/lv_gpu
        src/components/gui/lvgl/src/lv_hal
        src/components/gui/lvgl/src/lv_misc
        src/components/gui/lvgl/src/lv_themes
        src/components/gui/lvgl/src/lv_widgets
        )
target_link_libraries(${PROJECT_NAME}.elf lib_lis3dh lvgl)

set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}-V${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${GIT_HASH}.map" ${FPU_FLAGS})

SET(HEX_FILE ${PROJECT_BINARY_DIR}/firmware-v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${GIT_HASH}.hex)
SET(BIN_FILE ${PROJECT_BINARY_DIR}/firmware-v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${GIT_HASH}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND arm-none-eabi-size --format=berkeley ${PROJECT_NAME}.elf
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building done, output files:
${HEX_FILE}
${BIN_FILE}
File details, format berkeley:")

add_custom_target(flash
        COMMAND arm-none-eabi-size --format=berkeley ${PROJECT_NAME}.elf
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND dfu-util -a 0 -s 0x08000000:leave -D ${BIN_FILE} -w)

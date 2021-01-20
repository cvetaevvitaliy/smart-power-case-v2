SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

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

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/stm32_device_library/ld_script/STM32F411CEUx_FLASH.ld)

# Hardware floating point
SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")

SET(COMMON_FLAGS
        "-mcpu=cortex-m4 ${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections \
    -g -fno-common -fmessage-length=0 -specs=nosys.specs -specs=rdimon.specs")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections,--print-memory-usage -T ${LINKER_SCRIPT}")

PROJECT(smart-power-case-v2 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)

add_definitions(-D__weak=__attribute__\(\(weak\)\) -D__packed=__attribute__\(\(__packed__\)\) -DUSE_HAL_DRIVER -DSTM32F411xE)
add_definitions(-DMCU=\"STM32F411CEU6\")

###########################################################################################
# Source files
file(GLOB_RECURSE SOURCES_HAL
        "src/stm32_device_library/startup/startup_stm32f411xe.s"

        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c"

        "src/stm32_device_library/STM32F4xx_HAL_Driver/Src/*.c"
        "src/stm32_device_library/STM32_USB_Device_Library/Class/CDC/Src/*.c"
        "src/stm32_device_library/STM32_USB_Device_Library/Core/Src/*.*"

        "src/device/hal_config/system_stm32f4xx.c"
        "src/device/hal_config/stm32f4xx_hal_msp.c"
        "src/device/hal_config/stm32f4xx_it.c"

        "src/device/usbd/usb_device.c"
        "src/device/usbd/usbd_desc.c"
        "src/device/usbd/usbd_cdc_if.c"
        "src/device/usbd/usbd_conf.c"
        "src/device/usbd/usbd_desc.c"

        "src/device/init/stm32_init.c"

        "src/main.c")


# Include dirs
include_directories(
        src/stm32_device_library/CMSIS/Include
        src/stm32_device_library/CMSIS/Device/ST/STM32F4xx/Include
        src/stm32_device_library/STM32_USB_Device_Library/Core/Inc
        src/stm32_device_library/STM32_USB_Device_Library/Class/CDC/Inc
        src/stm32_device_library/STM32F4xx_HAL_Driver/Inc
        src/stm32_device_library/STM32F4xx_HAL_Driver/Inc/Legacy

        src/device/hal_config
        src/device/usbd

        src/device/init

        src)

###########################################################################################


add_definitions(-DBUILD_NAME=\"Smart\ power\ V2\")

#_WHERE_BUILD
if (_WHERE_BUILD STREQUAL Cloud)
    add_definitions(-D_WHERE_BUILD=\"Cloud\ build\")
else()
    add_definitions(-D_WHERE_BUILD=\"Local\ build\")
endif()



add_executable(${PROJECT_NAME}.elf ${SOURCES_HAL} ${LINKER_SCRIPT})

set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map" ${FPU_FLAGS})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

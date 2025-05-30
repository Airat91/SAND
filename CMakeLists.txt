cmake_minimum_required(VERSION 3.6)
set(PRJ_NAME SAND)
set(RELEASE 0)
set(BOARD_VER 0)
include(STM32Toolchain.cmake)
set(DEVICE_TYPE ${SAND_AI}) #select DEFPCB, SAND_AI for firmware compilation

set(MCU_FAMILY STM32F1xx)
set(MCU_LINE STM32F103xx)
set(MCU_TARGET STM32F103c8t6)
add_definitions(-DSTM32F103xB)

project(${PRJ_NAME} C CXX ASM)
if(${RELEASE})
    add_definitions(-DRELEASE_FLAG=1)
else()
    add_definitions(-DRELEASE_FLAG=0)
endif()
add_definitions(-DBOARD_VERSION=${BOARD_VER})
add_definitions(-DDEVICE_TYPE=${DEVICE_TYPE})

add_definitions(-D${MCU_LINE})
add_definitions(-DUSE_HAL_LIBRARY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(LIB_INSTALL_DIR)
else()
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
    set(LIB_INSTALL_DIR ${PROJECT_SOURCE_DIR}/lib)
endif()


#1.FreeRTOS source and headers files start
#check freertos
set(SOURCES "")
set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/freertos)
add_subdirectory(freertos)
set(FREERTOS_SOURCES ${SOURCES})
#headers
file(GLOB FREERTOS_HEADERS  freertos/inc/*.h)
file(GLOB FREERTOS_HEADERS  ${FREERTOS_HEADERS} freertos/inc/portable/*.h)
file(GLOB FREERTOS_HEADERS  ${FREERTOS_HEADERS} freertos/cmsis_os/*.h)
#freertos source and headers files END

#2.STM_hal
file(GLOB HAL_SOURCES Drivers/${MCU_FAMILY}_HAL_DRIVER/Src/*.c)
file(GLOB STM32F1_SYSTEM Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Source/Templates/system_${MCU_FAMILY}.c)
file(GLOB STM32F1_STARTUP Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Source/Templates/gcc/startup_stm32f103x6.s)

file(GLOB STM32_HEADERS Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include/*.h)
file(GLOB STM32_HEADERS ${STM32_HEADERS} Drivers/CMSIS/Include/*.h)
file(GLOB STM32_HEADERS ${STM32_HEADERS} Drivers/*.h)
file(GLOB STM32_HEADERS ${STM32_HEADERS} Drivers/${MCU_FAMILY}_HAL_Driver/Inc/*.h)
file(GLOB STM32_LEGACY_HEADERS Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy/*.h)
#stm_hal files END

#file(GLOB DCTS_SOURCES modules/DCTS/src/*.c)

file(GLOB USER_SOURCES src/*.c)
file(GLOB USB_CORE Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c)
file(GLOB USB_CLASS_CDC Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/*.c)
file(GLOB LINKER_FILE ${MCU_LINKER_SCRIPT})
file(GLOB SELF_HEADERS PUBLIC inc/*.h)

set(SOURCE_FILES  ${USB_CORE} ${USB_CLASS_CDC} ${USER_SOURCES} ${DCTS_SOURCES})

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(BUILD_TARGET ${PROJECT_NAME}.elf)

add_library(freertos_lib ${FREERTOS_SOURCES} ${FREERTOS_HEADERS} )
add_library(stm32f1_hal_lib ${HAL_SOURCES} ${STM32F1_STARTUP} ${STM32F1_SYSTEM} ${STM32_HEADERS} ${STM32_LEGACY_HEADERS} )
add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES} ${LINKER_FILE} ${FREERTOS_HEADERS} ${STM32_HEADERS} ${STM32_LEGACY_HEADERS} ${SELF_HEADERS})
target_link_libraries(${PROJECT_NAME}.elf freertos_lib stm32f1_hal_lib)

#freertos_lib use only self headers
target_include_directories(freertos_lib PUBLIC freertos/inc)
target_include_directories(freertos_lib PUBLIC freertos/inc/cmsis_os)
target_include_directories(freertos_lib PUBLIC freertos/inc/portable)
target_include_directories(freertos_lib PUBLIC Drivers/CMSIS/Include)

#stm32f1_hal_lib use only self headers
target_include_directories(stm32f1_hal_lib PUBLIC Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include)
target_include_directories(stm32f1_hal_lib PUBLIC Drivers/CMSIS/Include)
target_include_directories(stm32f1_hal_lib PUBLIC Drivers/${MCU_FAMILY}_HAL_Driver/Inc)
target_include_directories(stm32f1_hal_lib PUBLIC Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy)
target_include_directories(stm32f1_hal_lib PUBLIC Drivers)

#self 
target_include_directories(${PROJECT_NAME}.elf PUBLIC freertos/inc)
target_include_directories(${PROJECT_NAME}.elf PUBLIC freertos/cmsis_os)
target_include_directories(${PROJECT_NAME}.elf PUBLIC freertos/inc/portable)

target_include_directories(${PROJECT_NAME}.elf PUBLIC Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include)
target_include_directories(${PROJECT_NAME}.elf PUBLIC Drivers/CMSIS/Include)
target_include_directories(${PROJECT_NAME}.elf PUBLIC Drivers/${MCU_FAMILY}_HAL_Driver/Inc)
target_include_directories(${PROJECT_NAME}.elf PUBLIC inc)
target_include_directories(${PROJECT_NAME}.elf PUBLIC modules/DCTS/inc)

target_include_directories(${PROJECT_NAME}.elf PUBLIC Middlewares/ST/STM32_USB_Device_Library/Core/Inc)
target_include_directories(${PROJECT_NAME}.elf PUBLIC Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc)

#install headers
install(FILES ${FREERTOS_HEADERS} DESTINATION ${LIB_INSTALL_DIR}/freertos
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
install(FILES ${STM32_HEADERS} DESTINATION ${LIB_INSTALL_DIR}/stm32
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

#install libs
install(FILES  ${CMAKE_SOURCE_DIR}/cmake_arm/libfreertos_lib.a DESTINATION ${LIB_INSTALL_DIR}/
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
install(FILES  ${CMAKE_SOURCE_DIR}/cmake_arm/libstm32f1_hal_lib.a DESTINATION ${LIB_INSTALL_DIR}/
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
set(MAP_FILE ${PROJECT_SOURCE_DIR}/build/output.map)
set_target_properties(${BUILD_TARGET} PROPERTIES LINK_FLAGS
    "-mcpu=${MCU_ARCH}\
    -mthumb\
    -O0\
    -specs=nosys.specs\
    -Wl,-gc-sections -T${PROJECT_SOURCE_DIR}/${MCU_LINKER_SCRIPT}\
    -Wl,-Map,${MAP_FILE}")

target_compile_options(${BUILD_TARGET} PUBLIC 
                             -mcpu=${MCU_ARCH}
                             -mthumb -mthumb-interwork
                             -g -O0
                             -ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

set(compile_options -mcpu=${MCU_ARCH}
                    -mthumb
                    -mfloat-abi=${MCU_FLOAT_ABI} -std=gnu99 
                    -ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

target_compile_options(freertos_lib PUBLIC ${compile_options})
target_compile_options(stm32f1_hal_lib PUBLIC ${compile_options})
target_compile_options(${BUILD_TARGET} PUBLIC ${compile_options})
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)
set(LIST_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.lst)
set(ELF_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.elf)
add_dependencies(${BUILD_TARGET} generator_pre_compilation)
add_custom_target(generator_pre_compilation
    COMMAND python ${PROJECT_SOURCE_DIR}/generator/project_generator.py --pre -m "default" -d ${DEVICE_TYPE}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/generator/
    COMMENT "Running project generator...")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")

#add_custom_command(TARGET ${BUILD_TARGET} POST_BUILD
#        COMMAND python ${PROJECT_SOURCE_DIR}/generator/project_generator.py --post -m "default" -d ${DEVICE_TYPE}
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/generator/
#        COMMENT "Running add crc header...")

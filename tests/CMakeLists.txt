find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Common source files
set(COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/src/hal/gpio.c
    ${CMAKE_SOURCE_DIR}/src/hal/uart.c
    ${CMAKE_SOURCE_DIR}/src/drivers/led.c
    ${CMAKE_SOURCE_DIR}/src/drivers/sensor.c
)

# GPIO Tests
add_executable(test_gpio test_gpio.c ${CMAKE_SOURCE_DIR}/src/hal/gpio.c)
target_link_libraries(test_gpio m Threads::Threads)
add_test(NAME GPIO_Tests COMMAND test_gpio)

# UART Tests
add_executable(test_uart test_uart.c ${CMAKE_SOURCE_DIR}/src/hal/uart.c)
target_link_libraries(test_uart m Threads::Threads)
add_test(NAME UART_Tests COMMAND test_uart)

# LED Tests (depends on GPIO)
add_executable(test_led test_led.c ${CMAKE_SOURCE_DIR}/src/hal/gpio.c ${CMAKE_SOURCE_DIR}/src/drivers/led.c)
target_link_libraries(test_led m Threads::Threads)
add_test(NAME LED_Tests COMMAND test_led)

# Sensor Tests
add_executable(test_sensor test_sensor.c ${CMAKE_SOURCE_DIR}/src/drivers/sensor.c)
target_link_libraries(test_sensor m Threads::Threads)
add_test(NAME Sensor_Tests COMMAND test_sensor)

# Full integration test (keep the comprehensive one)
add_executable(test_drivers test_drivers.c ${COMMON_SOURCES})
target_link_libraries(test_drivers m Threads::Threads)
add_test(NAME Integration_Tests COMMAND test_drivers)

# Set environment for all tests
set_tests_properties(
    GPIO_Tests UART_Tests LED_Tests Sensor_Tests Integration_Tests
    PROPERTIES ENVIRONMENT "BUILD_TYPE=${CMAKE_BUILD_TYPE}"
)
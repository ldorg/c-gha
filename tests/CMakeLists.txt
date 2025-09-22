find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test_drivers
    test_drivers.c
    ${CMAKE_SOURCE_DIR}/src/hal/gpio.c
    ${CMAKE_SOURCE_DIR}/src/hal/uart.c
    ${CMAKE_SOURCE_DIR}/src/drivers/led.c
    ${CMAKE_SOURCE_DIR}/src/drivers/sensor.c
)

target_link_libraries(test_drivers
    m
    Threads::Threads
)

add_test(NAME DriverTests
    COMMAND test_drivers
)

set_property(TEST DriverTests PROPERTY
    ENVIRONMENT "BUILD_TYPE=${CMAKE_BUILD_TYPE}"
)
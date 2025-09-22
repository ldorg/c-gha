add_library(hal STATIC
    hal/gpio.c
    hal/uart.c
)

target_include_directories(hal PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    hal
)

add_library(drivers STATIC
    drivers/led.c
    drivers/sensor.c
)

target_include_directories(drivers PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    drivers
)

target_link_libraries(drivers hal)

add_executable(demo-firmware
    main.c
)

target_include_directories(demo-firmware PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(demo-firmware
    drivers
    hal
    m
)

install(TARGETS demo-firmware
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
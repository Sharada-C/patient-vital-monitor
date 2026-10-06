cmake_minimum_required(VERSION 3.16)

project(patient_vital_monitor
    VERSION 1.0
    DESCRIPTION "Multithreaded patient vital-sign monitoring simulation"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

include_directories(include)

add_executable(patient_monitor
    src/main.c
    src/ring_buffer.c
    src/monitor.c
    src/sensor.c
    src/processor.c
    src/alarm.c
    src/logger.c
)

find_package(Threads REQUIRED)

target_link_libraries(patient_monitor
    PRIVATE
    Threads::Threads
)

enable_testing()

add_executable(test_alarm
    tests/test_alarm.c
    src/alarm.c
)

target_link_libraries(test_alarm
    PRIVATE
    Threads::Threads
)

add_executable(test_ring_buffer
    tests/test_ring_buffer.c
    src/ring_buffer.c
)

target_link_libraries(test_ring_buffer
    PRIVATE
    Threads::Threads
)

add_executable(test_vital
    tests/test_vital.c
)

target_link_libraries(test_vital
    PRIVATE
    Threads::Threads
)

add_test(NAME AlarmTests COMMAND test_alarm)
add_test(NAME RingBufferTests COMMAND test_ring_buffer)
add_test(NAME VitalTests COMMAND test_vital)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/logs)

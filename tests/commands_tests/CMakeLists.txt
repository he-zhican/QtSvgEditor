add_executable(commands_tests
    commadntests.cpp
)

target_include_directories(commands_tests PRIVATE .)

target_link_libraries(commands_tests
    commands
    Qt5::Core
    Qt5::Test
)

add_test(NAME CommandTests COMMAND commands_tests)

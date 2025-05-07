add_executable(core_tests
    svgdocumenttest.cpp
    svgparsertest.cpp
)

target_include_directories(core_tests PRIVATE .)

target_link_libraries(core_tests
    core
    Qt5::Core
    Qt5::Test
)

add_test(NAME CoreTests COMMAND core_tests)

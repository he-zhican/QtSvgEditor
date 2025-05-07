add_executable(gui_tests
    canvasviewtest.cpp
    propertypaneltest.cpp
)

target_include_directories(gui_tests PRIVATE .)

target_link_libraries(gui_tests
    gui
    Qt5::Core
    Qt5::Test
)

add_test(NAME GuiTests COMMAND gui_tests)

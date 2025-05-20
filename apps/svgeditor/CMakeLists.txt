add_executable(svgeditor WIN32
    src/main.cpp
    ${RCC_FILE}
)

target_include_directories(svgeditor PRIVATE ${PROJECT_SOURCE_DIR}/libs/gui/include)

target_link_libraries(svgeditor
    core
    commands
    gui
    interaction
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Svg
)
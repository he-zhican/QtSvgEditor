add_executable(viewer
    src/main.cpp
)

target_include_directories(viewer PRIVATE src)

target_link_libraries(viewer
    core
    gui
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Svg
)

add_executable(svgeditor WIN32
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${RCC_FILE}
)

target_include_directories(svgeditor PRIVATE ${PROJECT_SOURCE_DIR}/libs/gui/include)

target_link_libraries(svgeditor
    gui
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Svg
)
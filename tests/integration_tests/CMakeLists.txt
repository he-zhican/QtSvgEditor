# 设置目标
add_executable(integration_tests
    IntegrationTest.cpp
)

# 包含头文件
target_include_directories(integration_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/libs/core/include
    ${PROJECT_SOURCE_DIR}/libs/commands/include
    ${PROJECT_SOURCE_DIR}/libs/gui/include
    ${PROJECT_SOURCE_DIR}/libs/interaction/include
    ${PROJECT_SOURCE_DIR}/libs/files/include
    ${PROJECT_SOURCE_DIR}/libs/utils/include
)

# 链接库
target_link_libraries(integration_tests
    core
    commands
    gui
    interaction
    files
    utils
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

# find_package(Qt6 REQUIRED COMPONENTS Test)
# target_link_libraries(integration_tests Qt5::Test)

file(GLOB COMMANDS_SRC "src/*.cpp")
file(GLOB COMMANDS_HDR "include/*.h")

add_library(commands SHARED ${COMMANDS_SRC} ${COMMANDS_HDR})

target_compile_definitions(commands PRIVATE COMMANDS_LIBRARY)
target_include_directories(commands PUBLIC include)
target_link_libraries(commands core Qt5::Widgets)

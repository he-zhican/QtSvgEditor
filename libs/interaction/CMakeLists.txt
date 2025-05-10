file(GLOB INTERACTION_SRC "src/*.cpp")
file(GLOB INTERACTION_HDR "include/*.h")

add_library(interaction SHARED ${INTERACTION_SRC} ${INTERACTION_HDR})

add_dependencies(interaction core commands)
target_include_directories(interaction PUBLIC include)
target_link_libraries(interaction core commands Qt5::Widgets)

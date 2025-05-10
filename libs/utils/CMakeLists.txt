file(GLOB UTILS_SRC "src/*.cpp")
file(GLOB UTILS_HDR "include/*.h")

add_library(utils STATIC ${UTILS_SRC} ${UTILS_HDR})

target_include_directories(utils PUBLIC include)
target_link_libraries(utils Qt5::Core Qt5::Widgets Qt5::Network)

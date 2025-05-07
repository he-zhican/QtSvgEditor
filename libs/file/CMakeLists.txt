file(GLOB FILE_SRC "src/*cpp")
file(GLOB FILE_HDR "include/*h")

add_library(file STATIC ${FILE_SRC} ${FILE_HDR})
add_dependencies(file core)
target_include_directories(file PUBLIC include)
target_link_libraries(file core Qt5::Widgets)

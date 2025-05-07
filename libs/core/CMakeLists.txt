file(GLOB CORE_SRC "src/*cpp")
file(GLOB CORE_HDR "include/*h")

add_library(core SHARED ${CORE_SRC} ${CORE_HDR})

target_include_directories(core PUBLIC include)

target_link_libraries(core PUBLIC Qt5::Core Qt5::Svg Qt5::Xml)

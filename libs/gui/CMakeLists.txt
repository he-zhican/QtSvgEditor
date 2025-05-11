file(GLOB GUI_SRC "src/*.cpp")
file(GLOB GUI_HDR "include/*.h")

source_group("Resource Files" FILES ${RCC_FILE})

add_library(gui SHARED ${GUI_SRC} ${GUI_HDR} ${RCC_FILE})
# add_dependencies(gui core commands interaction file)
target_include_directories(gui PUBLIC include)
target_link_libraries(gui core commands interaction file Qt5::Core Qt5::Widgets)

set(GUI_HDR 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/canvasview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/canvaspropertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/clipboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ellipsepropertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/graphicssvgitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/linepropertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/menubar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/multipropertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pathpropertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/propertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/propertypanelfactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rectpropertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sidetoolbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/textpropertypanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_mainwindow.h
)

set(GUI_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/canvasview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/canvaspropertypanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/clipboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ellipsepropertypanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphicssvgitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/linepropertypanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/menubar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/multipropertypanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pathpropertypanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/propertypanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/propertypanelfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rectpropertypanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sidetoolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/textpropertypanel.cpp
)

add_library(gui STATIC ${GUI_SRC} ${GUI_HDR})

target_include_directories(gui PUBLIC include)
target_link_libraries(gui core commands interaction Qt5::Core Qt5::Widgets Qt5::Network)
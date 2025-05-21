set(INTERACTION_HDR 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/canvascontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ellipsetoolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/freehandtoolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/linetoolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/movetoolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/polygontoolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/recttoolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/selectionmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/texttoolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/toolcontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/toolid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zoomtoolcontroller.h
)

set(INTERACTION_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/canvascontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ellipsetoolcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/freehandtoolcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/linetoolcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/movetoolcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/polygontoolcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/recttoolcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/selectionmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/texttoolcontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/zoomtoolcontroller.cpp
)

add_library(interaction STATIC ${INTERACTION_SRC} ${INTERACTION_HDR})

add_dependencies(interaction core commands)
target_include_directories(interaction PUBLIC include)
target_link_libraries(interaction core commands Qt5::Core Qt5::Widgets)
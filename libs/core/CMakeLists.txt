set(CORE_HDR 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgdocument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgwriter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgparser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgexception.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgelement.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgrect.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgellipse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgpolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgfreehand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/svgtext.h
)

set(CORE_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgdocument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgwriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgelement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgrect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgellipse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgpolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgfreehand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svgtext.cpp
)

add_library(core STATIC ${CORE_SRC} ${CORE_HDR})

target_include_directories(core PUBLIC include)
target_link_libraries(core Qt5::Core Qt5::Svg Qt5::Xml)
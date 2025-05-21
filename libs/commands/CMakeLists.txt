set(COMMANDS_HDR 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/command_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/commandmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/command.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/addelementscommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/deleteelementscommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/moveelementscommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/resizeelementcommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ordercommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/changeattributecommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/changeelementsattributecommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/changedocattributecommand.h
)

add_library(commands SHARED ${COMMANDS_HDR})

target_compile_definitions(commands PRIVATE COMMANDS_LIBRARY)
target_include_directories(commands PUBLIC include)
target_link_libraries(commands core Qt5::Widgets)

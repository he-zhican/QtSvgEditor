cmake_minimum_required(VERSION 3.10)

get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE) 
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME) 

set(CMAKE_TOOLCHAIN_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeProject/thirdparty_install/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(QTSVGEditor)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(RCC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc)

# 设置导出所有库符号，否则无法导出动态库的.lib文件
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui Network Svg Xml Test REQUIRED)

# 添加子目录
add_subdirectory(libs)
add_subdirectory(apps)
add_subdirectory(tests)
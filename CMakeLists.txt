cmake_minimum_required(VERSION 3.25)

project(CheckAPIResponse)

set(CMAKE_CXX_STANDARD 23)

include(source/common/CMakeLists.txt)
foreach(FILE ${FILES})
  set(source ${source} source/common/${FILE})
endforeach()

add_executable(CheckAPIResponse source/main.cpp ${source})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(CheckAPIResponse PRIVATE /MP /MTd /Zi /W3 /WX /fp:fast)
  target_link_options(CheckAPIResponse PRIVATE /SUBSYSTEM:CONSOLE)
endif()

target_precompile_headers(CheckAPIResponse PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/pch.h)

target_include_directories(CheckAPIResponse
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xerces-c-3.3.0
)

target_link_libraries(CheckAPIResponse
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xerces-c-3.3.0/lib/xerces-c_3D.lib
)

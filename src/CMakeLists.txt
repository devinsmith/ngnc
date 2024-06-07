cmake_minimum_required(VERSION 3.13.4)

project(ngnc)
set(TARGET_NAME ngnc)

find_package(FOX REQUIRED)
find_package(OpenSSL REQUIRED)

# Disable rdynamic
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# C and C++ sources are freely mixed.
set(SOURCES
  chattabitem.cpp
  connectdlg.cpp
  dnsresolver.cpp
  icons.cpp
  main.cpp
  nakenclient.cpp
  ngnc.cpp
  prefs.cpp
  prefsdlg.cpp
)

set(HEADERS
  chattabitem.h
  connectdlg.h
  defs.h
  dnsresolver.h
  icons.h
  nakenclient.h
  ngnc.h
  prefsdlg.h
  prefs.h
)


add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PRIVATE ${FOX_INCLUDE_DIR})
set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS ${FOX_CXX_FLAGS})
target_link_libraries(${TARGET_NAME} PRIVATE ${OPENSSL_LIBRARIES} ${FOX_LIBRARY})

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Werror)
endif()


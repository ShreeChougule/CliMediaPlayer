cmake_minimum_required (VERSION 2.6)
project (Indexing)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fstack-protector-all -lz")
include_directories(${PROJECT_SOURCE_DIR})

find_package(ZLIB)
find_package(taglib)
add_subdirectory(database)
include_directories("/usr/local/usr/include/kivi/osal")

set (SOURCES
  "USBIndexer/mediascanner.cpp"
    "USBIndexer/CoverartExtractor.cpp"
    "USBIndexer/MetaData.cpp"
    "USBIndexer/metadatareader.cpp"
    "USBIndexer/usbindexer.cpp"
)

set(LIBS 
   ${LIBS}
   ${TAGLIB_LIBRARY}
   "/usr/local/usr/lib/libkivicore.so"
   ${ZLIB_LIBRARIES}
   rt tag
)
add_library(index STATIC ${SOURCES})
target_link_libraries(index dbase ${LIBS})






















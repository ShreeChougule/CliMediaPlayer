cmake_minimum_required (VERSION 2.6)
project (GstreamerLocal)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fstack-protector-all -lz -pthread")

find_package(GLib)
find_package(ZLIB)
find_package(taglib)
find_package(GStreamer)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${GLIB_INC_DIR})
include_directories(${GLIBCONFIG_INC_DIR})
include_directories(${GSTREAMER_INCLUDE_DIR})
include_directories("/usr/local/usr/include/kivi/osal")

set (SOURCES
    "GstreamerImpl.cpp"
)

set(LIBS 
	${LIBS}
	${GLIB_LIBRARIES}
	${ZLIB_LIBRARIES}
	${TAGLIB_LIBRARY}
	${GSTREAMER_LIBRARIES}
	${GSTREAMER_BASE_LIBRARY}
	"/usr/local/usr/lib/libkivicore.so"
	rt tag
)

add_library(stream SHARED ${SOURCES})
target_link_libraries(stream ${LIBS})






















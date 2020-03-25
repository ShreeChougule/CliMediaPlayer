cmake_minimum_required (VERSION 2.6)
project (PlayerService)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fstack-protector-all -lz -pthread")
include_directories(${PROJECT_SOURCE_DIR})

find_package(GLib)
find_package(ZLIB)
find_package(GStreamer)


include_directories(${GLIB_INC_DIR})
include_directories(${GLIBCONFIG_INC_DIR})
include_directories(${GSTREAMER_INCLUDE_DIR})

add_subdirectory(usbIndexer)
add_subdirectory(GstreamerLocal)
add_subdirectory(DbHandler)

set (SOURCES
    "MainLoop.cpp"
)

set(LIBS 
	${LIBS}
	${GLIB_LIBRARIES}
	${GSTREAMER_LIBRARIES}
	${GSTREAMER_BASE_LIBRARY}
	index
	stream
	DbHandler
)

add_executable(player ${SOURCES})
target_link_libraries(player ${LIBS})






















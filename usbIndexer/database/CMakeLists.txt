cmake_minimum_required (VERSION 2.6)

project (Indexing)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fstack-protector-all")


find_package(sqlite)

include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(/usr/include)
include_directories(${SQLITE_INCLUDE_DIR})




set (SOURCES
  "database.cpp"
  
)

add_library(dbase SHARED ${SOURCES})

set(LIBS 
    ${LIBS}  
    ${SQLITE_LIBRARIES}    
)

target_link_libraries(dbase ${LIBS})






















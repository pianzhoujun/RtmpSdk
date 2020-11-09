PROJECT (rtmpsdk)
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
SET (CMAKE_CXX_FLAGS "-std=c++11 -g")

include_directories (
    ${PROJECT_SOURCE_DIR}/include
)

aux_source_directory  (
    ${PROJECT_SOURCE_DIR}/src SRC_LIST
)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp ${SRC_LIST})

find_package(Threads)

target_link_libraries(
    ${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_net.a 
    ${CMAKE_SOURCE_DIR}/lib/libmuduo_base.a
    ${CMAKE_THREAD_LIBS_INIT}
)

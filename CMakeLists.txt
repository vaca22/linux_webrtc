cmake_minimum_required(VERSION 3.21)
project(linux_webrtc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
add_definitions(-DWEBRTC_POSIX )

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)


find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

include_directories(${X11_INCLUDE_DIR})

link_directories(${X11_LIBRARIES})

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})


include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


include_directories(${CMAKE_SOURCE_DIR}/include/third_party/libyuv/include)
include_directories(${CMAKE_SOURCE_DIR}/client)
include_directories(abseil-cpp)
link_directories(abseil-cpp)
add_subdirectory(abseil-cpp)




add_executable(client client/linux/main.cc  client/linux/main_wnd.cc client/conductor.cc client/defaults.cc client/peer_connection_client.cc)


set(absl_LIBRARIES2
        absl::algorithm
        absl::base
        absl::debugging
        absl::flat_hash_map
        absl::memory
        absl::meta
        absl::numeric
        absl::str_format
        absl::strings
        absl::synchronization
        absl::time
        absl::flags
        absl::flags_internal
        absl::utility)




target_link_libraries(client  ${CMAKE_SOURCE_DIR}/libwebrtc.a
        Threads::Threads
        ${GTK3_LIBRARIES} ${X11_LIBRARIES} ${CMAKE_DL_LIBS}
        ${CMAKE_DL_LIBS}
        ${CMAKE_SOURCE_DIR}/libjsoncpp.so.24
        ${absl_LIBRARIES2}
        )


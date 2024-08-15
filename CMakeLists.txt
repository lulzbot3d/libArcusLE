cmake_policy(SET CMP0091 NEW)
project(libarcusle)
cmake_minimum_required(VERSION 3.23)
find_package(standardprojectsettings REQUIRED)
find_package(protobuf REQUIRED)

set(arcusle_SRCS
    src/Socket.cpp
    src/SocketListener.cpp
    src/MessageTypeStore.cpp
    src/PlatformSocket.cpp
    src/Error.cpp
)

if(BUILD_SHARED_LIBS)
    add_library(ArcusLE SHARED ${arcusle_SRCS})
else()
    add_library(ArcusLE STATIC ${arcusle_SRCS})
endif()

set_project_warnings(ArcusLE)
enable_sanitizers(ArcusLE)
use_threads(ArcusLE)

target_include_directories(ArcusLE
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
target_link_libraries(ArcusLE PUBLIC protobuf::libprotobuf)

if(WIN32)
    target_compile_definitions(ArcusLE PRIVATE -D_WIN32_WINNT=0x0600)
    # Declare we require Vista or higher, this allows us to use IPv6 functions.

    target_link_libraries(ArcusLE PUBLIC Ws2_32)
    set_target_properties(ArcusLE PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

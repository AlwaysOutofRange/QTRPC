cmake_minimum_required(VERSION 3.10)

# Remove when sharing with others.
list(APPEND CMAKE_PREFIX_PATH "/home/nico/Qt/Tools/QtCreator/lib/cmake/QtCreator")

project(QTRPC)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QtCreator COMPONENTS Core REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
set(QtX Qt${QT_VERSION_MAJOR})

add_qtc_plugin(QTRPC
  PLUGIN_DEPENDS
    QtCreator::Core
    QtCreator::ProjectExplorer
  DEPENDS
    ${QtX}::Widgets
    QtCreator::ExtensionSystem
    QtCreator::Utils
  SOURCES
    .github/workflows/build_cmake.yml
    .github/workflows/README.md
    README.md
    qtrpc.cpp
    qtrpc.h
    qtrpc_global.h
    qtrpcconstants.h
)

target_link_libraries(QTRPC PUBLIC "libdiscord-rpc.so")

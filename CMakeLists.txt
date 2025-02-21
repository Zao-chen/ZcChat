cmake_minimum_required(VERSION 3.16)

project(ZcChat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Multimedia REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ZcChat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        galgamedialog.h galgamedialog.cpp galgamedialog.ui
        tachie.h tachie.cpp tachie.ui
        include/ElaWidgetTools.qrc
        res.qrc
        logo.rc
        vad.h vad.cpp
        log.h
    )
else()
    if(ANDROID)
        add_library(ZcChat SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ZcChat
            ${PROJECT_SOURCES}
            include/ElaWidgetTools.qrc
        )
    endif()
endif()

include_directories(
    include
)

# 链接 winmm.lib
target_link_libraries(ZcChat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network Qt6::Multimedia ${CMAKE_CURRENT_SOURCE_DIR}/lib/elawidgettools.lib winmm)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ZcChat)
endif()

set_target_properties(ZcChat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ZcChat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ZcChat)
endif()

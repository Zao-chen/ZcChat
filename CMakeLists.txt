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
        utils/vad.h utils/vad.cpp
        utils/log.h
        mianwindow_uichild/setting_general.h mianwindow_uichild/setting_general.cpp mianwindow_uichild/setting_general.ui
        mianwindow_uichild/setting_ai.h mianwindow_uichild/setting_ai.cpp mianwindow_uichild/setting_ai.ui
        mianwindow_uichild/setting_vits.h mianwindow_uichild/setting_vits.cpp mianwindow_uichild/setting_vits.ui
        mianwindow_uichild/setting_voiceinput.h mianwindow_uichild/setting_voiceinput.cpp mianwindow_uichild/setting_voiceinput.ui
        mianwindow_uichild/setting_actor.h mianwindow_uichild/setting_actor.cpp mianwindow_uichild/setting_actor.ui
        galgamedialog_uichild/history.h galgamedialog_uichild/history.cpp galgamedialog_uichild/history.ui
        galgamedialog_uichild/historychild.h galgamedialog_uichild/historychild.cpp galgamedialog_uichild/historychild.ui
        dialog/messagewindow.h dialog/messagewindow.cpp dialog/messagewindow.ui
        utils/createwin.h utils/createwin.cpp
        mianwindow_uichild/about.h mianwindow_uichild/about.cpp mianwindow_uichild/about.ui


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

# 添加 nlohmann/json 头文件路径
include_directories(
    include
    ${CMAKE_SOURCE_DIR}/third_party/json
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


## 生成ts文件的文件夹
set(TS_DIR "${CMAKE_SOURCE_DIR}/translations")

set(TS_FILES
    "${TS_DIR}/en_US.ts"
    "${TS_DIR}/zh_CN.ts"
    "${TS_DIR}/zh_TW.ts"
    "${TS_DIR}/ja_JP.ts"
)

find_program(LUPDATE_EXECUTABLE lupdate)

foreach(_ts_file ${TS_FILES})
    execute_process(
        COMMAND ${LUPDATE_EXECUTABLE} -recursive ${CMAKE_SOURCE_DIR} -ts ${_ts_file}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach()



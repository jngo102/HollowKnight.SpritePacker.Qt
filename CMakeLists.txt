cmake_minimum_required(VERSION 3.5)

project(SpritePacker LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES SpritePacker_zh_CN.ts)

set(PROJECT_SOURCES
    animation.cpp
        animation.hpp
        clip.hpp
        collection.cpp
        collection.hpp
        logger.cpp
        logger.hpp
        main.cpp
        packer.cpp
        packer.hpp
        sprite.hpp
        sprite_animation.cpp
        sprite_animation.hpp
        sprite_info.hpp
        sprite_packer.cpp
        sprite_packer.hpp
        sprite_packer.ui
        sprites_folder.hpp
        stdafx.cpp
        stdafx.hpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SpritePacker
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(SpritePacker SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(SpritePacker
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(SpritePacker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

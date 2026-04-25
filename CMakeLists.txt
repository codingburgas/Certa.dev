cmake_minimum_required(VERSION 3.19)
project(Certa.dev LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    main.cpp

    assets/resources.qrc
    assets/Certadev.db

    presentationLayer/theme/color_theme.json

    presentationLayer/layout/layout.cpp
    presentationLayer/layout/layout.h
    presentationLayer/layout/layout.ui

    presentationLayer/widgets/home/home.cpp
    presentationLayer/widgets/home/home.h
    presentationLayer/widgets/home/home.ui

    presentationLayer/widgets/auth/auth.cpp
    presentationLayer/widgets/auth/auth.h
    presentationLayer/widgets/auth/auth.ui

    presentationLayer/widgets/auth/interestsDialog/interestsDialog.cpp
    presentationLayer/widgets/auth/interestsDialog/interestsDialog.h
    presentationLayer/widgets/auth/interestsDialog/interestsDialog.ui

    presentationLayer/widgets/movies/movies.cpp
    presentationLayer/widgets/movies/movies.h
    presentationLayer/widgets/movies/movies.ui
)

qt_add_executable(Certa.dev
    ${PROJECT_SOURCES}
)


target_link_libraries(Certa.dev PRIVATE
    Qt::Core
    Qt::Widgets
)

target_include_directories(Certa.dev PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/presentationLayer/layout
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/home
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/auth
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/auth/interestsDialog
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/movies
)

include(GNUInstallDirs)

install(TARGETS Certa.dev
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Certa.dev
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

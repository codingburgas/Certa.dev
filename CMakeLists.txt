cmake_minimum_required(VERSION 3.19)
project(Certa.dev LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp

    presentationLayer/home/home.cpp
    presentationLayer/home/home.h
    presentationLayer/home/home.ui
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

    ${CMAKE_SOURCE_DIR}/presentationLayer/home
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

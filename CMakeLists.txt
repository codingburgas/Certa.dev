cmake_minimum_required(VERSION 3.19)
project(Certa.dev LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Network)

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

    presentationLayer/widgets/movies/movie/movie.cpp
    presentationLayer/widgets/movies/movie/movie.h
    presentationLayer/widgets/movies/movie/movie.ui

    presentationLayer/widgets/common/movieCard/movieCard.cpp
    presentationLayer/widgets/common/movieCard/movieCard.h

    presentationLayer/widgets/recommendations/recommendations.cpp
    presentationLayer/widgets/recommendations/recommendations.h
    presentationLayer/widgets/recommendations/recommendations.ui

    presentationLayer/widgets/favorites/favorites.cpp
    presentationLayer/widgets/favorites/favorites.h
    presentationLayer/widgets/favorites/favorites.ui

    presentationLayer/widgets/settings/settings.cpp
    presentationLayer/widgets/settings/settings.h
    presentationLayer/widgets/settings/settings.ui

    presentationLayer/widgets/profile/profile.cpp
    presentationLayer/widgets/profile/profile.h
    presentationLayer/widgets/profile/profile.ui


    serviceLayer/userSession/userSession.cpp
    serviceLayer/userSession/userSession.h

    serviceLayer/authService/authService.cpp
    serviceLayer/authService/authService.h

    serviceLayer/authService/passwordHasher/passwordHasher.cpp
    serviceLayer/authService/passwordHasher/passwordHasher.h

    serviceLayer/userService/userService.cpp
    serviceLayer/userService/userService.h

    serviceLayer/movieService/movieService.cpp
    serviceLayer/movieService/movieService.h


    dataAccessLayer/database/database.cpp
    dataAccessLayer/database/database.h

    dataAccessLayer/authRepository/authRepository.cpp
    dataAccessLayer/authRepository/authRepository.h

    dataAccessLayer/userRepository/userRepository.cpp
    dataAccessLayer/userRepository/userRepository.h

    dataAccessLayer/movieRepository/movieRepository.cpp
    dataAccessLayer/movieRepository/movieRepository.h

    models/user/user.h
    models/user/userResponse.h
    models/user/userStatus.h

    models/auth/authStatus.h
    models/auth/authResponse.h

    models/movie/movieDto.h
    models/movie/movieStatus.h
    models/movie/movieResponse.h
)

qt_add_executable(Certa.dev
    ${PROJECT_SOURCES}
)

target_link_libraries(Certa.dev PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Sql
    Qt::Network
)

target_include_directories(Certa.dev PRIVATE
    ${CMAKE_SOURCE_DIR}

    ${CMAKE_SOURCE_DIR}/presentationLayer/layout
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/home
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/auth
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/auth/interestsDialog
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/movies
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/movies/movie
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/common/movieCard
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/recommendations
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/favorites
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/settings
    ${CMAKE_SOURCE_DIR}/presentationLayer/widgets/profile

    ${CMAKE_SOURCE_DIR}/serviceLayer/userSession
    ${CMAKE_SOURCE_DIR}/serviceLayer/authService
    ${CMAKE_SOURCE_DIR}/serviceLayer/authService/passwordHasher
    ${CMAKE_SOURCE_DIR}/serviceLayer/userService
    ${CMAKE_SOURCE_DIR}/serviceLayer/movieService

    ${CMAKE_SOURCE_DIR}/dataAccessLayer/database
    ${CMAKE_SOURCE_DIR}/dataAccessLayer/authRepository
    ${CMAKE_SOURCE_DIR}/dataAccessLayer/userRepository
    ${CMAKE_SOURCE_DIR}/dataAccessLayer/movieRepository

    ${CMAKE_SOURCE_DIR}/models
    ${CMAKE_SOURCE_DIR}/models/auth
    ${CMAKE_SOURCE_DIR}/models/user
    ${CMAKE_SOURCE_DIR}/models/movie
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

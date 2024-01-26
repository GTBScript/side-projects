cmake_minimum_required(VERSION 3.16)
project(AsteroidBelt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(AsteroidBelt Sources/main.cpp
        Sources/Actions.cpp
        Sources/Actions.h
        Sources/Includes.cpp
        Sources/Includes.h
        Sources/UIElements/UIButtons/UIButton.cpp
        Sources/UIElements/UIButtons/UIButton.h
        Sources/UIElements/UIButtons/StaticUIButton.cpp
        Sources/UIElements/UIButtons/StaticUIButton.h
        Sources/UIElements/UIButtons/DynamicUIButton.cpp
        Sources/UIElements/UIButtons/DynamicUIButton.h
        Sources/Scenes/Scene.cpp
        Sources/Scenes/Scene.h
        Sources/UIElements/UILabels/UILabels.cpp
        Sources/UIElements/UILabels/UILabels.h
        Sources/Scenes/MakeScene.cpp
        Sources/Scenes/MakeScene.h
        Sources/UIElements/UIImages/UIImage.cpp
        Sources/UIElements/UIImages/UIImage.h
        Sources/Internal.cpp
        Sources/Internal.h
)
target_link_libraries(AsteroidBelt PRIVATE sfml-graphics sfml-audio sfml-window sfml-network sfml-system)
target_compile_features(AsteroidBelt PRIVATE cxx_std_23)

if(WIN32)
    add_custom_command(
            TARGET AsteroidBelt
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:AsteroidBelt>
            VERBATIM)
endif()

install(TARGETS AsteroidBelt)
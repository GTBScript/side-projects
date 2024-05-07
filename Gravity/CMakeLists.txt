cmake_minimum_required(VERSION 3.16)
project(Gravity LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

if(WIN32)
    set(VLD_DIR "C:/Program Files (x86)/Visual Leak Detector")
    include_directories(${VLD_DIR}/include)
    link_directories(${VLD_DIR}/lib/Win64)
    add_definitions(-DVLDBUILD)
endif()

add_executable(Gravity Sources/main.cpp
        Headers/Scene/Scene.h
        Headers/UI/UIButton.h
        Sources/UI/UIButton.cpp
        Headers/Scene/MainMenu.h
        Sources/Scene/Scene.cpp
        Sources/Scene/MainMenu.cpp
        Headers/Scene/MakeScene.h
        Sources/Scene/MakeScene.cpp
        Headers/Action.h
        Headers/UI/UILabel.h
        Sources/UI/UILabel.cpp
)

target_link_libraries(Gravity PRIVATE sfml-graphics sfml-audio sfml-window sfml-network sfml-system vld)
target_compile_features(Gravity PRIVATE cxx_std_23)

if(WIN32)
    add_custom_command(
            TARGET Gravity
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Gravity>
            VERBATIM)
endif()

if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()


install(TARGETS Gravity)


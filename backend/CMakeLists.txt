cmake_minimum_required(VERSION 3.14)
project(banchoo)

set(PROJECT_SERVER ${PROJECT_NAME}_server)

# 선택 가능한 구성 설정
set(CONFIG_TYPE "simple" CACHE STRING "Config type (simple, test, api_test, release)")
set_property(CACHE CONFIG_TYPE PROPERTY STRINGS simple test api_test, release)
message(STATUS "Selected config: ${CONFIG_TYPE}.json")


# 경로 설정
set(CONFIG_SRC "${CMAKE_SOURCE_DIR}/config/${CONFIG_TYPE}.json")
set(CONFIG_DST "${CMAKE_BINARY_DIR}/config.json")

# 파일 존재 여부 체크
if(NOT EXISTS ${CONFIG_SRC})
    message(FATAL_ERROR "Config file '${CONFIG_SRC}' not found!")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (옵션) 빌드 타입 명시 – Debug로 하는 게 보통 clang-tidy에서 오류 덜 남
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(third_party/sqlite)

set(SERVER_SRC 
    ${PROJECT_SOURCE_DIR}/src/app/app_factory.cpp 
    ${PROJECT_SOURCE_DIR}/src/app/crow_app.cpp 
    ${PROJECT_SOURCE_DIR}/src/repository/inmemory_repository.cpp
    ${PROJECT_SOURCE_DIR}/src/repository/sqlite_repository.cpp
    ${PROJECT_SOURCE_DIR}/src/repository/base_repository.cpp
    ${PROJECT_SOURCE_DIR}/src/repository/repository_factory.cpp
)

# 실행 파일 이름
add_executable(${PROJECT_SERVER}
    src/main.cpp
    ${SERVER_SRC}
)

# pthread (Crow에서 필요함)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_SERVER} PRIVATE Threads::Threads sqlite3)

# include 경로 설정
target_include_directories(${PROJECT_SERVER}
    PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/third_party/
        ${PROJECT_SOURCE_DIR}/third_party/spdlog/include
)

configure_file(${CONFIG_SRC} ${CONFIG_DST} COPYONLY)


# main executable이 실행되기 전에 복사되도록 연결
add_custom_target(copy_config ALL DEPENDS ${CONFIG_DST})
add_dependencies(${PROJECT_SERVER} copy_config)

if (CONFIG_TYPE STREQUAL "test")
    # test executable
    set(PROJECT_TEST ${PROJECT_NAME}_test)
    add_executable(${PROJECT_TEST}
        test/main.cpp
        test/test_inmemory_repository.cpp
        ${SERVER_SRC}
    )

    target_include_directories(${PROJECT_TEST}
        PRIVATE
            ${PROJECT_SOURCE_DIR}
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/third_party/
            ${PROJECT_SOURCE_DIR}/third_party/spdlog/include
            ${PROJECT_SOURCE_DIR}/third_party/doctest
    )

    target_link_libraries(${PROJECT_TEST} PRIVATE Threads::Threads sqlite3)
    add_dependencies(${PROJECT_TEST} copy_config)
    
    add_test(NAME banchoo_test COMMAND banchoo_test)
endif()
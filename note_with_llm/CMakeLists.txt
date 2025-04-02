cmake_minimum_required(VERSION 3.14)
project(banchoo)

set(PROJECT_SERVER ${PROJECT_NAME}_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_SRC 
    ${PROJECT_SOURCE_DIR}/src/app/app_factory.cpp 
    ${PROJECT_SOURCE_DIR}/src/app/crow_app.cpp 
    ${PROJECT_SOURCE_DIR}/src/repository/inmemory_repository.cpp
    ${PROJECT_SOURCE_DIR}/src/repository/repository_factory.cpp
)

# 실행 파일 이름
add_executable(${PROJECT_SERVER}
    src/main.cpp
    ${SERVER_SRC}
)

# include 경로 설정
target_include_directories(${PROJECT_SERVER}
    PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src
)

# pthread (Crow에서 필요함)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_SERVER} PRIVATE Threads::Threads)

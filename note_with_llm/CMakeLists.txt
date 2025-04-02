cmake_minimum_required(VERSION 3.14)
project(llm_with_note)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 실행 파일 이름
add_executable(llm_note_server
    src/main.cpp
)

# include 경로 설정
target_include_directories(llm_note_server
    PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src
)

# pthread (Crow에서 필요함)
find_package(Threads REQUIRED)
target_link_libraries(llm_note_server PRIVATE Threads::Threads)

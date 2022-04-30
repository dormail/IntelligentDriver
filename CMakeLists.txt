cmake_minimum_required(VERSION "3.7.1")

project("macroscopic")

include_directories("include")
add_subdirectory(include)

add_executable("${PROJECT_NAME}" "cpp/src/macroscopic.cpp")

target_link_libraries("${PROJECT_NAME}" PRIVATE road)
target_link_libraries("${PROJECT_NAME}" PRIVATE car)

project("IDMInterface")


add_executable("${PROJECT_NAME}" "main.cpp")

target_link_libraries("${PROJECT_NAME}" PRIVATE road)
target_link_libraries("${PROJECT_NAME}" PRIVATE car)

install(TARGETS IDMInterface DESTINATION bin)
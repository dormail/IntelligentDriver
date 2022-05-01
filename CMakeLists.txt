cmake_minimum_required(VERSION "3.7.1")

#SET(CMAKE_CXX_FLAGS "-Wall -g -std=c++2a")
SET(CMAKE_CXX_FLAGS "-std=c++2a")

project("macroscopicIDM")

include_directories("include")
add_subdirectory(include)

add_executable("${PROJECT_NAME}" "cpp/src/macroscopic.cpp")

target_link_libraries("${PROJECT_NAME}" PRIVATE road)
target_link_libraries("${PROJECT_NAME}" PRIVATE car)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)


# microscopic data
project("microscopicIDM")
add_executable("${PROJECT_NAME}" "cpp/src/microscopic.cpp")

target_link_libraries("${PROJECT_NAME}" PRIVATE road)
target_link_libraries("${PROJECT_NAME}" PRIVATE car)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

cmake_minimum_required(VERSION 2.6)

project(mkgit)

set(EXECUTABLE_OUTPUT_PATH build/${CMAKE_BUILD_TYPE})


file(
	GLOB_RECURSE
	sources
	mkgit.c/mkgit_gabriel.c
)

add_executable(
	mkgit
	${sources}
)

link_directories(${CMAKE_BINARY_DIR}/res)
target_link_libraries(mkgit git2)
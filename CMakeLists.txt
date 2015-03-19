cmake_minimum_required(VERSION 2.6)

project(mkgit)

set(EXECUTABLE_OUTPUT_PATH build/${CMAKE_BUILD_TYPE})

file(
	GLOB_RECURSE
	sources
	mkgit.c/*
)

add_executable(
	main.out
	${sources}
mkgit.c/test.h mkgit.c/test.c)
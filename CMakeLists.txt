cmake_minimum_required(VERSION 3.18)

project(
	Demo-Game
	VERSION 0.1
	DESCRIPTION "A game demoing the Platypus game engine."
	LANGUAGES CXX
)

include(cmake/config.cmake)
include(cmake/funcs.cmake)

add_subdirectory("source")

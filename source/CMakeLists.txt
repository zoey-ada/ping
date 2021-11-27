set(project Demo_Game)

add_subdirectory("libraries")

include_directories(
	${CMAKE_SOURCE_DIR}/platypus
	${CMAKE_BINARY_DIR}
)

set(srcs
	main.cpp
)
source_group(source FILES ${srcs})

add_executable(${project} WIN32 ${srcs})

target_link_libraries(${project}
#	platypus-proto
	platypus
)

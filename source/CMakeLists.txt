set(project Demo_Game)

add_subdirectory("libraries")

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/libraries/platypus
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

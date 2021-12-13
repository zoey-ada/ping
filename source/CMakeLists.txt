
add_subdirectory(libraries)

include_directories(
	${CMAKE_SOURCE_DIR}
)

set(srcs
	main.cpp
	demoGameLogic.hpp
)

add_executable(Demo_Game WIN32 ${srcs})
link_static(Demo_Game)
warnings_as_errors(Demo_Game)

target_link_libraries(Demo_Game
	platypus-proto
	platypus
)

add_custom_command(
	TARGET Demo_Game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/engine_settings.json
		${bin_dir}/engine_settings.json)


add_subdirectory(libraries)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(srcs
	main.cpp
	demoGameLogic.hpp
	demoGameLogic.cpp

	ai_nodes/action_nodes/trackVerticallyNode.hpp
	ai_nodes/action_nodes/waitNode.hpp
	ai_nodes/decision_nodes/proximityNode.hpp

	entity_components/aiComponent.hpp
	entity_components/aiComponent.cpp
)

add_executable(Demo_Game WIN32 ${srcs})

link_static(Demo_Game)
warnings_as_errors(Demo_Game)

target_link_libraries(Demo_Game
	PRIVATE
		platypus-proto
		platypus
)

if(WITH_COVERAGE)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(Demo_Game
			PRIVATE
				-fprofile-instr-generate -fcoverage-mapping
		)
	elseif(CMAKE_CSS_COMPILER_ID STREQUAL "MSVC")
		target_compile_options(Demo_Game
			PRIVATE
			/fsanitize-coverage=inline-8bit-counters
		)
	endif()
endif()

add_custom_command(
	TARGET Demo_Game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_CURRENT_SOURCE_DIR}/engine_settings.json
	${bin_dir}/engine_settings.json)

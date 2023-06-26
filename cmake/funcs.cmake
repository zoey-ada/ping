function(warnings_as_errors target)
	if(MSVC)
		# Force to always compile with W4
		target_compile_options(${target} PRIVATE /W4 /WX)
	elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		# Update if necessary
		target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -Wpedantic)
	endif()
endfunction()

function(ignore_warnings target)
	get_target_property(type ${target} TYPE)
	if(NOT ${type} STREQUAL "INTERFACE_LIBRARY")
		if(MSVC)
			target_compile_options(${target} PRIVATE /w)
		else()
			target_compile_options(${target} PRIVATE -w)
		endif()
	endif()
endfunction()

function(link_static target)
	set_property(TARGET ${target}
		PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
	)
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
	target_link_options(${target} PRIVATE /NODEFAULTLIB:MSVCRTD)
endfunction()

function(link_dynamic target)
	set_property(TARGET ${target}
		PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>"
	)
endfunction()

function(_dg_compile_hlsl_shader shader_type shader_file out_dir
	__out_custom_target __out_compiled_shader)

	get_filename_component(shader_name ${shader_file} NAME_WLE)
	get_filename_component(shader_file_abs ${shader_file} ABSOLUTE)
	set(compiled_shader "${out_dir}/${shader_name}.cso")

	add_custom_command(
		OUTPUT ${compiled_shader}
		COMMENT "compiling shader: ${shader_name}"

		DEPENDS ${shader_file_abs}

		COMMAND fxc
			/T "${shader_type}"
			/Fo "${compiled_shader}"
			"${shader_file_abs}"
	)

	add_custom_target("generated-${shader_name}"
		DEPENDS ${compiled_shader}
	)

	set(${__out_custom_target} "generated-${shader_name}" PARENT_SCOPE)

	set(${__out_compiled_shader}
		${compiled_shader}

		PARENT_SCOPE
	)
endfunction()

function(_dg_process_texture texture_file out_dir __out_custom_target __out_processed_texture)

	get_filename_component(texture_name ${texture_file} NAME_WLE)
	get_filename_component(texture_file_extension ${texture_file} EXT)
	string(SUBSTRING ${texture_file_extension} 1 -1 texture_file_extension)
	get_filename_component(texture_file_abs ${texture_file} ABSOLUTE)
	set(processed_texture "${out_dir}/${texture_name}.${texture_file_extension}")
	set(target_name "processed-texture-${texture_name}_${texture_file_extension}")

	add_custom_command(
		OUTPUT ${processed_texture}
		COMMENT "processing texture: ${texture_name}"

		DEPENDS ${texture_file}

		COMMAND cmake -E copy "${texture_file_abs}" "${processed_texture}"
	)

	add_custom_target("${target_name}"
		DEPENDS ${processed_texture}
	)

	set(${__out_custom_target} "${target_name}" PARENT_SCOPE)
	set(${__out_processed_entity} ${processed_texture} PARENT_SCOPE)
endfunction()

function(_dg_process_mesh mesh_file out_dir __out_custom_target __out_processed_mesh)

	get_filename_component(mesh_name ${mesh_file} NAME_WLE)
	get_filename_component(mesh_file_extension ${mesh_file} EXT)
	string(SUBSTRING ${mesh_file_extension} 1 -1 mesh_file_extension)
	get_filename_component(mesh_file_abs ${mesh_file} ABSOLUTE)
	set(processed_mesh "${out_dir}/${mesh_name}.${mesh_file_extension}")
	set(target_name "processed-mesh-${mesh_name}_${mesh_file_extension}")

	add_custom_command(
		OUTPUT ${processed_mesh}
		COMMENT "processing mesh: ${mesh_name}"

		DEPENDS ${mesh_file}

		COMMAND cmake -E copy "${mesh_file_abs}" "${processed_mesh}"
	)

	add_custom_target("${target_name}"
		DEPENDS ${processed_mesh}
	)

	set(${__out_custom_target} "${target_name}" PARENT_SCOPE)
	set(${__out_processed_entity} ${processed_mesh} PARENT_SCOPE)
endfunction()

function(_dg_process_entity entity_file out_dir __out_custom_target __out_processed_entity)

	get_filename_component(entity_name ${entity_file} NAME_WLE)
	get_filename_component(entity_file_extension ${entity_file} EXT)
	string(SUBSTRING ${entity_file_extension} 1 -1 entity_file_extension)
	get_filename_component(entity_file_abs ${entity_file} ABSOLUTE)
	set(processed_entity "${out_dir}/${entity_name}.${entity_file_extension}")
	set(target_name "processed-entity-${entity_name}_${entity_file_extension}")

	add_custom_command(
		OUTPUT ${processed_entity}
		COMMENT "processing entity: ${entity_name}"

		DEPENDS ${entity_file}

		COMMAND cmake -E copy "${entity_file_abs}" "${processed_entity}"
	)

	add_custom_target("${target_name}"
		DEPENDS ${processed_entity}
	)

	set(${__out_custom_target} "${target_name}" PARENT_SCOPE)
	set(${__out_processed_entity} ${processed_entity} PARENT_SCOPE)
endfunction()

function(_dg_process_font font_file out_dir __out_custom_target __out_processed_font)
	get_filename_component(font_name ${font_file} NAME_WLE)
	get_filename_component(font_file_extension ${font_file} EXT)
	string(SUBSTRING ${font_file_extension} 1 -1 font_file_extension)
	get_filename_component(font_file_abs ${font_file} ABSOLUTE)
	set(processed_font "${out_dir}/${font_name}.${font_file_extension}")
	set(target_name "processed-font-${font_name}_${font_file_extension}")

	add_custom_command(
		OUTPUT ${processed_font}
		COMMENT "processing font: ${font_name}"

		DEPENDS ${font_file}

		COMMAND cmake -E copy "${font_file_abs}" "${processed_font}"
	)

	add_custom_target("${target_name}"
		DEPENDS ${processed_font}
	)

	set(${__out_custom_target} "${target_name}" PARENT_SCOPE)
	set(${__out_processed_font} ${processed_font} PARENT_SCOPE)
endfunction()

function(_dg_process_sound sound_file out_dir __out_custom_target __out_processed_sound)
	get_filename_component(sound_name ${sound_file} NAME_WLE)
	get_filename_component(sound_file_extension ${sound_file} EXT)
	string(SUBSTRING ${sound_file_extension} 1 -1 sound_file_extension)
	get_filename_component(sound_file_abs ${sound_file} ABSOLUTE)
	set(processed_sound "${out_dir}/${sound_name}.${sound_file_extension}")
	set(target_name "processed-sound-${sound_name}_${sound_file_extension}")

	add_custom_command(
		OUTPUT ${processed_sound}
		COMMENT "processing sound: ${sound_name}"

		DEPENDS ${sound_file}

		COMMAND cmake -E copy "${sound_file_abs}" "${processed_sound}"
	)

	add_custom_target("${target_name}"
		DEPENDS ${processed_sound}
	)

	set(${__out_custom_target} "${target_name}" PARENT_SCOPE)
	set(${__out_processed_sound} ${processed_sound} PARENT_SCOPE)
endfunction()

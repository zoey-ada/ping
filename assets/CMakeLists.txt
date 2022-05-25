#-------------------------------------------------------------------------------
# pixel shaders
#-------------------------------------------------------------------------------
set(pixel_shaders
	shaders/simple_pixel.hlsl
)
set_source_files_properties(${pixel_shaders} PROPERTIES VS_SHADER_TYPE Pixel)
source_group(shaders FILES ${pixel_shaders})

foreach(shader ${pixel_shaders})
	_dg_compile_hlsl_shader(ps_5_0 ${shader} ${bin_dir}/assets/shaders
		custom_target compiled_shader)

	set(compiled_pixel_shaders ${compiled_pixel_shaders} ${compiled_shader})
	set(pixel_shader_targets ${pixel_shader_targets} ${custom_target})
endforeach(shader ${pixel_shaders})

#-------------------------------------------------------------------------------
# vertex shaders
#-------------------------------------------------------------------------------
set(vertex_shaders
	shaders/simple_vertex.hlsl
)
set_source_files_properties(${vertex_shaders} PROPERTIES VS_SHADER_TYPE Vertex)
source_group(shaders FILES ${vertex_shaders})

foreach(shader ${vertex_shaders})
	_dg_compile_hlsl_shader(vs_5_0 ${shader} ${bin_dir}/assets/shaders
		custom_target compiled_shader)

	set(compiled_vertex_shaders ${compiled_vertex_shaders} ${compiled_shader})
	set(vertex_shader_targets ${vertex_shader_targets} ${custom_target})
endforeach(shader ${pixel_shaders})

#-------------------------------------------------------------------------------
# textures
#-------------------------------------------------------------------------------
set(textures
	# textures/cyndaquil.bmp
	textures/cyndaquil.jpg
	textures/platypus.jpg
)
source_group(textures FILES ${textures})

foreach(texture ${textures})
	_dg_process_texture(${texture} ${bin_dir}/assets/textures
		custom_target processed_texture)

	set(processed_textures ${processed_textures} ${processed_texture})
	set(texture_targets ${texture_targets} ${custom_target})
endforeach(texture ${textures})

#-------------------------------------------------------------------------------
# meshes
#-------------------------------------------------------------------------------
set(meshes
	meshes/cube.obj
)
source_group(meshes FILES ${meshes})

foreach(mesh ${meshes})
	_dg_process_mesh(${mesh} ${bin_dir}/assets/meshes
		custom_target processed_mesh)

	set(processed_meshes ${processed_meshes} ${processed_mesh})
	set(mesh_targets ${mesh_targets} ${custom_target})
endforeach(mesh ${meshes})

#-------------------------------------------------------------------------------
# entities
#-------------------------------------------------------------------------------
set(entities
	entities/cube.json
)
source_group(entities FILES ${entities})

foreach(entity ${entities})
	_dg_process_entity(${entity} ${bin_dir}/assets/entities
		custom_target processed_entity)

	set(processed_entities ${processed_entities} ${processed_entity})
	set(entity_targets ${entity_targets} ${custom_target})
endforeach(entity ${entities})

#-------------------------------------------------------------------------------
# archiving
#-------------------------------------------------------------------------------
set(all_assets
	${compiled_pixel_shaders}
	${compiled_vertex_shaders}
	${textures}
	${meshes}
	${entities}
)

add_custom_command(
	OUTPUT ${bin_dir}/assets.zip
	COMMENT INFO "creating assets zip"

	DEPENDS
		${compiled_pixel_shaders}
		${compiled_vertex_shaders}
		${processed_textures}
		${processed_meshes}
		${processed_entities}

	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND 7z a -tzip ${bin_dir}/assets.zip ${bin_dir}/assets/*
)

add_custom_target(assets
	DEPENDS ${bin_dir}/assets.zip
	SOURCES ${all_assets} ${pixel_shaders} ${vertex_shaders}
)

add_dependencies(assets
	${pixel_shader_targets}
	${vertex_shader_targets}
	${texture_targets}
	${mesh_targets}
	${entity_targets}
)

add_dependencies(Demo_Game assets)

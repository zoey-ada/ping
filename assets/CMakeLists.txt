# -------------------------------------------------------------------------------
# pixel shaders
# -------------------------------------------------------------------------------
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

	set_target_properties(${custom_target} PROPERTIES FOLDER assets/pixel_shaders)
endforeach(shader ${pixel_shaders})

# -------------------------------------------------------------------------------
# vertex shaders
# -------------------------------------------------------------------------------
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

	set_target_properties(${custom_target} PROPERTIES FOLDER assets/vertex_shaders)
endforeach(shader ${pixel_shaders})

# -------------------------------------------------------------------------------
# textures
# -------------------------------------------------------------------------------
set(textures
	textures/black.png
	textures/blue.png
	textures/cyndaquil.png
	textures/gray.png
	textures/green_5x5.png
	textures/green_10x10.png
	textures/green.png
	textures/magenta.png
	textures/orange.png
	textures/ping_title.png
	textures/pink.png
	textures/platypus.jpg
	textures/purple.png
	textures/red.png
	textures/teal.png
	textures/white.png
	textures/yellow.png
)
source_group(textures FILES ${textures})

foreach(texture ${textures})
	_dg_process_texture(${texture} ${bin_dir}/assets/textures
		custom_target processed_texture)

	set(processed_textures ${processed_textures} ${processed_texture})
	set(texture_targets ${texture_targets} ${custom_target})

	set_target_properties(${custom_target} PROPERTIES FOLDER assets/textures)
endforeach(texture ${textures})

# -------------------------------------------------------------------------------
# meshes
# -------------------------------------------------------------------------------
set(meshes
	meshes/cube.obj
)
source_group(meshes FILES ${meshes})

foreach(mesh ${meshes})
	_dg_process_mesh(${mesh} ${bin_dir}/assets/meshes
		custom_target processed_mesh)

	set(processed_meshes ${processed_meshes} ${processed_mesh})
	set(mesh_targets ${mesh_targets} ${custom_target})

	set_target_properties(${custom_target} PROPERTIES FOLDER assets/meshes)
endforeach(mesh ${meshes})

# -------------------------------------------------------------------------------
# entities
# -------------------------------------------------------------------------------
set(entities
	entities/ball.json
	entities/bottom_border.json
	entities/left_border.json
	entities/menu_title.json
	entities/p1_goal.json
	entities/p1_paddle.json
	entities/p2_goal.json
	entities/p2_paddle.json
	entities/right_border.json
	entities/top_border.json
)
source_group(entities FILES ${entities})

foreach(entity ${entities})
	_dg_process_entity(${entity} ${bin_dir}/assets/entities
		custom_target processed_entity)

	set(processed_entities ${processed_entities} ${processed_entity})
	set(entity_targets ${entity_targets} ${custom_target})

	set_target_properties(${custom_target} PROPERTIES FOLDER assets/entities)
endforeach(entity ${entities})

# -------------------------------------------------------------------------------
# fonts
# -------------------------------------------------------------------------------
set(fonts
	fonts/Hack-Regular.ttf
)
source_group(fonts FILES ${fonts})

foreach(font ${fonts})
	_dg_process_font(${font} ${bin_dir}/assets/fonts
		custom_target processed_font)

	set(processed_fonts ${processed_fonts} ${processed_font})
	set(font_targets ${font_targets} ${custom_target})

	set_target_properties(${custom_target} PROPERTIES FOLDER assets/fonts)
endforeach(font ${fonts})

# -------------------------------------------------------------------------------
# sounds
# -------------------------------------------------------------------------------
set(sounds
	sounds/pop3.wav
)
source_group(sounds FILES ${sounds})

foreach(sound ${sounds})
	_dg_process_sound(${sound} ${bin_dir}/assets/sounds
		custom_target processed_sound)

	set(processed_sounds ${processed_sounds} ${processed_sound})
	set(sound_targets ${sound_targets} ${custom_target})

	set_target_properties(${custom_target} PROPERTIES FOLDER assets/sounds)
endforeach(sound ${sounds})

# -------------------------------------------------------------------------------
# archiving
# -------------------------------------------------------------------------------
set(all_assets
	${compiled_pixel_shaders}
	${compiled_vertex_shaders}
	${textures}
	${meshes}
	${entities}
	${fonts}
	${sounds}
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
	${processed_fonts}
	${processed_sounds}

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
	${font_targets}
	${sound_targets}
)

add_dependencies(Demo_Game assets)

set_target_properties(assets PROPERTIES FOLDER assets)

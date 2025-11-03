/*
	Based on shaders examples from learnopengl.com
		(https://learnopengl.com/Advanced-Lighting/Advanced-Lighting,
		https://learnopengl.com/Advanced-Lighting/Normal-Mapping,
		https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
		among others)
	by Joey de Vries
	Original licensed under CC-BY-4.0
	
	Modifications and additions licensed under MIT
	Copyright (c) 2025 Paul Jankowski
*/

#version 130
in vec3 position_in;

/*
	discarding alpha frags in shadow map
	part 1:
*/
in vec2 texture_coords_in;

/*
	part 2:
*/
out vec2 texture_coords;

/*
		model
			
			standard model matrix
			(position, rotation, translation)
*/
uniform mat4 model;

/*
		light_space_matrix
		
			multiplied view and
			orthogonal projection
*/
uniform mat4 light_space_matrix;

void main()
{
	gl_Position = light_space_matrix * model * vec4(position_in, 1.0);
	
	//for discarding
	texture_coords = texture_coords_in;
}
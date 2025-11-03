/*
	Based on shader example from learnopengl.com (https://learnopengl.com/Getting-started/Textures)
	by Joey de Vries
	Original licensed under CC-BY-4.0
	
	Modifications licensed under MIT
	Copyright (c) 2025 Paul Jankowski
*/

#version 130
in vec3 position_in;
in vec2 texture_coords_in;

out vec2 texture_coordinates;

void main()
{
    gl_Position = vec4(position_in, 1.0);
	texture_coordinates = texture_coords_in;
}
/*
	Based on shader example from learnopengl.com (https://learnopengl.com/Getting-started/Textures)
	by Joey de Vries
	Original licensed under CC-BY-4.0
	
	Modifications licensed under MIT
	Copyright (c) 2025 Paul Jankowski
*/

#version 130
out vec4 final_color;

in vec2 texture_coordinates;

uniform sampler2D main_texture;

void main()
{
    final_color = texture(main_texture, texture_coordinates);
}
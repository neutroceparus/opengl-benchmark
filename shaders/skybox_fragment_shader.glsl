/*
	Based on shader example from learnopengl.com (https://learnopengl.com/Advanced-OpenGL/Cubemaps)
	by Joey de Vries
	Original licensed under CC-BY-4.0
	
	Modifications licensed under MIT
	Copyright (c) 2025 Paul Jankowski
*/

#version 130
out vec4 final_color;

in vec3 texture_coordinates;

uniform samplerCube skybox;

void main()
{    
    final_color = texture(skybox, texture_coordinates);
}
/*
	Based on shader example from learnopengl.com (https://learnopengl.com/Advanced-OpenGL/Cubemaps)
	by Joey de Vries
	Original licensed under CC-BY-4.0
	
	Modifications licensed under MIT
	Copyright (c) 2025 Paul Jankowski
*/

#version 130
in vec3 position_in;

out vec3 texture_coordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texture_coordinates = position_in;
    vec4 position = projection * view * vec4(position_in, 1.0);
    gl_Position = position.xyww;
}  
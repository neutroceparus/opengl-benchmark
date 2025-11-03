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

/*
		texture_coords
*/
in vec2 texture_coords;

/*
		main_texture
*/
uniform sampler2D main_texture;

void main()
{
	/*
		calculating main texture standard way
	*/
	vec4 texture_first_stage = texture(main_texture, texture_coords);
	
	/*
		if transparent then no need for further
		processing
	*/
	if(texture_first_stage.a < 0.1)
	{
		discard;
	}
	
	//	work of built in variables,
	//	but it anyway is done in background
	//gl_FragDepth = gl_FragCoord.z;
}
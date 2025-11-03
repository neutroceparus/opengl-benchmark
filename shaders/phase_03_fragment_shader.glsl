/*
	Based on shaders examples from learnopengl.com
		(https://learnopengl.com/Advanced-Lighting/Advanced-Lighting,
		https://learnopengl.com/Advanced-Lighting/Normal-Mapping,
		https://learnopengl.com/Advanced-OpenGL/Instancing
		among others)
	by Joey de Vries
	Original licensed under CC-BY-4.0
	
	Modifications and additions licensed under MIT
	Copyright (c) 2025 Paul Jankowski
*/

#version 140 // GLSL 1.4 = OpenGL 3.1

//	normal mapping + instances

/*
	outputs
		
		this goes to renderer
*/
out vec4 final_color;

/*
	inputs
*/
/*
		world_space_fragment_position
*/
in vec3 world_space_fragment_position;

/*
		texture_coords
*/
in vec2 texture_coords;

/*
		light_pos_tangent_space
			
			these are adjusted to surface
			for use with normal map
*/
in vec3 light_pos_tangent_space;

/*
		view_pos_tangent_space
*/
in vec3 view_pos_tangent_space;

/*
		frag_pos_tangent_space
*/
in vec3 frag_pos_tangent_space;

/*
		light_dir_tangent_space
*/
in vec3 light_dir_tangent_space;

/*
	uniforms
*/
/*
		main_texture
*/
uniform sampler2D main_texture;

/*
		normal_map
*/
uniform sampler2D normal_map;

/*
		abstract_light_position
			
			for shadow calculations
*/
uniform vec3 abstract_light_position;

/*
		diffuse_light_color
			
			this lights main texture
*/
uniform vec3 diffuse_light_color;

/*
		ambient_light_color
			
			environment
*/
uniform vec3 ambient_light_color;

/*
		specular_light_color
			
			reflects from surfaces
*/
uniform vec3 specular_light_color;

/*
		general_specular_value
			
			this is for adjusting reflecting
			on surface
			this can be texture also
*/
uniform vec3 general_specular_value;

void main()
{
	/*
		calculating main texture standard way
			
			adjusting to texture moved to end
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
	
	/*
		taking normal from normal map
		and adjusting to from: -1 to: 1
		(in tangent space)
	*/
	vec3 normal = texture(normal_map, texture_coords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	/*
		calculating ambient
	*/
	vec3 ambient_component = ambient_light_color;
	
	/*
		calculating diffuse
			
			this illuminate main texture
			using dot(scalar) product
			light direction is from target to source
			for case < 0 this will be zero,
			meaning completely dark surface
	*/
	float diffuse_strength =
		max(
			dot(
				light_dir_tangent_space,
				normal
				),
			0.0
			);
	
	vec3 diffuse_component =
		diffuse_light_color * diffuse_strength;
	
	/*
		calculating specular
			
			specular is light
			that comply to viewer
			
			reflect calculate reflection
			according to equation in specs
			
			specular strength < 0 is 0
			means no reflection
			
			shininess choosen by trial and error
			to 16
			(this is usually in material properties)
			
			changed to blinn-phong - using halfway_direction
			
			also changed to tangent space
	*/
	vec3 view_direction = normalize(view_pos_tangent_space - frag_pos_tangent_space);
	
	vec3 halfway_direction =
		normalize(light_dir_tangent_space + view_direction);
	
	float specular_strength =
		pow(
			max(
				dot(
					normal,
					halfway_direction
					),
				0.0
				),
			32
			);
	
	vec3 specular_component = specular_light_color * specular_strength * general_specular_value;
	
	/*
		final output - 3 lights added
		this is safe, because values are anyway
		cropped to from 0 to 1
	*/	
	final_color =
		vec4(
				(
					ambient_component
					+
					diffuse_component
					+
					specular_component
				)
				*
				texture_first_stage.rgb,
			//1.0
			texture_first_stage.a//there should be original alpha?
			);
}
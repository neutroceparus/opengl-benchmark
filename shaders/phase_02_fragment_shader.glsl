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

#version 130	// GLSL 1.3 = OpenGL 3.0

/*
	outputs
		
		this goes to renderer
*/
out vec4 final_color;

/*
	inputs
*/
/*
		world_space_normal
*/
in vec3 world_space_normal;

/*
		texture_coords
*/
in vec2 texture_coords;

/*
		frag_pos_light_space
			
			position of fragment in
			light space,
			needed for calculating shadow
*/
in vec4 frag_pos_light_space;

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
		shadow_map
*/
uniform sampler2D shadow_map;

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
		light_direction
		
			from source to target convention
*/
uniform vec3 light_direction;

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

/*
		camera_position
*/
uniform vec3 camera_position;

/*
		shadow_calculation
*/
float shadow_calculation(vec4 frag_pos_light_space)
{
	float shadow;
	//for cases behind far plane of projection
	//we want them not in shadow
	if(frag_pos_light_space.z > 1.0)
	{
		shadow = 0.0;
	}
	else
	{
		/*
			perspective division
		*/
		vec3 projected_coordinates =
			frag_pos_light_space.xyz
			/
			frag_pos_light_space.w;
		
		/*
			transforming to range from 0 to 1
		*/
		projected_coordinates = projected_coordinates * 0.5 + 0.5;
			
		/*
			depth of current fragment from light's perspective
		*/
		float current_depth = projected_coordinates.z;
		
		/*
			shadow bias
			correction of shadow acne
			values choosen by trial and error
			because grass models are small
		*/
		float bias =
			max(
					0.0008
					*
					(
						1.0
						-
						dot(
							normalize(world_space_normal),
							normalize(light_direction)
							)
						),
				0.0008
				);
		
		/*
			percentage close filtering
			
				simple implementation - average of 9 pixels
				
				textureSize - built in function
				gives dimensions of texture
		*/
		shadow = 0.0;
		vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
		
		for(int x = -1; x <= 1; x++)
		{
			for(int y = -1; y <= 1; y++)
			{
				//taking exact point
				float texel_depth =
					texture(
						shadow_map,
						projected_coordinates.xy + vec2(x, y) * texel_size
						).
							r;
				
				/*
					check if current frag position is in shadow
				*/
				shadow += current_depth - bias > texel_depth ? 1.0 : 0.0;
			}
		}
		
		shadow /= 9.0;
	}
	
	return shadow;
}

void main()
{
	/*
		note: multiplying value from 0 to 1
		by other value from 0 to 1
		result is always in range from 0 to 1
	*/
	
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
			16
			);
	
	vec3 specular_component = specular_light_color * specular_strength * general_specular_value;
	
	/*
		final output - 3 lights added
		this is safe, because values are anyway
		cropped to from 0 to 1
	*/
	float shadow = shadow_calculation(frag_pos_light_space);
	
	vec3 adjusted_to_shadow = (1.0 - shadow) * (diffuse_component + specular_component);
	
	final_color =
		vec4(
				(
					ambient_component
					+
					adjusted_to_shadow
				)
				*
				texture_first_stage.rgb,
			//1.0
			texture_first_stage.a//there should be original alpha?
			);
}
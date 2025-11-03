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

/*	
		OFFSET_ADJUSTED_TO_SQRT_2
	
			distance between models
			adjusted because all rectangle should be
			rotated by 45 degrees
			this is probably simplest solution
			(must be also adjusted instance_matrix)
*/
#define OFFSET_ADJUSTED_TO_SQRT_2 0.19

//	normal mapping + instances

/*
	inputs
*/
/*
		position_in
		
			position of vertex as it is
			in 3d model
*/
in vec3 position_in;

/*
		normal_in
			
			perpendicular to "face" of vertex
			as it is in 3d model
*/
in vec3 normal_in;

/*
		texture_coords_in
*/
in vec2 texture_coords_in;

/*
		tangents_in
*/
in vec3 tangent_in;

/*
		bitangents_in
*/
in vec3 bitangent_in;

/*
	outputs
*/
/*
		world_space_fragment_position

			model matrix multiplied by
			position of vertex
*/
out vec3 world_space_fragment_position;

/*
		texture_coords
		
			here just passing it
*/
out vec2 texture_coords;

/*
		light_pos_tangent_space
			
			these are adjusted to surface
			for use with normal map
*/
out vec3 light_pos_tangent_space;

/*
		view_pos_tangent_space
*/
out vec3 view_pos_tangent_space;

/*
		frag_pos_tangent_space
*/
out vec3 frag_pos_tangent_space;

/*
		light_dir_tangent_space
*/
out vec3 light_dir_tangent_space;

/*
	uniforms
*/
/*
		model
		
			position, rotation and scale
			on model in world space
*/
uniform mat4 model;

/*
		view
		
			looks from camera to target
*/
uniform mat4 view;

/*
		projection

			projection to 2d screen
*/
uniform mat4 projection;

/*
		normal_matrix
		
			matrix for adjusting normals in
			world space
			just like model
*/
uniform mat3 normal_matrix;

/*
		abstract_light_position
			
			theese are also in fragment shader
*/
uniform vec3 abstract_light_position;

/*
		light_direction
		
			from source to target convention
*/
uniform vec3 light_direction;

/*
		camera_position
*/
uniform vec3 camera_position;

/*
		num_of_instances
			
			total sum of instances
			(to be avaible to determine position)
*/
uniform int num_of_instances;

/*
		trunc_sqrt_of_instances_plus_1
			
			size of square in which they will be positioned
			in C its:
			(int) (ceil( sqrt((float) num_of_instances) ) + 1);
			to avoid computing it every pass
*/
uniform int ceil_sqrt_of_instances_plus_1;

void main()
{
	/*
		adjusting normals
	*/
	//world_space_normal = normal_matrix * normal_in;
	
	/*
		adjusting fragments
	*/
	
	//row
	int row;
	//column
	int column;
	
	if(gl_InstanceID == 0)
	{
		row = 0;
		column = 0;
	}
	else
	{
		row = gl_InstanceID / ceil_sqrt_of_instances_plus_1;
		column = gl_InstanceID % ceil_sqrt_of_instances_plus_1;
	}
		
	// column major !
	vec4 instance_location =
		vec4(
			-1.0 * (row * OFFSET_ADJUSTED_TO_SQRT_2),
			0.0,
			-1.0 * (column * OFFSET_ADJUSTED_TO_SQRT_2),
			1.0
			);
			
	mat4 instance_rotation =
		mat4(
			cos(radians(-45.0)), 0.0, -sin(radians(-45.0)), 0.0,
			0.0, 1.0, 0.0, 0.0,
			sin(radians(-45.0)), 0.0, cos(radians(-45.0)), 0.0,
			0.0, 0.0, 0.0, 1.0
			);
	
	vec4 instance_translation = instance_rotation * instance_location;
	
	mat4 instance_matrix =
		mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			instance_translation[0],
			instance_translation[1],
			instance_translation[2],
			1.0
			);
	
	vec4 fragment_position_vec4 = instance_matrix * model * vec4(position_in, 1.0);
	
	/*
		passing fragments
	*/
	world_space_fragment_position =
		vec3(fragment_position_vec4);
	
	/*
		build-in variable that puts it on screen
	*/
	gl_Position = projection * view * fragment_position_vec4;
	
	/*
		passing texture coordinates
	*/
	texture_coords = texture_coords_in;
	
	/*
		tangent space
		
			(T)angent (B)itangent (N)ormal
	*/
	vec3 tangent = normalize(normal_matrix * tangent_in);
	
	vec3 normal = normalize(normal_matrix * normal_in);
	
	//  Gram-Schmidt process - re-orthogonalize
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	
	vec3 bitangent = normalize(normal_matrix * bitangent_in);
	
	// TBN matrix
	// here transpose of 3 perpendicular vectors is inversing
	mat3 TBN = transpose(mat3(tangent, bitangent, normal));
	
	// adjusting
	light_pos_tangent_space = TBN * abstract_light_position;
	view_pos_tangent_space = TBN * camera_position;
	frag_pos_tangent_space = TBN * world_space_fragment_position;
	light_dir_tangent_space = TBN * normalize(-1.0 * light_direction);// !
}
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

#version 130 // GLSL 1.3 = OpenGL 3.0

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
		world_space_normal

			passing adjusted for
			normal matrix (from model matrix)
			here only rotation used
			see whats passed to shader
*/
out vec3 world_space_normal;

/*
		texture_coords
		
			here just passing it
*/
out vec2 texture_coords;

/*
		frag_pos_light_space
			
			position of fragment in
			light space,
			needed for calculating shadow
*/
out vec4 frag_pos_light_space;

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
		light_space_matrix
			
			for calculating shadows based
			on shadow map
*/
uniform mat4 light_space_matrix;

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

void main()
{
	/*
		adjusting normals
	*/
	world_space_normal = normal_matrix * normal_in;
	
	/*
		adjusting fragments
	*/
	vec4 fragment_position_vec4 = model * vec4(position_in, 1.0);
	
	/*
		needed in TBN
	*/
	vec3 world_space_fragment_position =
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
		position of fragment in light space
		for adjusting to shadow map
	*/
	frag_pos_light_space = light_space_matrix * fragment_position_vec4;
	
	/*
		tangent space
		
			(T)angent (B)itangent (N)ormal
	*/
	vec3 tangent = normalize(normal_matrix * tangent_in);
	
	vec3 normal = normalize(normal_matrix * normal_in);
	
	//  Gram-Schmidt process - re-orthogonalize
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	
	vec3 bitangent = cross(normal, tangent);
	
	// TBN matrix
	// here transpose of 3 perpendicular vectors is inversing
	mat3 TBN = transpose(mat3(tangent, bitangent, normal));
	
	// adjusting
	light_pos_tangent_space = TBN * abstract_light_position;
	view_pos_tangent_space = TBN * camera_position;
	frag_pos_tangent_space = TBN * world_space_fragment_position;
	light_dir_tangent_space = TBN * normalize(-1.0 * light_direction); // !
}
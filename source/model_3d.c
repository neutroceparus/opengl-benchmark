//	Copyright (c) 2025 Paul Jankowski

#include "model_3d.h"

/*
functions definitions
*/
/*
	initialization
*/
/*
		empty_model_3d_in_gl_30
		
			!! version 3.0 onward
*/
struct model_3d_in_gl_30 empty_model_3d_in_gl_30(void)
{
	struct model_3d_in_gl_30 result;
	
	result.state = MY_FALSE;

	result.name = NULL;
	
	// VAO
	result.vertex_array_obj_id = 0;
	
	// deciced to have everything primary in one
	result.vertex_buffer_obj_id = 0;
	result.num_of_vertices = -1;
	
	// element buffer
	result.element_buffer_obj_id = 0;
	result.num_of_indices = -1;
	
	//texture object
	result.texture_id = 0;
	result.normal_map_id = 0;
	
	//position, roration and scale
	result.model_matrix =
		identity_matrix_4_x_4_float();
	result.normal_matrix =
		identity_matrix_3_x_3_float();

	// face culling adjusted to each model
	result.counter_clock_face_culling_ready = MY_FALSE;
	
	// tangents bitangents
	result.tangents_bitangents_present = MY_FALSE;
	result.tangents_buffer_id = 0;
	result.bitangents_buffer_id = 0;
		
	return result;
}

/*
		empty_generic_vertex
*/
struct generic_vertex empty_generic_vertex(void)
{
	struct generic_vertex result;
	
	result.position = null_column_vector_3_x_1_float();
	result.normals = null_column_vector_3_x_1_float();
	result.texture_coordinates =
		null_column_vector_2_x_1_float();
	
	return result;
}

/*
		empty_model_3d
*/
struct model_3d empty_model_3d(void)
{
	struct model_3d result;
	
	result.state = MY_FALSE;
	
	result.name = NULL;
	
	result.num_of_vertices = -1;
	result.vertices = NULL;
	
	result.num_of_indices_triplets = -1;
	result.indices_triplets = NULL;
	
	result.texture = NULL;
	result.normal_map = NULL;
	
	result.tangents_bitangents_present = MY_FALSE;
	result.tangents = NULL;
	result.bitangents = NULL;
	
	result.legacy_display_list_id = 0;
	
	return result;
}

/*
	cleanup
*/
/*
		deinit_model_3d_in_gl
*/
void deinit_model_3d_in_gl_30(
	struct model_3d_in_gl_30 * model
	)
{
	if(model == NULL)
	{
		simplest_log(
			"deinit_model_3d_in_gl_30 "
			"provided NULL"
			);
	}
	else
	{
		if((*model).state == MY_FALSE)
		{
			simplest_log(
				"deinit_model_3d_in_gl_30 "
				"state is false"
				);
		}
		else
		{
			(*model).state = MY_FALSE;
			
			if((*model).name != NULL)
			{
				free((*model).name);
			}
			
			if((*model).vertex_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &((*model).vertex_buffer_obj_id));
			}
			
			if((*model).element_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &((*model).element_buffer_obj_id));
			}
			
			if((*model).vertex_array_obj_id != 0)
			{
				glDeleteVertexArrays(1, &((*model).vertex_array_obj_id));
			}
			
			if((*model).texture_id != 0)
			{
				glDeleteTextures(1, &((*model).texture_id));
			}
			
			if((*model).normal_map_id != 0)
			{
				glDeleteTextures(1, &((*model).normal_map_id));
			}
			
			if((*model).tangents_bitangents_present == MY_TRUE)
			{
				if((*model).tangents_buffer_id != 0)
				{
					glDeleteBuffers(1, &((*model).tangents_buffer_id));
				}
				
				if((*model).bitangents_buffer_id != 0)
				{
					glDeleteBuffers(1, &((*model).bitangents_buffer_id));
				}
			}
			
			gl_error_tool("deinit_model_3d_in_gl_30");
			
			(*model) = empty_model_3d_in_gl_30();
		}
	}
}

/*
		zero_generic_vertex
*/
void zero_generic_vertex(
	struct generic_vertex * vertex
	)
{
	if(vertex != NULL)
	{
		(*vertex).position = null_column_vector_3_x_1_float();
		(*vertex).normals = null_column_vector_3_x_1_float();
		(*vertex).texture_coordinates =
			null_column_vector_2_x_1_float();
	}
	else
	{
		simplest_log(
			"zero_generic_vertex "
			"provided NULL"
			);
	}
}

/*
		deinit_model_3d
*/
void deinit_model_3d(struct model_3d * model)
{
	if(model ==	NULL)
	{
		simplest_log(
			"deinit_model_3d "
			"provided NULL"
			);
	}
	else
	{
		if((*model).state == MY_FALSE)
		{
			simplest_log(
				"deinit_model_3d "
				"state is false"
				);
		}
		else
		{
			(*model).state = MY_FALSE;
			
			if((*model).name != NULL)
			{
				free((*model).name);
			}
			
			if((*model).num_of_vertices > 0)
			{
				if((*model).vertices != NULL)
				{
					free((*model).vertices);
				}
				(*model).num_of_vertices = -1;
			}
			
			if((*model).num_of_indices_triplets > 0)
			{
				if((*model).indices_triplets != NULL)
				{
					free((*model).indices_triplets);
				}
				(*model).num_of_indices_triplets = -1;
			}
			
			if((*model).texture != NULL)
			{
				SDL_FreeSurface((*model).texture);
			}
			
			if((*model).normal_map != NULL)
			{
				SDL_FreeSurface((*model).normal_map);
			}
			
			if((*model).legacy_display_list_id != 0)
			{
				//delete it
			}
		}
	}
}

/*
	debug
*/
/*
		dump_generic_vertex_to_log
*/
void dump_generic_vertex_to_simplest_log(
	struct generic_vertex * vertex
	)
{
	if(vertex != NULL)
	{
		simplest_log("dump_generic_vertex_to_simplest_log:");
		simplest_log("position:");
		
		print_column_vector_3_x_1_float_to_simplest_log(
			(*vertex).position
			);
			
		simplest_log("normals:");
		
		print_column_vector_3_x_1_float_to_simplest_log(
			(*vertex).normals
			);
		
		simplest_log("texture_coordinates:");
		
		print_column_vector_2_x_1_float_to_simplest_log(
			(*vertex).texture_coordinates
			);
	}
	else
	{
		simplest_log(
			"dump_generic_vertex_to_simplest_log "
			"provided NULL"
			);
	}
}

/*
		dump_model_3d_to_simplest_log
*/
void dump_model_3d_to_simplest_log(struct model_3d * model)
{
	if(model ==	NULL)
	{
		simplest_log(
			"dump_model_3d_to_simplest_log "
			"provided NULL"
			);
	}
	else
	{
		int i;
		
		simplest_log(
			"dump_model_3d_to_simplest_log: %p",
			model
			);
		
		simplest_log("state: %d", (*model).state);
		
		if((*model).name != NULL)
		{
			simplest_log("name: %s", (*model).name);
		}
		
		simplest_log("num_of_vertices: %d", (*model).num_of_vertices);
		
		if((*model).vertices != NULL)
		{
			for(i = 0; i < (*model).num_of_vertices; i++)
			{
				simplest_log("vertex %d",i);
				dump_generic_vertex_to_simplest_log(
					(*model).vertices + i
					);
			}
		}
		
		simplest_log(
			"num_of_indices_triplets: %d",
			(*model).num_of_indices_triplets
			);
		
		if((*model).indices_triplets != NULL)
		{
			for(i = 0; i < (*model).num_of_indices_triplets; i++)
			{
				simplest_log("indices triplet %d",i);
				print_column_vector_3_x_1_uint_to_simplest_log(
					*((*model).indices_triplets + i)
					);
			}
		}
		
		if((*model).texture != NULL)
		{
			simplest_log(
				"texture present: %p",
				(*model).texture
				);
		}
	}
}

/*
		dump_model_3d_to_txt_variant_00
*/
void dump_model_3d_to_txt_variant_00(
	struct model_3d * model,
	const char * file_name
	)
{
	if(model != NULL && file_name != NULL)
	{
		FILE * file = fopen(file_name, "w");
		
		if(file != NULL)
		{
			fprintf(
				file,
				"model: "
				);
			
			if((*model).name != NULL)
			{
				fprintf(
				file,
				"%s",
				(*model).name
				);
			}
			
			fprintf(
				file,
				"; state: %d",
				(*model).state
				);
			
			fprintf(
				file,
				"\n num_of_vertices: %d\n",
				(*model).num_of_vertices
				);
			
			int i;
			for(i = 0; i < (*model).num_of_vertices; i++)
			{
				fprintf(
				file,
				"vert %d:\t%f %f %f\t%f %f %f\t%f %f\n",
				i,
				(*((*model).vertices + i)).position.by_index[0],
				(*((*model).vertices + i)).position.by_index[1],
				(*((*model).vertices + i)).position.by_index[2],
				(*((*model).vertices + i)).normals.by_index[0],
				(*((*model).vertices + i)).normals.by_index[1],
				(*((*model).vertices + i)).normals.by_index[2],
				(*((*model).vertices + i)).texture_coordinates.by_index[0],
				(*((*model).vertices + i)).texture_coordinates.by_index[1]
				);
			}
			
			fprintf(
				file,
				"\n num_of_indices_triplets: %d\n",
				(*model).num_of_indices_triplets
				);
			
			for(i = 0; i < (*model).num_of_indices_triplets; i++)
			{
				fprintf(
				file,
				"ind_triplet %d:\t%u %u %u\n",
				i,
				(*((*model).indices_triplets + i)).by_index[0],
				(*((*model).indices_triplets + i)).by_index[1],
				(*((*model).indices_triplets + i)).by_index[2]
				);
			}
			
			fclose(file);
		}
	}
}

/*
	preparation
*/
/*
		put_model_to_gl_30
*/
struct model_3d_in_gl_30 put_model_to_gl_30(
	struct model_3d * model,
	union column_vector_3_x_1_float * position_or_null,
	float * rotation_around_y_axis_in_degrees_or_null,
	int counter_clock_face_culling_readiness,
	int include_tangents_and_bitangents
	)
{
	enum model_to_gl_30_checklist
	{
		initial_value = 0,
		constraints_ok,
		name_ok,
		gl_ok,
		buffers_created,
		vertex_buffer_ok,
		element_buffer_ok,
		texture_ok,
		tangents_bitangents_ok,
		normal_map_ok
	} model_to_gl_30_checklist =
		initial_value;
		
	struct model_3d_in_gl_30 result =
		empty_model_3d_in_gl_30();
		
	if(model_to_gl_30_checklist == initial_value)
	{
		if(model != NULL)
		{
			if((*model).state == MY_TRUE)
			{			
				model_to_gl_30_checklist =
					constraints_ok;
			}
			else
			{
				simplest_log(
					"put_model_to_gl_30 "
					"model is not true"
					);
			}
		}
		else
		{
			simplest_log(
				"put_model_to_gl_30 "
				"model is NULL"
				);
		}
	}
	
	if(model_to_gl_30_checklist == constraints_ok)
	{
		if((*model).name != NULL)
		{
			result.name =
				(char *)
					calloc(
						strlen((*model).name) + 1,
						sizeof(char)
						);
			
			if(result.name != NULL)
			{
				strcpy(result.name, (*model).name);
				
				model_to_gl_30_checklist = name_ok;
			}
			else
			{
				simplest_log(
					"put_model_to_gl_30 "
					"cannot calloc name"
					);
			}
		}
		else // no name no problem
		{
			model_to_gl_30_checklist = name_ok;
		}
	}
	
	if(model_to_gl_30_checklist == name_ok)
	{		
		if(
			#if defined _WIN32
			opengl_version_this_context()
			#else
			general_opengl_version()
			#endif
			>=
			30
			)
		{
			if(//temp
				all_gl_functions_present_by_names(
					18,
					"glGetError", // 1.0
					"glTexParameteri",
					"glBindTexture", // 1.1
					"glDeleteTextures",
					"glGenTextures",
					"glBindBuffer", // 1.5
					"glBufferData",
					"glDeleteBuffers",
					"glGenBuffers",
					"glEnableVertexAttribArray", // 2.0
					"glGetUniformLocation",
					"glUniform1i",
					"glUseProgram",
					"glVertexAttribPointer",
					"glBindVertexArray", // 3.0
					"glDeleteVertexArrays",
					"glGenerateMipmap",
					"glGenVertexArrays"
					)
				==
				MY_TRUE
				)
			{
				model_to_gl_30_checklist = gl_ok;
			}
			else
			{
				simplest_log(
					"put_model_to_gl_30 "
					"not all required functions present"
					);
			}
		}
		else
		{
			simplest_log(
				"put_model_to_gl_30 "
				"wrong opengl version"
				);
		}
	}
	
	if(model_to_gl_30_checklist == gl_ok)
	{
		glGenVertexArrays(
			1,
			&(result.vertex_array_obj_id)
			);
		
		glGenBuffers(1, &(result.vertex_buffer_obj_id));
				
		glGenBuffers(1, &(result.element_buffer_obj_id));
		
		if(gl_error_tool("put_model_to_gl_30 create buffers") == GL_NO_ERROR)
		{
			model_to_gl_30_checklist = buffers_created;
		}
		else
		{
			simplest_log(
				"put_model_to_gl_30 "
				"cannot create buffers"
				);
				
			if(result.element_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.element_buffer_obj_id));
			}
			
			if(result.vertex_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.vertex_buffer_obj_id));
			}
			
			if(result.vertex_array_obj_id != 0)
			{
				glDeleteVertexArrays(1, &(result.vertex_array_obj_id));
			}
			
			gl_error_tool("put_model_to_gl_30 cleanup buffers");
		}
	}
	
	if(model_to_gl_30_checklist == buffers_created)
	{
		//vertex array
		glBindVertexArray(result.vertex_array_obj_id);
		
		//vertices
		glBindBuffer(
			GL_ARRAY_BUFFER,
			result.vertex_buffer_obj_id
			);

		glBufferData(
			GL_ARRAY_BUFFER, 
			sizeof(struct generic_vertex) * (*model).num_of_vertices,
			(*model).vertices,
			GL_STATIC_DRAW
			);
		
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(struct generic_vertex),
			(void *) 0
			);
		
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(struct generic_vertex),
			(void *) (sizeof(union column_vector_3_x_1_float))
			);
		
		glEnableVertexAttribArray(1);
		
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(struct generic_vertex),
			(void *) (2 * sizeof(union column_vector_3_x_1_float))
			);
		
		glEnableVertexAttribArray(2);
		
		if(gl_error_tool("put_model_to_gl_30 filling vertex buffer") == GL_NO_ERROR)
		{
			model_to_gl_30_checklist = vertex_buffer_ok;
		}
		else
		{
			simplest_log(
				"put_model_to_gl_30 "
				"error while filling vertex buffer"
				);
		}
	}
	
	if(model_to_gl_30_checklist == vertex_buffer_ok)
	{
		//indices
		glBindBuffer(
			GL_ELEMENT_ARRAY_BUFFER,
			result.element_buffer_obj_id
			);
		
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
				sizeof(union column_vector_3_x_1_uint)
				*
				(*model).num_of_indices_triplets,
			(*model).indices_triplets,
			GL_STATIC_DRAW
			);
		
		if(gl_error_tool("put_model_to_gl_30 filling element buffer") == GL_NO_ERROR)
		{
			result.num_of_indices =
				(*model).num_of_indices_triplets * 3;
			
			model_to_gl_30_checklist = element_buffer_ok;
		}
		else
		{
			simplest_log(
				"put_model_to_gl_30 "
				"error while filling element buffer"
				);
		}
	}
	
	if(model_to_gl_30_checklist == element_buffer_ok)
	{
		// because in opengl it from lower left
		// and also format
		SDL_Surface * temp = SDL_ConvertSurfaceFormat(
			(*model).texture,
			SDL_PIXELFORMAT_RGBA8888,
			0
			);
		
		if(temp != NULL)
		{
			flip_continuous_2d_int_table_vertically(
				(int *) (*temp).pixels,
				(*temp).w,
				(*temp).h
				);
		
			glGenTextures(1, &(result.texture_id));
			
			glBindTexture(GL_TEXTURE_2D, result.texture_id);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				(GLsizei) (*( (*model).texture )).w,
				(GLsizei) (*( (*model).texture )).h,
				0,
				GL_RGBA,
				GL_UNSIGNED_INT_8_8_8_8,
				(const void *) (*temp).pixels
				);
			
			glGenerateMipmap(GL_TEXTURE_2D);
			
			if(gl_error_tool("put_model_to_gl_30 texture") == GL_NO_ERROR)
			{
				model_to_gl_30_checklist = texture_ok;
			}
			else
			{
				if(result.texture_id != 0)
				{
					glDeleteTextures(1, &(result.texture_id));
					gl_error_tool("put_model_to_gl_30 texture cleanup");
				}
				
				simplest_log(
					"put_model_to_gl_30 "
					"error while making texture"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"put_model_to_gl_30 "
				"cannot duplicate for vertical flipping"
				);
		}
	}
	
	if(model_to_gl_30_checklist == texture_ok)
	{
		if(include_tangents_and_bitangents == MY_TRUE)
		{
			int calculated = MY_FALSE;
			
			if((*model).tangents_bitangents_present == MY_TRUE)
			{
				calculated = MY_TRUE;
			}
			else
			{
				if(
					model_3d_calculate_tangents_bitangents(
						model
						)
					==
					MY_TRUE
					)
				{
					calculated = MY_TRUE;
				}
				else
				{
					simplest_log(
						"put_model_to_gl_30 "
						"requested including tangents bitangents "
						"but computation failed"
						);
				}
			}
			
			if(calculated == MY_TRUE)
			{
				glGenBuffers(1, &(result.tangents_buffer_id));
				
				glGenBuffers(1, &(result.bitangents_buffer_id));
				
				glBindBuffer(
					GL_ARRAY_BUFFER,
					result.tangents_buffer_id
					);
		
				glBufferData(
					GL_ARRAY_BUFFER, 
						sizeof(union column_vector_3_x_1_float)
						*
						(*model).num_of_vertices,
					(*model).tangents,
					GL_STATIC_DRAW
					);
				
				glVertexAttribPointer(
					3,
					3,
					GL_FLOAT,
					GL_FALSE,
					0,
					(void *) 0
					);
				
				glEnableVertexAttribArray(3);
				
				glBindBuffer(
					GL_ARRAY_BUFFER,
					result.bitangents_buffer_id
					);
		
				glBufferData(
					GL_ARRAY_BUFFER, 
						sizeof(union column_vector_3_x_1_float)
						*
						(*model).num_of_vertices,
					(*model).bitangents,
					GL_STATIC_DRAW
					);
				
				glVertexAttribPointer(
					4,
					3,
					GL_FLOAT,
					GL_FALSE,
					0,
					(void *) 0
					);
				
				glEnableVertexAttribArray(4);
				
				if(gl_error_tool("tangents bitqangents buffers") == GL_NO_ERROR)
				{
					result.tangents_bitangents_present = MY_TRUE;
					
					model_to_gl_30_checklist =
						tangents_bitangents_ok;
				}
				else
				{
					simplest_log(
						"put_model_to_gl_30 "
						"error while making "
						"tangents bitangents buffers"
						);
					
					if(result.tangents_buffer_id > 0)
					{
						glDeleteBuffers(1, &(result.tangents_buffer_id));
					}
					
					if(result.bitangents_buffer_id > 0)
					{
						glDeleteBuffers(1, &(result.bitangents_buffer_id));
					}
				}
			}
		}
		else // no tangents no problem
		{
			model_to_gl_30_checklist =
				tangents_bitangents_ok;
		}
	}	
	
	if(model_to_gl_30_checklist == tangents_bitangents_ok)
	{
		if((*model).normal_map != NULL)
		{
			SDL_Surface * temp = SDL_ConvertSurfaceFormat(
				(*model).normal_map,
				SDL_PIXELFORMAT_RGBA8888,
				0
				);
			
			if(temp != NULL)
			{
				flip_continuous_2d_int_table_vertically(
					(int *) (*temp).pixels,
					(*temp).w,
					(*temp).h
					);
					
				glGenTextures(1, &(result.normal_map_id));
				
				glBindTexture(GL_TEXTURE_2D, result.normal_map_id);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGBA,
					(GLsizei) (*( (*model).normal_map )).w,
					(GLsizei) (*( (*model).normal_map )).h,
					0,
					GL_RGBA,
					GL_UNSIGNED_INT_8_8_8_8,
					(const void *) (*temp).pixels
					);
				
				glGenerateMipmap(GL_TEXTURE_2D);
				if(gl_error_tool("put_model_to_gl_30 normal map") == GL_NO_ERROR)
				{
					model_to_gl_30_checklist = normal_map_ok;
				}
				else
				{
					if(result.normal_map_id != 0)
					{
						glDeleteTextures(1, &(result.normal_map_id));
						gl_error_tool("put_model_to_gl_30 normal map cleanup");
					}
					
					simplest_log(
						"put_model_to_gl_30 "
						"error while making normal map"
						);
				}
				
				SDL_FreeSurface(temp);
			}
			else
			{
				simplest_log(
					"put_model_to_gl_30 "
					"normal map "
					"cannot duplicate for vertical flipping"
					);
			}
		}
		else //no map no problem
		{
			model_to_gl_30_checklist =
				normal_map_ok;
		}
	}	
	
	if(model_to_gl_30_checklist == normal_map_ok)
	{
		if(position_or_null != NULL)
		{
			result.model_matrix =
				translate_matrix_4_x_4_by_column_vector_3_float(
					result.model_matrix,
					(*position_or_null)
					);
		}
		
		if(rotation_around_y_axis_in_degrees_or_null != NULL)
		{
			result.model_matrix =
				rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
					result.model_matrix,
					*rotation_around_y_axis_in_degrees_or_null,
					column_vector_3_x_1_float_direct_values(0, 1, 0)
					);
		}
		
		if(
			position_or_null != NULL
			||
			rotation_around_y_axis_in_degrees_or_null != NULL
			)
		{
			union matrix_3_x_3_float inverse;
			if(
				inverse_of_matrix_3_x_3_float(
					&inverse,
					matrix_3_x_3_float_from_4_x_4_upper_left(
						result.model_matrix
						)
					)
				==
				MY_TRUE
				)
			{
				result.normal_matrix =
					matrix_3_x_3_float_transpose(
						inverse
						);
			}
			else
			{
				simplest_log(
					"put_model_to_gl_30 "
					"cannot inverse matrix "
					"to make normal matrix, "
					"proceed, leaving identity"
					);
			}
			
		}
		
		result.counter_clock_face_culling_ready =
			counter_clock_face_culling_readiness;
			
		result.state = MY_TRUE;
		
		return result;
	}
	else
	{
		if(model_to_gl_30_checklist >= normal_map_ok)
		{
			if(result.normal_map_id != 0)
			{
				glDeleteTextures(1, &(result.normal_map_id));
			}
		}

		if(model_to_gl_30_checklist >= tangents_bitangents_ok)
		{
			if(result.tangents_buffer_id > 0)
			{
				glDeleteBuffers(1, &(result.tangents_buffer_id));
			}
			
			if(result.bitangents_buffer_id > 0)
			{
				glDeleteBuffers(1, &(result.bitangents_buffer_id));
			}
		}
		
		if(model_to_gl_30_checklist >= texture_ok)
		{
			if(result.texture_id != 0)
			{
				glDeleteTextures(1, &(result.texture_id));
			}
		}
			
		if(model_to_gl_30_checklist >= buffers_created)
		{
			if(result.element_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.element_buffer_obj_id));
			}
			
			if(result.vertex_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.vertex_buffer_obj_id));
			}
			
			if(result.vertex_array_obj_id != 0)
			{
				glDeleteVertexArrays(1, &(result.vertex_array_obj_id));
			}
		}
		
		gl_error_tool("put_model_to_gl_30 cleanup");
		
		if(model_to_gl_30_checklist >= name_ok)
		{
			if(result.name != NULL)
			{
				free(result.name);
			}
		}
		
		return empty_model_3d_in_gl_30();
	}
}

/*
	rendering
*/
/*
		render_model_3d_in_gl_30_cur_shader
			
			assuming shader in use
			and uniforms set
			(except model and normal)
*/
int render_model_3d_in_gl_30_cur_shader(
	struct model_3d_in_gl_30 * model,
	struct shader_in_gl_30 * shader
	)
{
	if(model != NULL && shader != NULL)
	{
		if((*shader).model_uniform_loc > 0)
		{
			glUniformMatrix4fv(
				(*shader).model_uniform_loc,
				1,
				GL_TRUE, // transpose
				(const float *) &((*model).model_matrix)
				);
		}
			
		if((*shader).normal_matrix_uniform_loc > 0)
		{
			glUniformMatrix3fv(
				(*shader).normal_matrix_uniform_loc,
				1,
				GL_TRUE, // transpose
				(const float *) &((*model).normal_matrix)
				);
		}

		//culling
		if((*model).counter_clock_face_culling_ready == MY_TRUE)
		{
			glEnable(GL_CULL_FACE);
		}
	
		//texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, (*model).texture_id);

		//normal map
		if((*model).normal_map_id > 0)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, (*model).normal_map_id);
		}

		//vertices
		glBindVertexArray((*model).vertex_array_obj_id);

		glDrawElements(
			GL_TRIANGLES,
			(*model).num_of_indices,
			GL_UNSIGNED_INT,
			0
			);

		//culling
		if((*model).counter_clock_face_culling_ready == MY_TRUE)
		{
			glDisable(GL_CULL_FACE);
		}

		if(
			gl_error_tool("render_model_3d_in_gl_30")
			==
			GL_NO_ERROR
			)
		{
			return MY_TRUE;
		}
		else
		{
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"render_model_3d_in_gl_30 "
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}

/*
		render_model_3d_instanced_cur_shader
			
			assuming shader in use
			and uniforms set
			(except model and normal)
			(and num_of_instances)
			(and trunc_sqrt_of_instances_plus_1)
*/
int render_model_3d_instanced_cur_shader(
	struct model_3d_in_gl_30 * model,
	struct shader_in_gl_30 * shader,
	int instances
	)
{
	if(model != NULL && shader != NULL && instances >= 1)
	{
		if((*shader).model_uniform_loc > 0)
		{
			glUniformMatrix4fv(
				(*shader).model_uniform_loc,
				1,
				GL_TRUE, // transpose
				(const float *) &((*model).model_matrix)
				);
		}
			
		if((*shader).normal_matrix_uniform_loc > 0)
		{
			glUniformMatrix3fv(
				(*shader).normal_matrix_uniform_loc,
				1,
				GL_TRUE, // transpose
				(const float *) &((*model).normal_matrix)
				);
		}
		
		//culling
		if((*model).counter_clock_face_culling_ready == MY_TRUE)
		{
			glEnable(GL_CULL_FACE);
		}
	
		//texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, (*model).texture_id);

		//normal map
		if((*model).normal_map_id > 0)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, (*model).normal_map_id);
		}

		//vertices
		glBindVertexArray((*model).vertex_array_obj_id);

		glDrawElementsInstanced(
			GL_TRIANGLES,
			(*model).num_of_indices,
			GL_UNSIGNED_INT,
			0,
			instances
			);

		//culling
		if((*model).counter_clock_face_culling_ready == MY_TRUE)
		{
			glDisable(GL_CULL_FACE);
		}

		if(
			gl_error_tool("render_model_3d_instanced_cur_shader")
			==
			GL_NO_ERROR
			)
		{
			return MY_TRUE;
		}
		else
		{
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"render_model_3d_instanced_cur_shader "
			"provided NULL(s) or instances <= 0"
			);
		return MY_FALSE;
	}
}

/*
		render_model_3d_legacy
			
			OpenGL 1.0
*/	
int render_model_3d_legacy(
	struct model_3d * model
	)
{
	if(model != NULL && (*model).state == MY_TRUE)
	{
		if((*model).texture != NULL)
		{
			SDL_Surface * converted_and_flipped;
			
			if(get_endianness() == ENDIANNES_LITTLE)
			{
				converted_and_flipped =
					SDL_ConvertSurfaceFormat(
						(*model).texture,
						SDL_PIXELFORMAT_ABGR8888,// little endian, ABGR !!
						0
						);
			}
			else if(get_endianness() == ENDIANNES_BIG)
			{
				converted_and_flipped =
					SDL_ConvertSurfaceFormat(
						(*model).texture,
						SDL_PIXELFORMAT_RGBA8888,// big endian
						0
						);
			}
			else
			{
				simplest_log(
					"render_model_3d_legacy "
					"endiannesses other than big or little not supported"
					);
				return MY_FALSE;
			}
			
			if(converted_and_flipped != NULL)
			{
				flip_continuous_2d_int_table_vertically(
					(int *) (*(converted_and_flipped)).pixels,
					(*(converted_and_flipped)).w,
					(*(converted_and_flipped)).h
					);
			}
			else
			{
				simplest_log(
					"render_legacy_overlay_2_values "
					"cannot convert and flip"
					);
				return MY_FALSE;
			}
			
			
			
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				(GLsizei) (*(converted_and_flipped)).w,
				(GLsizei) (*(converted_and_flipped)).h,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				(const void *) (*(converted_and_flipped)).pixels
				);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		if((*model).legacy_display_list_id != 0)
		{
			glCallList((*model).legacy_display_list_id);
		}
		else
		{
			glBegin(GL_TRIANGLES);
			
			int i, j;
			for(i = 0; i < (*model).num_of_indices_triplets; i++)
			{
				for(j = 0; j < 3; j++)
				{
					struct generic_vertex * cur_vertex =
						(*model).vertices
						+
						( (* ((*model).indices_triplets + i) ).by_index[j] );
					
					// tex coords and normals must be before vertex coords
					glTexCoord4f(
						(*cur_vertex).texture_coordinates.by_index[0],
						(*cur_vertex).texture_coordinates.by_index[1],
						0.0,
						1.0
						);
						
					glNormal3f(
						(*cur_vertex).normals.by_index[0],
						(*cur_vertex).normals.by_index[1],
						(*cur_vertex).normals.by_index[2]
						);
						
					glEdgeFlag(GL_TRUE);//for a case
					
					glVertex4f(
						(*cur_vertex).position.by_index[0],
						(*cur_vertex).position.by_index[1],
						(*cur_vertex).position.by_index[2],
						1.0
						);
				}
			}
			
			glEnd();
		}
		
		//deinit texture - important
		if((*model).texture != NULL)
		{
			int dummy = -1;
			
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				0,
				0,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				(const void *) &dummy
				);
		}
		
		if(
			gl_error_tool("render_model_3d_legacy")
			==
			GL_NO_ERROR
			)
		{
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"render_model_3d_legacy"
				"gl error"
				);
			return MY_FALSE;
		}
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"render_model_3d_legacy"
			"provided NULL or state is false"
			);
		return MY_FALSE;
	}
}

/*
		render_model_3d_wireframe_legacy
*/
int render_model_3d_wireframe_legacy(
	struct model_3d * model
	)
{
	if(model != NULL && (*model).state == MY_TRUE)
	{
		int dummy = -1;
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			0,
			0,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			(const void *) &dummy
			);
		
		glColor4f(1, 1, 1, 1);
		
		if((*model).legacy_display_list_id != 0)
		{
			glCallList((*model).legacy_display_list_id);
		}
		else
		{
			glBegin(GL_TRIANGLES);
			
			int i, j;
			for(i = 0; i < (*model).num_of_indices_triplets; i++)
			{
				for(j = 0; j < 3; j++)
				{
					struct generic_vertex * cur_vertex =
						(*model).vertices
						+
						( (* ((*model).indices_triplets + i) ).by_index[j] );
					
					// tex coords and normals must be before vertex coords
					glTexCoord4f(
						(*cur_vertex).texture_coordinates.by_index[0],
						(*cur_vertex).texture_coordinates.by_index[1],
						0.0,
						1.0
						);
						
					glNormal3f(
						(*cur_vertex).normals.by_index[0],
						(*cur_vertex).normals.by_index[1],
						(*cur_vertex).normals.by_index[2]
						);
						
					glEdgeFlag(GL_TRUE);//for a case
					
					glVertex4f(
						(*cur_vertex).position.by_index[0],
						(*cur_vertex).position.by_index[1],
						(*cur_vertex).position.by_index[2],
						1.0
						);
				}
			}
			
			glEnd();
		}
		
		if(
			gl_error_tool("render_model_3d_wireframe_legacy")
			==
			GL_NO_ERROR
			)
		{
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"render_model_3d_wireframe_legacy"
				"gl error"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"render_model_3d_wireframe_legacy"
			"provided NULL or state is false"
			);
		return MY_FALSE;
	}
}

/*
	utils
*/
/*
		model_3d_append_textures
		
			to already loaded
*/
int model_3d_append_textures(
	struct model_3d * model,
	char * path_to_texture_file,
	char * path_to_normal_map_file_or_null
	)
{
	enum model_append_textures
	{
		initial_value = 0,
		constraints_ok,
		texture_ok,
		normal_map_ok
	} model_append_textures = initial_value;
	
	if(model_append_textures == initial_value)
	{
		if(
			model != NULL
			&&
			path_to_texture_file != NULL
			)
		{
			if((*model).state == MY_TRUE)
			{
				if((*model).texture == NULL)
				{
					model_append_textures =
						constraints_ok;
				}
				else
				{
					simplest_log(
						"model_3d_append_textures "
						"seems that textures are already present"
						);
				}
			}
			else
			{
				simplest_log(
					"model_3d_append_textures "
					"state is not true"
					);
			}
		}
		else
		{
			simplest_log(
				"model_3d_append_textures "
				"provided NULL"
				);
		}
	}
	
	if(model_append_textures == constraints_ok)
	{
		SDL_Surface * temp =
			IMG_Load(
				path_to_texture_file
				);
		
		if(temp != NULL)
		{
			(*model).texture =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if((*model).texture != NULL)
			{
				model_append_textures =
					texture_ok;
			}
			else
			{
				simplest_log(
					"model_3d_append_textures "
					"cannot convert texture %s, "
					"nothing changed",
					path_to_texture_file
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"model_3d_append_textures "
				"cannot load texture %s, "
				"nothing changed",
				path_to_texture_file
				);
		}
	}
	
	if(model_append_textures == texture_ok)
	{
		if(path_to_normal_map_file_or_null != NULL)
		{
			SDL_Surface * temp =
				IMG_Load(
					path_to_normal_map_file_or_null
					);
			
			if(temp != NULL)
			{
				(*model).normal_map =
					SDL_ConvertSurfaceFormat(
						temp,
						SDL_PIXELFORMAT_RGBA8888,
						0
						);
				
				if((*model).normal_map != NULL)
				{
					model_append_textures =
						normal_map_ok;
				}
				else
				{
					simplest_log(
						"model_3d_append_textures "
						"cannot convert normal map %s, "
						"nothing changed",
						path_to_normal_map_file_or_null
						);
				}
				
				SDL_FreeSurface(temp);
			}
			else
			{
				simplest_log(
					"model_3d_append_textures "
					"cannot load normal map %s, "
					"nothing changed",
					path_to_normal_map_file_or_null
					);
			}
		}
		else
		{//no map no problem
			model_append_textures = normal_map_ok;
		}
	}
	
	if(model_append_textures == normal_map_ok)
	{
		return MY_TRUE;
	}
	else
	{
		if(model_append_textures >= texture_ok)
		{
			SDL_FreeSurface((*model).texture);
		}
		
		return MY_FALSE;
	}
}

/*
		model_3d_uniform_scale
			
			this can be done also with scaling matrix
			but here for simplicity is only
			scalar multiplication
*/
int model_3d_uniform_scale(
	struct model_3d * model,
	float factor
	)
{
	if(model == NULL)
	{
		simplest_log(
			"model_3d_uniform_scale "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	int i;
	
	if((*model).num_of_vertices > 0)
	{
		for(i = 0; i < (*model).num_of_vertices; i++)
		{
			(*((*model).vertices + i)).
				position.
					by_index[0] *=
						factor;
						
			(*((*model).vertices + i)).
				position.
					by_index[1] *=
						factor;
						
			(*((*model).vertices + i)).
				position.
					by_index[2] *=
						factor;
		}
	}
	
	return MY_TRUE;
}

/*
		model_3d_calculate_tangents_bitangents
			
			this approach choosen
			
			based on from learnopnegl
			and opengl-tutorial.org
*/	
int model_3d_calculate_tangents_bitangents(
	struct model_3d * model
	)
{
	enum tangents_bitangents_checklist
	{
		initial_value = 0,
		constraints_ok,
		helper_table_ok,
		tables_calloced,
		calculations_done
	} tangents_bitangents_checklist =
		initial_value;
	
	//not most efficient approach,
	//but one of the simplest
	unsigned int * how_many_of_each_index = NULL;
	
	int i;
	
	union column_vector_3_x_1_float triangle_edge_0_to_1 =
		null_column_vector_3_x_1_float();
	union column_vector_3_x_1_float triangle_edge_0_to_2 =
		null_column_vector_3_x_1_float();
	
	union column_vector_2_x_1_float delta_uv_1_to_0 =
		null_column_vector_2_x_1_float();
	union column_vector_2_x_1_float delta_uv_2_to_0 =
		null_column_vector_2_x_1_float();
	
	union column_vector_3_x_1_float cur_tangent =
		null_column_vector_3_x_1_float();
	union column_vector_3_x_1_float cur_bitangent =
		null_column_vector_3_x_1_float();
	
	float multiplier = 1;
		
	if(tangents_bitangents_checklist == initial_value)
	{
		if(model != NULL)
		{
			if(
				(*model).num_of_vertices > 0
				&&
				(*model).num_of_indices_triplets > 0
				)
			{
				if((*model).tangents_bitangents_present == MY_FALSE)
				{
					tangents_bitangents_checklist =
						constraints_ok;
				}
				else if((*model).tangents_bitangents_present == MY_TRUE)
				{
					simplest_log(
						"model_3d_calculate_tangents_bitangents "
						"tangents bitangents already present"
						);
				}
				else
				{
					simplest_log(
						"model_3d_calculate_tangents_bitangents "
						"model corrupted"
						);
				}
			}
			else
			{
				simplest_log(
					"model_3d_calculate_tangents_bitangents "
					"must be at least one vertex and at least "
					"one indices triplet"
					);
			}
		}
		else
		{
			simplest_log(
				"model_3d_calculate_tangents_bitangents "
				"provided NULL"
				);
		}
	}
	
	if(tangents_bitangents_checklist == constraints_ok)
	{
		how_many_of_each_index =
			(unsigned int *)
				calloc(
					(*model).num_of_vertices,
					sizeof(unsigned int)
					);
		
		if(how_many_of_each_index != NULL)
		{
			for(i = 0; i < (*model).num_of_vertices; i++)
			{
				*(how_many_of_each_index + i) = 0;
			}
			
			tangents_bitangents_checklist = helper_table_ok;
		}
	}
	
	if(tangents_bitangents_checklist == helper_table_ok)
	{
		(*model).tangents =
			(union column_vector_3_x_1_float *)
				calloc(
					(*model).num_of_vertices,
					sizeof(union column_vector_3_x_1_float)
					);
			
		if((*model).tangents != NULL)
		{
			(*model).bitangents =
				(union column_vector_3_x_1_float *)
					calloc(
						(*model).num_of_vertices,
						sizeof(union column_vector_3_x_1_float)
						);
			
			if((*model).bitangents != NULL)
			{
				for(i = 0; i < (*model).num_of_vertices; i++)
				{
					*((*model).tangents + i) =
						null_column_vector_3_x_1_float();
					*((*model).bitangents + i) =
						null_column_vector_3_x_1_float();
				}
				
				tangents_bitangents_checklist =
					tables_calloced;
			}
			else
			{
				simplest_log(
					"model_3d_calculate_tangents_bitangents "
					"cannot calloc bitangents"
					);
				free((void *) (*model).tangents);
			}
		}
		else
		{
			simplest_log(
				"model_3d_calculate_tangents_bitangents "
				"cannot calloc tangents"
				);
		}
	}

	if(tangents_bitangents_checklist == tables_calloced)
	{
		for(i = 0; i < (*model).num_of_indices_triplets; i++)
		{	// loop 1 - calculating and summing when appropriate
			#define CUR_TRIANGLE_VERT_00 \
				(* ((*model).indices_triplets + i) ).by_index[0]
			#define CUR_TRIANGLE_VERT_01 \
				(* ((*model).indices_triplets + i) ).by_index[1]
			#define CUR_TRIANGLE_VERT_02 \
				(* ((*model).indices_triplets + i) ).by_index[2]
			
			triangle_edge_0_to_1 = 
				column_vectors_3_x_1_float_substraction(
					(* ((*model).vertices + CUR_TRIANGLE_VERT_01) ).
						position,
					(* ((*model).vertices + CUR_TRIANGLE_VERT_00) ).
						position
					);
			
			triangle_edge_0_to_2 = 
				column_vectors_3_x_1_float_substraction(
					(* ((*model).vertices + CUR_TRIANGLE_VERT_02) ).
						position,
					(* ((*model).vertices + CUR_TRIANGLE_VERT_00) ).
						position
					);
			
			delta_uv_1_to_0 =
				column_vectors_2_x_1_float_substraction(
					(* ((*model).vertices + CUR_TRIANGLE_VERT_01) ).
						texture_coordinates,
					(* ((*model).vertices + CUR_TRIANGLE_VERT_00) ).
						texture_coordinates
					);
					
			delta_uv_2_to_0 =
				column_vectors_2_x_1_float_substraction(
					(* ((*model).vertices + CUR_TRIANGLE_VERT_02) ).
						texture_coordinates,
					(* ((*model).vertices + CUR_TRIANGLE_VERT_00) ).
						texture_coordinates
					);
			
			multiplier =
				1.0
				/
				(
					delta_uv_1_to_0.by_index[0] * delta_uv_2_to_0.by_index[1]
					-
					delta_uv_2_to_0.by_index[0] * delta_uv_1_to_0.by_index[1]
				);
			
			cur_tangent.
				by_index[0] =
					multiplier
					*
					(
						delta_uv_2_to_0.by_index[1] * triangle_edge_0_to_1.by_index[0]
						-
						delta_uv_1_to_0.by_index[1] * triangle_edge_0_to_2.by_index[0]
					);
			
			cur_tangent.
				by_index[1] =
					multiplier
					*
					(
						delta_uv_2_to_0.by_index[1] * triangle_edge_0_to_1.by_index[1]
						-
						delta_uv_1_to_0.by_index[1] * triangle_edge_0_to_2.by_index[1]
					);
					
			cur_tangent.
				by_index[2] = 
					multiplier
					*
					(
						delta_uv_2_to_0.by_index[1] * triangle_edge_0_to_1.by_index[2]
						-
						delta_uv_1_to_0.by_index[1] * triangle_edge_0_to_2.by_index[2]
					);
					
			cur_bitangent.
				by_index[0] =
					multiplier
					*
					(
						-1.0 * delta_uv_2_to_0.by_index[0] * triangle_edge_0_to_1.by_index[0]
						+
						delta_uv_1_to_0.by_index[0] * triangle_edge_0_to_2.by_index[0]
					);
					
			cur_bitangent.
				by_index[1] =
					multiplier
					*
					(
						-1.0 * delta_uv_2_to_0.by_index[0] * triangle_edge_0_to_1.by_index[1]
						+
						delta_uv_1_to_0.by_index[0] * triangle_edge_0_to_2.by_index[1]
					);
					
			cur_bitangent.
				by_index[2] =
					multiplier
					*
					(
						-1.0 * delta_uv_2_to_0.by_index[0] * triangle_edge_0_to_1.by_index[2]
						+
						delta_uv_1_to_0.by_index[0] * triangle_edge_0_to_2.by_index[2]
					);
					
			*((*model).tangents + CUR_TRIANGLE_VERT_00) =
				column_vectors_3_x_1_float_sum(
					*((*model).tangents + CUR_TRIANGLE_VERT_00),
					cur_tangent
					);
			*((*model).bitangents + CUR_TRIANGLE_VERT_00) =
				column_vectors_3_x_1_float_sum(
					*((*model).bitangents + CUR_TRIANGLE_VERT_00),
					cur_bitangent
					);

			*((*model).tangents + CUR_TRIANGLE_VERT_01) =
				column_vectors_3_x_1_float_sum(
					*((*model).tangents + CUR_TRIANGLE_VERT_01),
					cur_tangent
					);
			*((*model).bitangents + CUR_TRIANGLE_VERT_01) =
				column_vectors_3_x_1_float_sum(
					*((*model).bitangents + CUR_TRIANGLE_VERT_01),
					cur_bitangent
					);
					
			*((*model).tangents + CUR_TRIANGLE_VERT_02) =
				column_vectors_3_x_1_float_sum(
					*((*model).tangents + CUR_TRIANGLE_VERT_02),
					cur_tangent
					);
			*((*model).bitangents + CUR_TRIANGLE_VERT_02) =
				column_vectors_3_x_1_float_sum(
					*((*model).bitangents + CUR_TRIANGLE_VERT_02),
					cur_tangent
					);
					
			*(how_many_of_each_index + CUR_TRIANGLE_VERT_00) += 1;
			*(how_many_of_each_index + CUR_TRIANGLE_VERT_01) += 1;
			*(how_many_of_each_index + CUR_TRIANGLE_VERT_02) += 1;
			
		}//end of loop 1
		
		for(i = 0; i < (*model).num_of_vertices; i++)
		{//loop 2 - averaging where appropriate
			if(*(how_many_of_each_index + i) > 1)
			{
				*((*model).tangents + i) =
					column_vector_3_x_1_float_multiply_by_float(
						*((*model).tangents + i),
						1.0 / (*(how_many_of_each_index + i))
						);
			}
		}//end of loop 2
		
		tangents_bitangents_checklist = calculations_done;
	}
	
	//cleanup
	if(tangents_bitangents_checklist >= helper_table_ok)
	{
		free((void*) how_many_of_each_index);
	}
	
	//no need for eventually cleaning tangents and bitangents
	
	if(tangents_bitangents_checklist == calculations_done)
	{
		(*model).tangents_bitangents_present = MY_TRUE;
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		model_3d_assign_legacy_display_list
			
			for OpenGL 1.0
*/
int model_3d_assign_legacy_display_list(
	struct model_3d * model
	)
{
	enum model_3d_legacy_list_checklist
	{
		initial_value = 0,
		not_null,
		state_ok,
		list_generated,
		compiled
	} model_3d_legacy_list_checklist =
		initial_value;
	
	if(model_3d_legacy_list_checklist == initial_value)
	{
		if(model != NULL)
		{
			model_3d_legacy_list_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"model_3d_assign_legacy_display_list "
				"provided NULL"
				);
		}
	}
	
	if(model_3d_legacy_list_checklist == not_null)
	{
		if((*model).state == MY_TRUE)
		{
			model_3d_legacy_list_checklist =
				state_ok;
		}
		else
		{
			simplest_log(
				"model_3d_assign_legacy_display_list "
				"state is not true"
				);
		}
	}
	
	if(model_3d_legacy_list_checklist == state_ok)
	{
		(*model).legacy_display_list_id =
			glGenLists(1);
			
		if(
			(*model).legacy_display_list_id != 0
			&&
				gl_error_tool(
					"model_3d_assign_legacy_display_list "
					"generating list"
					)
				==
				GL_NO_ERROR
			)
		{
			model_3d_legacy_list_checklist =
				list_generated;
		}
		else
		{
			simplest_log(
				"model_3d_assign_legacy_display_list "
				"cannot generate list"
				);
			
			//for a case
			if((*model).legacy_display_list_id != 0)
			{
				glDeleteLists((*model).legacy_display_list_id, 1);
				gl_error_tool(
					"model_3d_assign_legacy_display_list "
					"deleteing list in case of error"
					);
			}
		}
	}
	
	if(model_3d_legacy_list_checklist == list_generated)
	{
		glNewList((*model).legacy_display_list_id, GL_COMPILE);
	
		// no texture here!
		
		glBegin(GL_TRIANGLES);
		
		int i, j;
		for(i = 0; i < (*model).num_of_indices_triplets; i++)
		{
			for(j = 0; j < 3; j++)
			{
				struct generic_vertex * cur_vertex =
					(*model).vertices
					+
					( (* ((*model).indices_triplets + i) ).by_index[j] );
				
				// tex coords and normals must be before vertex coords
				glTexCoord4f(
					(*cur_vertex).texture_coordinates.by_index[0],
					(*cur_vertex).texture_coordinates.by_index[1],
					0.0,
					1.0
					);
					
				glNormal3f(
					(*cur_vertex).normals.by_index[0],
					(*cur_vertex).normals.by_index[1],
					(*cur_vertex).normals.by_index[2]
					);
				
				glEdgeFlag(GL_TRUE);
				
				glVertex4f(
					(*cur_vertex).position.by_index[0],
					(*cur_vertex).position.by_index[1],
					(*cur_vertex).position.by_index[2],
					1.0
					);
			}
		}
		
		glEnd();
		
		glEndList();
		
		if(
			gl_error_tool(
				"model_3d_assign_legacy_display_list "
				"compiling list"
				)
			==
			GL_NO_ERROR
			)
		{
			model_3d_legacy_list_checklist = compiled;
		}
		else
		{
			simplest_log(
				"model_3d_assign_legacy_display_list "
				"gl error while compiling list"
				);
		}
	}
	
	if(model_3d_legacy_list_checklist == compiled)
	{
		return MY_TRUE;
	}
	else
	{
		if((*model).legacy_display_list_id != 0)
		{
			glDeleteLists((*model).legacy_display_list_id, 1);
			gl_error_tool(
				"model_3d_assign_legacy_display_list "
				"deleteing list in case of error"
				);
		}
		(*model).legacy_display_list_id = 0;
		
		return MY_FALSE;
	}
}
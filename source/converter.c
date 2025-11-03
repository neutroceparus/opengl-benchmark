//	Copyright (c) 2025 Paul Jankowski

#include "converter.h"

/*
		save_model_3d
*/
int save_model_3d(
	struct model_3d * model_to_save,
	const char * file_name
	)
{
	enum save_model_3d_checklist
	{
		initial_value = 0,
		prerequisites_ok,
		file_ok,
		header_ok,
		sums_ok,
		vertices_written,
		triangles_written
	} save_model_3d_checklist =
		initial_value;
	
	FILE * file = NULL;
	int i, err_in_loop = -1;
	
	if(save_model_3d_checklist == initial_value)
	{
		if(
			file_name != NULL
			&&
			model_to_save != NULL
			&&
			(*model_to_save).state == MY_TRUE
			)
		{
			#if defined _WIN32
			// see makefile libraries not included with -mconsole option
			if(PathFileExistsA(file_name) == FALSE)//win bool
			{
			#endif
				save_model_3d_checklist = prerequisites_ok;
			#if defined _WIN32
			}
			else
			{
				simplest_log(
					"save_model_3d "
					"file already exists: %s",
					file_name
					);
				printf(
					"file already exists: %s\n",
					file_name
					);
			}
			#endif
		}
		else
		{
			simplest_log(
				"save_model_3d "
				"prerequisites not met"
				);
			printf(
				"save_model_3d "
				"prerequisites not met\n"
				);
		}
	}
	
	if(save_model_3d_checklist == prerequisites_ok)
	{
		file = fopen(file_name, "wb");
		
		if(file != NULL)
		{
			save_model_3d_checklist = file_ok;
		}
		else
		{
			simplest_log(
				"save_model_3d "
				"cannot open file for writing: %s",
				file_name
				);
			printf(
				"cannot open file for writing: %s\n",
				file_name
				);
		}
	}
	
	if(save_model_3d_checklist == file_ok)
	{
		if(
			fputs("model_3d", file)
			>=
			0
			)
		{
			save_model_3d_checklist =
				header_ok;
		}
		else
		{
			simplest_log(
				"save_model_3d "
				"cannot print header to file: %s",
				file_name
				);
			printf(
				"cannot print header to file: %s\n",
				file_name
				);
		}
	}
	
	if(save_model_3d_checklist == header_ok)
	{
		if(
			save_uint_custom_convention(
				(unsigned int)
					(*model_to_save).num_of_vertices,
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				save_uint_custom_convention(
					(unsigned int)
						(*model_to_save).num_of_indices_triplets,
					file
					)
				==
				MY_TRUE
				)
			{
				save_model_3d_checklist =
					sums_ok;
			}
			else
			{
				simplest_log(
					"save_model_3d "
					"cannot print num_of_indices_triplets"
					);
				printf(
					"cannot print num_of_indices_triplets\n"
					);
			}
		}
		else
		{
			simplest_log(
				"save_model_3d "
				"cannot print num_of_vertices"
				);
			printf(
				"cannot print num_of_vertices\n"
				);
		}
	}
	
	if(save_model_3d_checklist == sums_ok)
	{
		for(
			i = 0;
				i < (*model_to_save).num_of_vertices
				&&
				err_in_loop == -1;
			i++
			)
		{
			if(
				save_generic_vertex_custom_convention(
					*((*model_to_save).vertices + i),
					file
					)
				==
				MY_TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"save_model_3d "
					"error while saving vertex: %d",
					i
					);
				printf(
					"error while saving vertex: %d\n",
					i
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			save_model_3d_checklist =
				vertices_written;
		}
		else
		{
			;
		}
	}
	
	if(save_model_3d_checklist == vertices_written)
	{
		for(
			i = 0;
				i < (*model_to_save).num_of_indices_triplets
				&&
				err_in_loop == -1;
			i++
			)
		{
			if(
				save_column_vector_3_x_1_uint_custom_convention(
					*((*model_to_save).indices_triplets + i),
					file
					)
				==
				MY_TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"save_model_3d "
					"error while saving triplet: %d",
					i
					);
				printf(
					"error while saving triplet: %d\n",
					i
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			save_model_3d_checklist =
				triangles_written;
		}
		else
		{
			;
		}
	}
	
	if(save_model_3d_checklist >= file_ok)
	{
		fclose(file);
	}
	
	if(save_model_3d_checklist == triangles_written)
	{
		return MY_TRUE;
	}
	else
	{
		if(save_model_3d_checklist >= file_ok)
		{
			simplest_log(
				"save_model_3d "
				"deleting file: %s",
				file_name
				);
			printf(
				"deleting file: %s\n",
				file_name
				);
				
		#if defined _WIN32
			int last_error = 0;
		
			if(DeleteFileA(file_name) != 0)
			{
				;
			}
			else
			{
				last_error = GetLastError();
				if(last_error == 2)
				{
					;
				}
				else if(last_error == 5)
				{
					simplest_log(
						"save_model_3d "
						"cannot delete: %s, "
						"Access denied.",
						file_name
						);
					printf(
						"cannot delete: %s, "
						"Access denied.\n",
						file_name
						);
				}
				else
				{
					simplest_log(
						"save_model_3d "
						"cannot delete: %s, "
						"Generic error.",
						file_name
						);
					printf(
						"cannot delete: %s, "
						"Generic error.\n",
						file_name
						);
				}
			}
			
		#else
			
			if(unlink(file_name) == 0)
			{
				;
			}
			else
			{
				simplest_log(
					"save_model_3d "
					"cannot delete: %s, "
					"Generic error.",
					file_name
					);
				printf(
					"cannot delete: %s, "
					"Generic error.\n",
					file_name
					);
			}
			
		#endif
		}
		
		return MY_FALSE;
	}
}

/*
		saving functions from smallest to largest
			
			all return MY_TRUE on success
			otherwise MY_FALSE
*/
/*
		save_uint_8
*/	
int save_uint_8(
	uint8_t value,
	FILE * file
	)
{
	if(file != NULL)
	{
		if(
			putc(
				(int) value,
				file
				)
			!=
			EOF
			)
		{
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"save_uint_8 "
				"cannot save %u to %p",
				(unsigned int) value,
				file
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"save_uint_8 "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		save_uint_custom_convention
*/
int save_uint_custom_convention(
	unsigned int value,
	FILE * file
	)
{
	if(file != NULL)
	{
	#if defined ENABLE_FAST_CONVERSION
		if(get_endianness() == ENDIANNES_LITTLE)
		{
			union uint32_4_uint8s val_u;
			
			if(value <= 16777215) // 0xFFFFFF
			{
				val_u = (union uint32_4_uint8s) value;
			}
			else
			{
				val_u.uint32 = 16777215; // 0xFFFFFF
			}
			if(
				save_uint_8(val_u.uint8[2], file)
				==
				MY_TRUE
				)
			{
				if(
					save_uint_8(val_u.uint8[1], file)
					==
					MY_TRUE
					)
				{
					if(
						save_uint_8(val_u.uint8[0], file)
						==
						MY_TRUE
						)
					{
						return MY_TRUE;
					}
				}
			}
			
			simplest_log(
				"save_uint_custom_convention "
				"cannot save %u %p",
				value,
				file
				);
			return MY_FALSE;
		}
		else if(get_endianness() == ENDIANNES_BIG)
		{
			union uint32_4_uint8s val_u;
			
			if(value <= 16777215) // 0xFFFFFF
			{
				val_u = (union uint32_4_uint8s) value;
			}
			else
			{
				val_u.uint32 = 16777215; // 0xFFFFFF
			}
			if(
				save_uint_8(val_u.uint8[1], file)
				==
				MY_TRUE
				)
			{
				if(
					save_uint_8(val_u.uint8[2], file)
					==
					MY_TRUE
					)
				{
					if(
						save_uint_8(val_u.uint8[3], file)
						==
						MY_TRUE
						)
					{
						return MY_TRUE;
					}
				}
			}
			
			simplest_log(
				"save_uint_custom_convention "
				"cannot save %u %p",
				value,
				file
				);
			return MY_FALSE;
		}
		else
		{
	#endif
			if(
				save_uint_8(
					(uint8_t) ((value & 0x00FF0000) / 0x00010000), 
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					save_uint_8(
						(uint8_t) ((value & 0x0000FF00) / 0x00000100), 
						file
						)
					==
					MY_TRUE
					)
				{
					if(
						save_uint_8(
							(uint8_t) (value & 0x000000FF), 
							file
							)
						==
						MY_TRUE
						)
					{
						return MY_TRUE;
					}
				}
			}
			
			simplest_log(
				"save_uint_custom_convention "
				"cannot save %u %p",
				value,
				file
				);
			return MY_FALSE;
	#if defined ENABLE_FAST_CONVERSION
		}
	#endif
	}
	else
	{
		simplest_log(
			"save_uint_custom_convention "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		save_float_custom_convention
*/
int save_float_custom_convention(
	float value,
	FILE * file
	)
{
	if(file != NULL)
	{
		if(value > (float) 8388607)
		{
			value = (float) 8388607;
		}
		else if(value < (float) -8388607)
		{
			value = (float) -8388607;
		}
		
		int minus = MY_FALSE;
		
		if(value < 0)
		{
			minus = MY_TRUE;
			value = fabs(value);
		}
		
		float integral_part_f =
			floor(value);
		if(
			save_uint_custom_convention(
				((unsigned int) integral_part_f) + (minus == MY_TRUE ? 0x800000 : 0),
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				save_uint_custom_convention(
					(unsigned int)
						floor((value - integral_part_f) * 16777216), //see specification
					file
					)
				==
				MY_TRUE
				)
			{
				return MY_TRUE;
			}
		}
		
		simplest_log(
			"save_float_custom_convention "
			"cannot save %f %p",
			value,
			file
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"save_float_custom_convention "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		save_column_vector_3_x_1_uint_custom_convention
*/
int save_column_vector_3_x_1_uint_custom_convention(
	union column_vector_3_x_1_uint value,
	FILE * file
	)
{
	if(file != NULL)
	{
		if(
			save_uint_custom_convention(
				value.by_index[0],
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				save_uint_custom_convention(
					value.by_index[1],
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					save_uint_custom_convention(
						value.by_index[2],
						file
						)
					==
					MY_TRUE
					)
				{
					return MY_TRUE;
				}
			}
		}
		simplest_log(
			"save_column_vector_3_x_1_uint_custom_convention "
			"cannot save"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"save_column_vector_3_x_1_uint_custom_convention "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		save_column_vector_2_x_1_float_custom_convention
*/
int save_column_vector_2_x_1_float_custom_convention(
	union column_vector_2_x_1_float value,
	FILE * file
	)
{
	if(file != NULL)
	{
		if(
			save_float_custom_convention(
				value.by_index[0],
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				save_float_custom_convention(
					value.by_index[1],
					file
					)
				==
				MY_TRUE
				)
			{
				return MY_TRUE;
			}
		}
		
		simplest_log(
			"save_column_vector_2_x_1_float_custom_convention "
			"cannot save"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"save_column_vector_2_x_1_float_custom_convention "
			"provided NULL"
			);
		return MY_FALSE;
	}
}
	
/*
		save_column_vector_3_x_1_float_custom_convention
*/
int save_column_vector_3_x_1_float_custom_convention(
	union column_vector_3_x_1_float value,
	FILE * file
	)
{
	if(file != NULL)
	{
		if(
			save_float_custom_convention(
				value.by_index[0],
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				save_float_custom_convention(
					value.by_index[1],
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					save_float_custom_convention(
						value.by_index[2],
						file
						)
					==
					MY_TRUE
					)
				{
					return MY_TRUE;
				}
			}
		}
		
		simplest_log(
			"save_column_vector_3_x_1_float_custom_convention "
			"cannot save"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"save_column_vector_3_x_1_float_custom_convention "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		save_triplet_column_vectors_3_x_1_uint_custom_convention
*/
int save_triplet_column_vectors_3_x_1_uint_custom_convention(
	struct triplet_column_vectors_3_x_1_uint triplet,
	FILE * file
	)
{
	if(file != NULL)
	{
		if(
			save_column_vector_3_x_1_uint_custom_convention(
				triplet.by_index[0],
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				save_column_vector_3_x_1_uint_custom_convention(
					triplet.by_index[1],
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					save_column_vector_3_x_1_uint_custom_convention(
						triplet.by_index[2],
						file
						)
					==
					MY_TRUE
					)
				{
					return MY_TRUE;
				}
			}
		}
		
		simplest_log(
			"save_triplet_column_vectors_3_x_1_uint_custom_convention "
			"cannot save"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"save_triplet_column_vectors_3_x_1_uint_custom_convention "
			"provided NULL"
			);
		return MY_FALSE;
	}
}
	
/*
		save_generic_vertex_custom_convention
*/
int save_generic_vertex_custom_convention(
	struct generic_vertex vertex,
	FILE * file
	)
{
	if(file != NULL)
	{
		if(
			save_column_vector_3_x_1_float_custom_convention(
				vertex.position,
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				save_column_vector_3_x_1_float_custom_convention(
					vertex.normals,
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					save_column_vector_2_x_1_float_custom_convention(
						vertex.texture_coordinates,
						file
						)
					==
					MY_TRUE
					)
				{
					return MY_TRUE;
				}
			}
		}
		
		simplest_log(
			"save_generic_vertex_custom_convention "
			"cannot save"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"save_generic_vertex_custom_convention "
			"provided NULL"
			);
		return MY_FALSE;
	}
}
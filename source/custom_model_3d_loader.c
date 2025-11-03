//	Copyright (c) 2025 Paul Jankowski

#include "custom_model_3d_loader.h"

/*
	complementary writing functions are in:
		converter.h / converter.c
*/
/*
		parse_single_custom_model_3d
*/
struct model_3d parse_single_custom_model_3d(
	char * file_name
	)
{
	enum parse_model_3d_checklist
	{
		initial_value = 0,
		file_ok,
		header_ok,
		sums_taken,
		calloced,
		vertices_ok,
		triangles_ok
	} parse_model_3d_checklist = initial_value;
	
	struct model_3d result = empty_model_3d();
	
	FILE * file_to_read = NULL;
	
	int i, err_in_loop = -1;

	if(parse_model_3d_checklist == initial_value)
	{
		if(file_name != NULL)
		{
			file_to_read = fopen(file_name, "rb");
			
			if(file_to_read != NULL)
			{
				parse_model_3d_checklist = file_ok;
			}
			else
			{
				simplest_log(
					"parse_single_custom_model_3d "
					"cannot open file %s",
					file_name
					);
			}
		}
		else
		{
			simplest_log(
				"parse_single_custom_model_3d "
				"provided NULL"
				);
		}
	}
	
	if(parse_model_3d_checklist == file_ok)
	{
		char buf[9];
		buf[8] = '\0';
		
		if(fgets(buf, 9, file_to_read) == buf)
		{	
			if(strcmp(buf, "model_3d") == 0)
			{
				parse_model_3d_checklist = header_ok;
			}
			else
			{
				simplest_log(
					"parse_single_custom_model_3d "
					"wrong header of %s",
					file_name
					);
			}
		}
		else
		{
			simplest_log(
				"parse_single_custom_model_3d "
				"cannot read header of %s",
				file_name
				);
		}
	}
	
	if(parse_model_3d_checklist == header_ok)
	{
		if(
			read_uint_custom_convention(
				(unsigned int *) &(result.num_of_vertices),
				file_to_read
				)
			==
			MY_TRUE
			)
		{
			if(
				read_uint_custom_convention(
					(unsigned int *) &(result.num_of_indices_triplets),
					file_to_read
					)
				==
				MY_TRUE
				)
			{
				parse_model_3d_checklist = sums_taken;
			}
			else
			{
				simplest_log(
					"parse_single_custom_model_3d "
					"cannot take triangles_num of %s",
					file_name
					);
			}
		}
		else
		{
			simplest_log(
				"parse_single_custom_model_3d "
				"cannot take vertices_num of %s",
				file_name
				);
		}
	}
	
	if(parse_model_3d_checklist == sums_taken)
	{
		result.vertices =
			(struct generic_vertex *)
				calloc(
					result.num_of_vertices,
					sizeof(struct generic_vertex)
					);
		
		if(result.vertices != NULL)
		{
			result.indices_triplets =
				(union column_vector_3_x_1_uint *)
					calloc(
						result.num_of_indices_triplets,
						sizeof(union column_vector_3_x_1_uint)
						);
			
			if(result.indices_triplets != NULL)
			{
				parse_model_3d_checklist = calloced;
			}
			else
			{
				simplest_log(
					"parse_single_custom_model_3d "
					"cannot calloc indices of %s",
					file_name
					);
				free(result.vertices);
			}
		}
		else
		{
			simplest_log(
				"parse_single_custom_model_3d "
				"cannot calloc vertices of %s",
				file_name
				);
		}
	}	
	
	if(parse_model_3d_checklist == calloced)
	{
		for(
			i = 0;
				i < result.num_of_vertices
				&&
				err_in_loop == -1;
			i++
			)
		{
			if(
				read_generic_vertex_custom_convention(
					result.vertices + i,
					file_to_read
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
					"parse_single_custom_model_3d "
					"failed to read vertice %d of %s",
					i,
					file_name
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			parse_model_3d_checklist = vertices_ok;
		}
	}
	
	if(parse_model_3d_checklist == vertices_ok)
	{
		for(
			i = 0;
				i < result.num_of_indices_triplets
				&&
				err_in_loop == -1;
			i++
			)
		{
			if(
				read_column_vector_3_x_1_uint_custom_convention(
					result.indices_triplets + i,
					file_to_read
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
					"parse_single_custom_model_3d "
					"failed to read triangle %d of %s",
					i,
					file_name
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			parse_model_3d_checklist = triangles_ok;
		}
	}
	
	if(parse_model_3d_checklist == triangles_ok)
	{
		if(fgetc(file_to_read) != EOF)
		{
			simplest_log(
				"parse_single_custom_model_3d "
				"there is some more content i file %s,"
				"acceptable, proceed",
				file_name
				);
		}
	}
	
	if(parse_model_3d_checklist >= file_ok)
	{
		fclose(file_to_read);
	}
	
	if(parse_model_3d_checklist == triangles_ok)
	{
		result.state = MY_TRUE;
		return result;
	}
	else
	{
		if(parse_model_3d_checklist >= calloced)
		{
			free(result.vertices);
			free(result.indices_triplets);
		}
		
		return empty_model_3d();
	}
}

/*
		reading functions from smallest to largest
			
			all return MY_TRUE on success
			otherwise MY_FALSE
*/
/*
		read_uint_8
*/
int read_uint_8(
	uint8_t * where_to_put_result,
	FILE * file
	)
{
	if(
		where_to_put_result != NULL
		&&
		file != NULL
		)
	{
		int c = fgetc(file);
		if(c != EOF)
		{
			*where_to_put_result =
				(uint8_t) c;
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"read_uint_8"
				"error while reading"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"read_uint_8"
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}
	
/*
		read_uint_custom_convention
*/
int read_uint_custom_convention(
	unsigned int * where_to_put_result,
	FILE * file
	)
{
	if(
		where_to_put_result != NULL
		&&
		file != NULL
		)
	{
	#if defined ENABLE_FAST_CONVERSION
		if(get_endianness() == ENDIANNES_LITTLE)
		{
			union uint32_4_uint8s buf;
			buf.uint32 = 0;
			if(
				read_uint_8(
					buf.uint8 + 2,
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					read_uint_8(
						buf.uint8 + 1,
						file
						)
					==
					MY_TRUE
					)
				{
					if(
						read_uint_8(
							buf.uint8 + 0,
							file
							)
						==
						MY_TRUE
						)
					{
						(*where_to_put_result) =
							(unsigned int)
								buf.uint32;
						return MY_TRUE;
					}
				}
			}
		}
		else if(get_endianness() == ENDIANNES_BIG)
		{
			union uint32_4_uint8s buf;
			buf.uint32 = 0;
			if(
				read_uint_8(
					buf.uint8 + 1,
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					read_uint_8(
						buf.uint8 + 2,
						file
						)
					==
					MY_TRUE
					)
				{
					if(
						read_uint_8(
							buf.uint8 + 3,
							file
							)
						==
						MY_TRUE
						)
					{
						(*where_to_put_result) =
							(unsigned int)
								buf.uint32;
						return MY_TRUE;
					}
				}
			}
		}
		else
		{
	#endif
			uint8_t buf;
			if(
				read_uint_8(&buf, file)
				==
				MY_TRUE
				)
			{
				*where_to_put_result =
					((unsigned int) buf)
					*
					((unsigned int) 0x00010000);
					
				if(
					read_uint_8(&buf, file)
					==
					MY_TRUE
					)
				{
					*where_to_put_result +=
						((unsigned int) buf)
						*
						((unsigned int) 0x00000010);
						
					if(
						read_uint_8(&buf, file)
						==
						MY_TRUE
						)
					{
						*where_to_put_result +=
							((unsigned int) buf);
						
						return MY_TRUE;
					}
				}
			}
		
	#if defined ENABLE_FAST_CONVERSION
		}
	#endif
		
		simplest_log(
			"read_uint_custom_convention"
			"error while reading"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"read_uint_custom_convention"
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}

/*
		read_float_custom_convention
*/
int read_float_custom_convention(
	float * where_to_put_result,
	FILE * file
	)
{
	if(
		where_to_put_result != NULL
		&&
		file != NULL
		)
	{
		unsigned int buf;
		int minus = MY_FALSE;
		
		if(
			read_uint_custom_convention(
				&buf,
				file
				)
			==
			MY_TRUE
			)
		{
			if((buf & 0x800000) == 0x800000)
			{
				minus = MY_TRUE;
			}
			
			*where_to_put_result =
				(float)
					(buf & 0x7FFFFF);
			
			if(
				read_uint_custom_convention(
					&buf,
					file
					)
				==
				MY_TRUE
				)
			{
				*where_to_put_result +=
					((float) buf)
					/
					((float) 0x1000000);
				
				if(minus == MY_TRUE)
				{
					*where_to_put_result *= -1;
				}
				
				return MY_TRUE;
			}
		}
		
		simplest_log(
			"read_float_custom_convention"
			"cannot read"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"read_float_custom_convention"
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}

/*
		read_column_vector_3_x_1_uint_custom_convention
*/
int read_column_vector_3_x_1_uint_custom_convention(
	union column_vector_3_x_1_uint * where_to_put_result,
	FILE * file
	)
{
	if(
		where_to_put_result != NULL
		&&
		file != NULL
		)
	{
		if(
			read_uint_custom_convention(
				(*where_to_put_result).by_index + 0,
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				read_uint_custom_convention(
					(*where_to_put_result).by_index + 1,
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					read_uint_custom_convention(
						(*where_to_put_result).by_index + 2,
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
			"read_column_vector_3_x_1_uint_custom_convention"
			"cannot read"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"read_column_vector_3_x_1_uint_custom_convention"
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}

/*
		read_column_vector_2_x_1_float_custom_convention
*/
int read_column_vector_2_x_1_float_custom_convention(
	union column_vector_2_x_1_float * where_to_put_result,
	FILE * file
	)
{
	if(
		where_to_put_result != NULL
		&&
		file != NULL
		)
	{
		if(
			read_float_custom_convention(
				(*where_to_put_result).by_index + 0,
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				read_float_custom_convention(
					(*where_to_put_result).by_index + 1,
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
			"read_column_vector_2_x_1_float_custom_convention"
			"cannot read"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"read_column_vector_2_x_1_float_custom_convention"
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}
	
/*
		read_column_vector_3_x_1_float_custom_convention
*/
int read_column_vector_3_x_1_float_custom_convention(
	union column_vector_3_x_1_float * where_to_put_result,
	FILE * file
	)
{
	if(
		where_to_put_result != NULL
		&&
		file != NULL
		)
	{
		if(
			read_float_custom_convention(
				(*where_to_put_result).by_index + 0,
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				read_float_custom_convention(
					(*where_to_put_result).by_index + 1,
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					read_float_custom_convention(
						(*where_to_put_result).by_index + 2,
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
			"read_column_vector_3_x_1_float_custom_convention"
			"cannot read"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"read_column_vector_3_x_1_float_custom_convention"
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}
	
/*
		read_generic_vertex_custom_convention
*/
int read_generic_vertex_custom_convention(
	struct generic_vertex * where_to_put_result,
	FILE * file
	)
{
	if(
		where_to_put_result != NULL
		&&
		file != NULL
		)
	{
		if(
			read_column_vector_3_x_1_float_custom_convention(
				&((*where_to_put_result).position),
				file
				)
			==
			MY_TRUE
			)
		{
			if(
				read_column_vector_3_x_1_float_custom_convention(
					&((*where_to_put_result).normals),
					file
					)
				==
				MY_TRUE
				)
			{
				if(
					read_column_vector_2_x_1_float_custom_convention(
						&((*where_to_put_result).texture_coordinates),
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
			"read_generic_vertex_custom_convention"
			"cannot read"
			);
		return MY_FALSE;
	}
	else
	{
		simplest_log(
			"read_generic_vertex_custom_convention"
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
}
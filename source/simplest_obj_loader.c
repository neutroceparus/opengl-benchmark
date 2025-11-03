//	Copyright (c) 2025 Paul Jankowski

#include "simplest_obj_loader.h"

/*
	static helpers functions
*/
/*
		line_buf_256_divided_by_spaces_to_column_vector_3_x_1_float
*/
static int line_buf_256_divided_by_spaces_to_column_vector_3_x_1_float(
	char * line_buffer,
	int pos_of_start_of_first_float,
	union column_vector_3_x_1_float * where_to_return,
	int * here_returned_num_of_read_chars
	);

/*
		line_buf_256_divided_by_spaces_to_column_vector_2_x_1_float
*/
static int line_buf_256_divided_by_spaces_to_column_vector_2_x_1_float(
	char * line_buffer,
	int pos_of_start_of_first_float,
	union column_vector_2_x_1_float * where_to_return,
	int * here_returned_num_of_read_chars
	);

/*
		line_buf_256_obj_style_indices_to_3_int_vectors
*/
static int line_buf_256_obj_style_indices_to_3_int_vectors(
	char * line_buffer,
	union column_vector_3_x_1_int * where_to_put_indices_0,
	union column_vector_3_x_1_int * where_to_put_indices_1,
	union column_vector_3_x_1_int * where_to_put_indices_2
	);

/*
		parse_single_model_obj
			
			only first model in the file
			
			on windows to printf time must be attached console
			or compiled with console entry point
			
			estimated time functionality is not finished
*/
struct model_3d parse_single_model_obj(
	char * path_to_obj_file,
	int printf_estimated_time
	)
{
	enum first_from_obj_checklist
	{
		initial_value = 0,
		not_nulls,
		file_opened,
		count_and_first_check_ok,
		transfer_buffers_alloced,
		name_alloced,
		data_loaded,
		indexation_phase_1_ok,
		verts_and_indices_in_model_calloced,
		values_written_to_model
	} first_from_obj_checklist =
		initial_value;
	
	struct model_3d result =
		empty_model_3d();
	
	FILE * file_to_read = NULL;
	
	// these buffers are for transfering from file
	int vertices_in_file_count = 0;
	union column_vector_3_x_1_float * vertex_transfering_buf = NULL;
	int normals_in_file_count = 0;
	union column_vector_3_x_1_float * normals_transfering_buf = NULL;
	int texture_coords_in_file_count = 0;
	union column_vector_2_x_1_float * texture_coords_transfering_buf = NULL;
	
	// indices as they are in file - remember to multiply by 3
	int indices_triplets_in_file_count = 0;
	union column_vector_3_x_1_int * indices_transferring_buf = NULL;
	
	// usage of buffer to avoid passing EOF case from subfunctions
	// also knowing accepted .obj structure
	char line_buffer[256];
	int i, j, k;
	for(i = 0; i < 256; i++)
	{
		line_buffer[i] = '\0';
	}
	
	//for final storage
	int vertices_distinct_count = 0;
	
	int name_length = 0;
	
	int * indexation_phase_1 = NULL;
	
	if(first_from_obj_checklist == initial_value)
	{
		if(
			path_to_obj_file != NULL
			)
		{
			first_from_obj_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"parse_single_model_obj "
				"provided NULL(s)"
				);
		}
	}
	
	if(first_from_obj_checklist == not_nulls)
	{
		file_to_read = fopen(path_to_obj_file, "rb");
		if(file_to_read != NULL)
		{
			first_from_obj_checklist =
				file_opened;
		}
		else
		{
			simplest_log(
				"parse_single_model_obj "
				"cannot open file %s, "
				"returning MY_FALSE",
				path_to_obj_file
				);
		}
	}

	if(first_from_obj_checklist == file_opened)
	{
		// first loop - counting
		
		int loop_running = MY_TRUE;
		int err_in_loop = MY_FALSE;
		
		int pos_in_line = 0;
		int line_count = 0;
		
		int cur_char = 0;
		int prev_char = 0;
		
		int comment_line = MY_FALSE;
		int line_with_name = MY_FALSE;
		
		int name_encountered = MY_FALSE;
		
		while(
			loop_running == MY_TRUE
			&&
			err_in_loop == MY_FALSE
			)
		{
			prev_char = cur_char;
			cur_char = fgetc(file_to_read);
			
			if(line_count >= INT_MAX)
			{
				loop_running = MY_FALSE;
				err_in_loop = MY_TRUE;
				simplest_log(
					"parse_single_model_obj "
					"too many lines, "
					"in file: %s",
					path_to_obj_file
					);
			}
			
			if(pos_in_line >= INT_MAX)
			{
				loop_running = MY_FALSE;
				err_in_loop = MY_TRUE;
				simplest_log(
					"parse_single_model_obj "
					"too many chars in line, "
					"in line: %d, "
					"in file: %s",
					line_count,
					path_to_obj_file
					);
			}
			
			if(cur_char == EOF)
			{
				loop_running = MY_FALSE;
				line_count++;
			}
			else if(cur_char == '\n')
			{
				line_count++;
				pos_in_line = 0;
				
				if(comment_line == MY_TRUE)
				{
					comment_line = MY_FALSE;
				}
				
				if(line_with_name == MY_TRUE)
				{
					line_with_name = MY_FALSE;
				}
			}
			else if(
				cur_char == '\r'
				&&
				prev_char == '\n'
				)
			{
				;
			}
			else if(
				pos_in_line == 0
				&&
				cur_char == '#'
				)
			{
				pos_in_line += 1;
				comment_line = MY_TRUE;
			}
			else if(
				pos_in_line == 1
				&&
				cur_char == ' '
				&&
				prev_char == 'o'
				)
			{
				pos_in_line += 1;
				line_with_name = MY_TRUE;
				
				if(name_encountered == MY_FALSE)
				{
					name_encountered = MY_TRUE;
				}
				else
				{
					simplest_log(
						"parse_single_model_obj "
						"more than 1 object, "
						"use separate files, "
						"in line: %d, "
						"in file: %s",
						line_count,
						path_to_obj_file
						);
					loop_running = MY_FALSE;
					err_in_loop = MY_TRUE;
				}
			}
			else if(
				pos_in_line == 1
				&&
				cur_char == ' '
				&&
				prev_char == 'v'
				)
			{
				vertices_in_file_count += 1;
				pos_in_line += 1;
			}
			else if(
				pos_in_line == 1
				&&
				cur_char == 'n'
				&&
				prev_char == 'v'
				)
			{
				normals_in_file_count += 1;
				pos_in_line += 1;
			}
			else if(
				pos_in_line == 1
				&&
				cur_char == 't'
				&&
				prev_char == 'v'
				)
			{
				texture_coords_in_file_count += 1;
				pos_in_line += 1;
			}
			else if(
				pos_in_line == 1
				&&
				cur_char == ' '
				&&
				prev_char == 'f'
				)
			{
				indices_triplets_in_file_count += 1;
				pos_in_line += 1;
			}
			else if(
				pos_in_line == 0
				&&
				cur_char == 'v'
				)
			{
				pos_in_line += 1;
			}
			else if(
				pos_in_line == 0
				&&
				cur_char == 'o'
				)
			{
				pos_in_line += 1;
			}
			else if(
				pos_in_line == 0
				&&
				cur_char == 's'
				)
			{
				pos_in_line += 1;
			}
			else if(
				pos_in_line == 0
				&&
				cur_char == 'f'
				)
			{
				pos_in_line += 1;
			}
			else if(
				pos_in_line >= 1
				&&
				comment_line == MY_TRUE
				)
			{
				pos_in_line += 1;
			}
			else if(
				pos_in_line >= 1
				&&
				line_with_name == MY_TRUE
				)
			{
				pos_in_line += 1;
				
				if(pos_in_line >= 2)
				{
					name_length += 1;
				}
			}
			else if(
				cur_char >= '0'
				&&
				cur_char <= '9'
				)
			{
				pos_in_line += 1;
			}
			else if(
				cur_char == '-'
				||
				cur_char == '.'
				||
				cur_char == '/'
				||
				cur_char == ' '
				)
			{
				pos_in_line += 1;
			}
			else
			{
				loop_running = MY_FALSE;
				err_in_loop = MY_TRUE;
				
				simplest_log(
					"parse_single_model_obj "
					"illegal char: %c, "
					"in line: %d, "
					"in file: %s",
					cur_char,
					line_count,
					path_to_obj_file
					);
			}
		}
		
		if(err_in_loop == MY_FALSE)
		{
			if(
				vertices_in_file_count >= 1
				&&
				normals_in_file_count >= 1
				&&
				texture_coords_in_file_count >= 1
				&&
				indices_triplets_in_file_count >= 1
				)
			{
				
				//here print estimated time
				//estimated time functionality not finished
				//so only printing num of triangles
				if(printf_estimated_time == MY_TRUE)
				{
					printf(
						"num of indices triplets: %d\n"
						//"estimated time: \n"
						,
						indices_triplets_in_file_count
						);
				}
				
				first_from_obj_checklist =
					count_and_first_check_ok;
			}
			else
			{
				simplest_log(
					"parse_single_model_obj "
					"file lacks entries, "
					"in file: %s",
					path_to_obj_file
					);
			}
		}
		else
		{
			;//logged in loop
		}
	}

	if(
		first_from_obj_checklist
		==
		count_and_first_check_ok
		)
	{
		vertex_transfering_buf =
			(union column_vector_3_x_1_float *)
				calloc(
					vertices_in_file_count,
					sizeof(union column_vector_3_x_1_float)
					);
		
		if(vertex_transfering_buf != NULL)
		{
			normals_transfering_buf =
				(union column_vector_3_x_1_float *)
					calloc(
						normals_in_file_count,
						sizeof(union column_vector_3_x_1_float)
						);
			
			if(normals_transfering_buf != NULL)
			{
				texture_coords_transfering_buf =
					(union column_vector_2_x_1_float *)
						calloc(
							texture_coords_in_file_count,
							sizeof(union column_vector_2_x_1_float)
							);
				
				if(texture_coords_transfering_buf != NULL)
				{
					indices_transferring_buf =
						(union column_vector_3_x_1_int *)
							calloc(
								indices_triplets_in_file_count * 3, // !
								sizeof(union column_vector_3_x_1_int)
								);
					
					if(indices_transferring_buf != NULL)
					{
						first_from_obj_checklist =
							transfer_buffers_alloced;
					}
					else
					{
						simplest_log(
							"parse_single_model_obj "
							"cannot calloc "
							"indices_transferring_buf"
							);
						free(vertex_transfering_buf);
						free(normals_transfering_buf);
						free(texture_coords_transfering_buf);
					}
				}
				else
				{
					simplest_log(
						"parse_single_model_obj "
						"cannot calloc "
						"texture_coords_transfering_buf"
						);
					free(vertex_transfering_buf);
					free(normals_transfering_buf);
				}
			}
			else
			{
				simplest_log(
					"parse_single_model_obj "
					"cannot calloc "
					"normals_transfering_buf"
					);
				free(vertex_transfering_buf);
			}
		}
		else
		{
			simplest_log(
				"parse_single_model_obj "
				"cannot calloc "
				"vertex_transfering_buf"
				);
		}
	}
	
	if(
		first_from_obj_checklist
		==
		transfer_buffers_alloced
		)
	{
		result.name =
			(char *)
				calloc(
					name_length + 1,
					sizeof(char)
					);
		
		if(result.name != NULL)
		{
			first_from_obj_checklist =
				name_alloced;
		}
	}

	if(
		first_from_obj_checklist
		==
		name_alloced
		)
	{
		// second loop - reading
		
		int loop_running = MY_TRUE;
		int err_in_loop = MY_FALSE;
		int sub_loop_running = MY_TRUE;
		
		int line_count = 0;
		
		int cur_char = 0;
		
		int obj_def_encountered = MY_FALSE;
		
		int vertices_count = 0, vertices_normals_count = 0;
		int texture_coords_count = 0, indices_triplets_count = 0;
		
		int buf_pos;
		
		int how_many_chars_used_in_parsing_func = 0;
		
		rewind(file_to_read);
		
		while(
			loop_running == MY_TRUE
			&&
			err_in_loop == MY_FALSE
			)
		{
			for(i = 0; i < 256; i++)
			{
				line_buffer[i] = 0;
			}
			
			sub_loop_running = MY_TRUE;
			buf_pos = 0;
			while(sub_loop_running == MY_TRUE)
			{
				cur_char = fgetc(file_to_read);
				
				if(cur_char == EOF)
				{
					sub_loop_running = MY_FALSE;
					loop_running = MY_FALSE;
					//for ease
					if(buf_pos < 256)
					{
						line_buffer[buf_pos] = '\n';
						line_count++;
					}
				}
				else
				{
					if(!(cur_char == '\r' && buf_pos == 0))
					{
						line_buffer[buf_pos] = cur_char;
						buf_pos++;
					}
					
					if(cur_char == '\n')
					{
						sub_loop_running = MY_FALSE;
						line_count++;
					}
					
					if(buf_pos > 255)
					{
						simplest_log(
							"parse_single_model_obj "
							"reached end of line buffer "
							"file corrupted, "
							"returning MY_FALSE, "
							"at line %d in file %s",
							line_count,
							path_to_obj_file
							);
						sub_loop_running = MY_FALSE;
						loop_running = MY_FALSE;
						err_in_loop = MY_TRUE;
					}
				}
			} // end of sub loop
			
			if(loop_running == MY_TRUE) // part 2 of loop
			{
				if(
					line_buffer[0] == '#'
					||
					line_buffer[0] == 's'
					||
					line_buffer[0] == 'l'
					)
				{
					;
				}
				else if(
					line_buffer[0] == 'o'
					&&
					line_buffer[1] == ' '
					)
				{
					if(obj_def_encountered == MY_FALSE)
					{
						obj_def_encountered = MY_TRUE;
						
						for(i = 0; i < name_length; i++)
						{
							*(result.name + i) =
								line_buffer[i + 2];
						}
						*(result.name + i) = '\0';
						
						if(line_buffer[i + 2] != '\n')
						{
							simplest_log(
								"parse_single_model_obj "
								"error while writing name "
								"file corrupted, "
								"returning MY_FALSE, "
								"in file %s",
								path_to_obj_file
								);
							loop_running = MY_FALSE;
							err_in_loop = MY_TRUE;
						}
					}
					else
					{
						STRICT_LOG(
							"parse_single_model_obj "
							"second object "
							"stopping, "
							"at line %d in file %s",
							line_count,
							path_to_obj_file
							)
						loop_running = MY_FALSE;
					}
				}
				else if(
					line_buffer[0] == 'v'
					&&
					line_buffer[1] == ' '
					)
				{
					if(vertices_count < vertices_in_file_count)
					{
						if(
							line_buf_256_divided_by_spaces_to_column_vector_3_x_1_float(
								line_buffer,
								2,
								vertex_transfering_buf + vertices_count,
								&how_many_chars_used_in_parsing_func
								)
							==
							MY_TRUE
							)
						{
							if(
								line_buffer[how_many_chars_used_in_parsing_func + 2]
								==
								'\n'
								)
							{
								vertices_count++;
							}
							else
							{
								simplest_log(
									"parse_single_model_obj "
									"corrupted line, "
									"file corrupted, "
									"returning MY_FALSE, "
									"at line %d in file %s",
									line_count,
									path_to_obj_file
									);
								loop_running = MY_FALSE;
								err_in_loop = MY_TRUE;
							}
						}
						else
						{
							simplest_log(
								"parse_single_model_obj "
								"wrong content in line, "
								"file corrupted, "
								"returning MY_FALSE, "
								"at line %d in file %s",
								line_count,
								path_to_obj_file
								);
							loop_running = MY_FALSE;
							err_in_loop = MY_TRUE;
						}
					}
					else
					{
						simplest_log(
							"parse_single_model_obj "
							"more vertices than given "
							"by early analysis, "
							"file corrupted, "
							"returning MY_FALSE, "
							"at line %d in file %s",
							line_count,
							path_to_obj_file
							);
						loop_running = MY_FALSE;
						err_in_loop = MY_TRUE;
					}
				}
				else if(
					line_buffer[0] == 'v'
					&&
					line_buffer[1] == 'n'
					)
				{
					if(
						vertices_normals_count
						<
						normals_in_file_count
						)
					{
						if(
							line_buf_256_divided_by_spaces_to_column_vector_3_x_1_float(
								line_buffer,
								3,
								normals_transfering_buf +
									vertices_normals_count,
								&how_many_chars_used_in_parsing_func
								)
							==
							MY_TRUE
							)
						{
							if(
								line_buffer[how_many_chars_used_in_parsing_func + 3]
								==
								'\n'
								)
							{
								vertices_normals_count++;
							}
							else
							{
								simplest_log(
									"parse_single_model_obj "
									"corrupted line, "
									"file corrupted, "
									"returning MY_FALSE, "
									"at line %d in file %s",
									line_count,
									path_to_obj_file
									);
								loop_running = MY_FALSE;
								err_in_loop = MY_TRUE;
							}
						}
						else
						{
							simplest_log(
								"parse_single_model_obj "
								"wrong content in line, "
								"file corrupted, "
								"returning MY_FALSE, "
								"at line %d in file %s",
								line_count,
								path_to_obj_file
								);
							loop_running = MY_FALSE;
							err_in_loop = MY_TRUE;
						}
					}
					else
					{
						simplest_log(
							"parse_single_model_obj "
							"more vertices normals than given "
							"by early analysis, "
							"file corrupted, "
							"returning MY_FALSE, "
							"at line %d in file %s",
							line_count,
							path_to_obj_file
							);
						loop_running = MY_FALSE;
						err_in_loop = MY_TRUE;
					}
				}
				else if(
					line_buffer[0] == 'v'
					&&
					line_buffer[1] == 't'
					)
				{
					if(
						texture_coords_count
						<
						texture_coords_in_file_count
						)
					{
						if(
							line_buf_256_divided_by_spaces_to_column_vector_2_x_1_float(
								line_buffer,
								3,
								texture_coords_transfering_buf +
									texture_coords_count,
								&how_many_chars_used_in_parsing_func
								)
							==
							MY_TRUE
							)
						{
							if(
								line_buffer[how_many_chars_used_in_parsing_func + 3]
								==
								'\n'
								)
							{
								texture_coords_count++;
							}
							else
							{
								simplest_log(
									"parse_single_model_obj "
									"corrupted line, "
									"file corrupted, "
									"returning MY_FALSE, "
									"at line %d in file %s",
									line_count,
									path_to_obj_file
									);
								loop_running = MY_FALSE;
								err_in_loop = MY_TRUE;
							}
						}
						else
						{
							simplest_log(
								"parse_single_model_obj "
								"wrong content in line, "
								"file corrupted, "
								"returning MY_FALSE, "
								"at line %d in file %s",
								line_count,
								path_to_obj_file
								);
							loop_running = MY_FALSE;
							err_in_loop = MY_TRUE;
						}
					}
					else
					{
						simplest_log(
							"parse_single_model_obj "
							"more texture coords than given "
							"by early analysis, "
							"file corrupted, "
							"returning MY_FALSE, "
							"at line %d in file %s",
							line_count,
							path_to_obj_file
							);
						loop_running = MY_FALSE;
						err_in_loop = MY_TRUE;
					}
				}
				else if(
					line_buffer[0] == 'f'
					&&
					line_buffer[1] == ' '
					)
				{
					if(
						indices_triplets_count
						<
						indices_triplets_in_file_count
						)
					{
						if(
							line_buf_256_obj_style_indices_to_3_int_vectors(
								line_buffer,
								indices_transferring_buf +
									(indices_triplets_count * 3 + 0),
								indices_transferring_buf +
									(indices_triplets_count * 3 + 1),
								indices_transferring_buf +
									(indices_triplets_count * 3 + 2)
								)
							==
							MY_TRUE
							)
						{
							indices_triplets_count += 1;
						}
						else
						{
							simplest_log(
								"parse_single_model_obj "
								"corrupted line, "
								"file corrupted, "
								"returning MY_FALSE, "
								"at line %d in file %s",
								line_count,
								path_to_obj_file
								);
							loop_running = MY_FALSE;
							err_in_loop = MY_TRUE;
						}
					}
					else
					{
						simplest_log(
							"parse_single_model_obj "
							"more indices than given "
							"by early analysis, "
							"file corrupted, "
							"returning MY_FALSE, "
							"at line %d in file %s",
							line_count,
							path_to_obj_file
							);
						loop_running = MY_FALSE;
						err_in_loop = MY_TRUE;
					}
				}
				else
				{
					simplest_log(
						"parse_single_model_obj "
						"invalid line, "
						"file corrupted, "
						"returning MY_FALSE, "
						"at line %d in file %s",
						line_count,
						path_to_obj_file
						);
					loop_running = MY_FALSE;
					err_in_loop = MY_TRUE;
				}
			}// end of part 2 of loop
		}// end of second loop
		
		if(err_in_loop == MY_FALSE)
		{
			first_from_obj_checklist =
				data_loaded;
		}
	}

	if(first_from_obj_checklist == data_loaded)
	{
		//some naive distinct count
		//MUST be known exact number
		
		//this is the reason why
		//new format will be better
		
		indexation_phase_1 =
			calloc(
				indices_triplets_in_file_count * 3,
				sizeof(int)
				);
		
		if(indexation_phase_1 != NULL)
		{
			for(i = 0; i <indices_triplets_in_file_count * 3; i++)
			{
				*(indexation_phase_1 + i) = -1;
			}
			
			int cur_index = 0;
			
			for(i = 0; i < indices_triplets_in_file_count * 3; i++)
			{
				if(*(indexation_phase_1 + i) == -1)
				{
					for(j = i; j < indices_triplets_in_file_count * 3; j++)
					{
						if(*(indexation_phase_1 + j) == -1)// !
						{
							if(
								equality_2_column_vectors_3_x_1_int(
									*(indices_transferring_buf + i),
									*(indices_transferring_buf + j)
									)
								==
								MY_TRUE
								)
							{
								*(indexation_phase_1 + j) = cur_index;
							}
						}
					}
					
					cur_index += 1;
				}
			}
			
			// cur_index is distinct count
			vertices_distinct_count = cur_index;
			
			first_from_obj_checklist =
				indexation_phase_1_ok;
		}
		else
		{
			simplest_log(
				"parse_single_model_obj "
				"cannot calloc "
				"indexation_phase_1 "
				"returning MY_FALSE, "
				"in file %s",
				path_to_obj_file
				);
		}
	}

	if(first_from_obj_checklist == indexation_phase_1_ok)
	{
		// start of finishing
		result.num_of_vertices = vertices_distinct_count;
		result.num_of_indices_triplets = indices_triplets_in_file_count;
		
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
				first_from_obj_checklist =
					verts_and_indices_in_model_calloced;
			}
			else
			{
				simplest_log(
					"parse_single_model_obj "
					"cannot calloc "
					"result.indices_triplets "
					"returning MY_FALSE, "
					"in file %s",
					path_to_obj_file
					);
				free(result.vertices);
			}
		}
		else
		{
			simplest_log(
				"parse_single_model_obj "
				"cannot calloc "
				"result.vertices "
				"returning MY_FALSE, "
				"in file %s",
				path_to_obj_file
				);
		}
	}

	if(
		first_from_obj_checklist
		==
		verts_and_indices_in_model_calloced
		)
	{
		int cur_vert_index;
		union column_vector_3_x_1_int values_cur_obj_style_indice;
		
		//this approach choosen
		
		for(j = 0; j < result.num_of_indices_triplets; j++)
		{
			for(k = 0; k < 3; k++)
			{
				cur_vert_index =
					*(indexation_phase_1 + j * 3 + k); // value !
					
				(*(result.indices_triplets + j)).by_index[k] =
					int_to_uint_negatives_to_zero(
						cur_vert_index
						);
				
				//some doubling but its ok
				
				values_cur_obj_style_indice =
					*(indices_transferring_buf + j * 3 + k);
				
				(*(result.vertices + cur_vert_index)).
					position =
						*(
							vertex_transfering_buf
							+
							values_cur_obj_style_indice.by_index[0]
							-
							1
							);
							
				(*(result.vertices + cur_vert_index)).
					normals =
						*(
							normals_transfering_buf
							+
							values_cur_obj_style_indice.by_index[2]// !
							-
							1
							);
							
				(*(result.vertices + cur_vert_index)).
					texture_coordinates =
						*(
							texture_coords_transfering_buf
							+
							values_cur_obj_style_indice.by_index[1] // because in .obj its: f v/vt/vn ...
							-
							1
							);
			}
		}
		
		first_from_obj_checklist =
			values_written_to_model;
	}

	//general cleanup
	if(
		first_from_obj_checklist
		>=
		indexation_phase_1_ok
		)
	{
		free(indexation_phase_1);
	}
	
	if(
		first_from_obj_checklist
		>=
		transfer_buffers_alloced
		)
	{
		free(vertex_transfering_buf);
		free(normals_transfering_buf);
		free(texture_coords_transfering_buf);
		free(indices_transferring_buf);
	}
	
	if(
		first_from_obj_checklist
		>=
		file_opened
		)
	{
		fclose(file_to_read);
	}
	
	if(
		first_from_obj_checklist
		==
		values_written_to_model
		)
	{
		result.state = MY_TRUE;
		return result;
	}
	else
	{
		if(
			first_from_obj_checklist
			>=
			verts_and_indices_in_model_calloced
			)
		{
			free(result.vertices);
			free(result.indices_triplets);
		}
		
		if(
			first_from_obj_checklist
			>=
			name_alloced
			)
		{
			free(result.name);
		}
		
		
		
		return empty_model_3d();
	}
}

/*
		parse_single_model_obj_and_textures
			
			only first model in the file
*/
struct model_3d parse_single_model_obj_and_textures(
	char * path_to_obj_file,
	char * path_to_texture_file
	)
{
	enum obj_and_textures_checklist
	{
		initial_value,
		not_nulls,
		obj_ok,
		main_texture_ok
	} obj_and_textures_checklist =
		initial_value;
	
	struct model_3d result =
		empty_model_3d();
	
	if(obj_and_textures_checklist == initial_value)
	{
		if(
			path_to_obj_file != NULL
			&&
			path_to_texture_file != NULL
			)
		{
			obj_and_textures_checklist = not_nulls;
		}
		else
		{
			simplest_log(
				"parse_single_model_obj_and_textures "
				"provided NULL(s)"
				);
		}
	}
	
	if(obj_and_textures_checklist == not_nulls)
	{
		result = 
			parse_single_model_obj(
				path_to_obj_file,
				MY_FALSE
				);
		
		if(result.state == MY_TRUE)
		{
			obj_and_textures_checklist = obj_ok;
		}
		else
		{
			simplest_log(
				"parse_single_model_obj_and_textures "
				"cannot load model: %s",
				path_to_obj_file
				);
		}
	}
	
	if(obj_and_textures_checklist == obj_ok)
	{
		SDL_Surface * temp =
			IMG_Load(
				path_to_texture_file
				);
		
		if(temp != NULL)
		{
			result.texture =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(result.texture != NULL)
			{
				obj_and_textures_checklist =
					main_texture_ok;
			}
			else
			{
				simplest_log(
					"parse_single_model_"
					"obj_and_textures "
					"cannot convert texture %s",
					path_to_texture_file
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"parse_single_model_"
				"obj_and_textures "
				"cannot load texture %s",
				path_to_texture_file
				);
		}
	}
	
	if(obj_and_textures_checklist == main_texture_ok)
	{
		return result;
	}
	else
	{
		if(
			obj_and_textures_checklist
			>=
			main_texture_ok
			)
		{
			SDL_FreeSurface(result.texture);
		}
		
		if(
			obj_and_textures_checklist
			>=
			obj_ok
			)
		{
			deinit_model_3d(&result);
		}
		
		return empty_model_3d();
	}
}

/*
	static helpers functions definitions
*/
/*
		line_buf_256_divided_by_spaces_to_column_vector_3_x_1_float
*/
static int line_buf_256_divided_by_spaces_to_column_vector_3_x_1_float(
	char * line_buffer,
	int pos_of_start_of_first_float,
	union column_vector_3_x_1_float * where_to_return,
	int * here_returned_num_of_read_chars
	)
{
	if(
		line_buffer == NULL
		||
		pos_of_start_of_first_float < 0
		||
		where_to_return == NULL
		||
		here_returned_num_of_read_chars == NULL
		)
	{
		simplest_log(
			"line_buf_256_divided_by_spaces"
			"_to_column_vector_3_x_1_float "
			"wrong params provided, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	int cur_pos = pos_of_start_of_first_float;
	int loop_running = MY_TRUE, err_in_loop = MY_FALSE;
	int i = 0;
	int helper_01 = 0;
	
	while(loop_running == MY_TRUE && i < 3)
	{
		if(
			simplest_decimal_char_to_float_arbitrary_limits(
				&((*where_to_return).by_index[i]),
				line_buffer + cur_pos,
				MY_TRUE,
				&helper_01
			)
			==
			MY_TRUE
			)
		{
			cur_pos += helper_01;
		}
		else
		{
			simplest_log(
				"line_buf_256_divided_by_spaces"
				"_to_column_vector_3_x_1_float "
				"no correct float, "
				"results are not guaranted, "
				"returning MY_FALSE"
				);
			loop_running = MY_FALSE;
			err_in_loop = MY_TRUE;
		}
		
		if(loop_running == MY_TRUE && i < 2)
		{
			if(*(line_buffer + cur_pos) == ' ')
			{
				cur_pos++;
			}
			else
			{
				simplest_log(
					"line_buf_256_divided_by_spaces"
					"_to_column_vector_3_x_1_float "
					"wrong char when reading floats, "
					"results are not guaranted, "
					"returning MY_FALSE"
					);
				loop_running = MY_FALSE;
				err_in_loop = MY_TRUE;
			}
		}
		
		i++;
	}
	
	if(err_in_loop == MY_TRUE || cur_pos > 253)
	{
		(*here_returned_num_of_read_chars) = 0;
		if(cur_pos > 253)
			simplest_log(
				"line_buf_256_divided_by_spaces"
				"_to_column_vector_3_x_1_float "
				"line too long, "
				"results are not guaranted, "
				"returning MY_FALSE"
				);
		return MY_FALSE;
	}
	else
	{
		(*here_returned_num_of_read_chars) =
			cur_pos - pos_of_start_of_first_float;
						
		return MY_TRUE;
	}
}

/*
		line_buf_256_divided_by_spaces_to_column_vector_2_x_1_float
*/
static int line_buf_256_divided_by_spaces_to_column_vector_2_x_1_float(
	char * line_buffer,
	int pos_of_start_of_first_float,
	union column_vector_2_x_1_float * where_to_return,
	int * here_returned_num_of_read_chars
	)
{
	if(
		line_buffer == NULL
		||
		pos_of_start_of_first_float < 0
		||
		where_to_return == NULL
		||
		here_returned_num_of_read_chars == NULL
		)
	{
		simplest_log(
			"line_buf_256_divided_by_spaces"
			"_to_column_vector_2_x_1_float "
			"wrong params provided, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	int cur_pos = pos_of_start_of_first_float;
	int loop_running = MY_TRUE, err_in_loop = MY_FALSE;
	int i = 0;
	int helper_01 = 0;
	
	while(loop_running == MY_TRUE && i < 2)
	{
		if(
			simplest_decimal_char_to_float_arbitrary_limits(
				&((*where_to_return).by_index[i]),
				line_buffer + cur_pos,
				MY_TRUE,
				&helper_01
			)
			==
			MY_TRUE
			)
		{
			cur_pos += helper_01;
		}
		else
		{
			simplest_log(
				"line_buf_256_divided_by_spaces"
				"_to_column_vector_2_x_1_float "
				"no correct float, "
				"results are not guaranted, "
				"returning MY_FALSE"
				);
			loop_running = MY_FALSE;
			err_in_loop = MY_TRUE;
		}
		
		if(loop_running == MY_TRUE && i < 1)
		{
			if(*(line_buffer + cur_pos) == ' ')
			{
				cur_pos++;
			}
			else
			{
				simplest_log(
					"line_buf_256_divided_by_spaces"
					"_to_column_vector_2_x_1_float "
					"wrong char when reading floats, "
					"results are not guaranted, "
					"returning MY_FALSE"
					);
				loop_running = MY_FALSE;
				err_in_loop = MY_TRUE;
			}
		}
		
		i++;
	}
	
	if(err_in_loop == MY_TRUE || cur_pos > 253)
	{
		(*here_returned_num_of_read_chars) = 0;
		if(cur_pos > 253)
			simplest_log(
				"line_buf_256_divided_by_spaces"
				"_to_column_vector_2_x_1_float "
				"line too long, "
				"results are not guaranted, "
				"returning MY_FALSE"
				);
		return MY_FALSE;
	}
	else
	{
		(*here_returned_num_of_read_chars) =
			cur_pos - pos_of_start_of_first_float;
			
		return MY_TRUE;
	}
}

/*
		line_buf_256_obj_style_indices_to_3_int_vectors
*/
static int line_buf_256_obj_style_indices_to_3_int_vectors(
	char * line_buffer,
	union column_vector_3_x_1_int * where_to_put_indices_0,
	union column_vector_3_x_1_int * where_to_put_indices_1,
	union column_vector_3_x_1_int * where_to_put_indices_2
	)
{
	if(
		line_buffer == NULL
		||
		where_to_put_indices_0 == NULL
		||
		where_to_put_indices_1 == NULL
		||
		where_to_put_indices_2 == NULL
		)
	{
		simplest_log(
			"line_buf_256_obj_style_"
			"indices_to_3_int_vectors "
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
	
	if(
		*(line_buffer + 0) != 'f'
		||
		*(line_buffer + 1) != ' '
		)
	{
		simplest_log(
			"line_buf_256_obj_style_"
			"indices_to_3_int_vectors "
			"start of line not correct, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	int i, local_proceed, how_many_read = 0;
	int how_many_read_sum = 2;
	int error = MY_FALSE;
	
	union column_vector_3_x_1_int * cur_where_to_put =
		where_to_put_indices_0;
	
	for(i = 0; i < 3 && error == MY_FALSE; i++)
	{
		local_proceed = MY_TRUE;
		
		if(i == 1)
		{
			cur_where_to_put = where_to_put_indices_1;
		}
		else if(i == 2)
		{
			cur_where_to_put = where_to_put_indices_2;
		}
		
		if(
			simplest_decimal_char_to_int(
				&((*cur_where_to_put).by_index[0]),
				line_buffer + how_many_read_sum,
				MY_TRUE,
				&how_many_read
				)
			==
			MY_TRUE
			)
		{
			how_many_read_sum += how_many_read;
		}
		else
		{
			simplest_log(
				"line_buf_256_obj_style_"
				"indices_to_3_int_vectors "
				"incorrect int, "
				"returning MY_FALSE"
				);
			error = MY_TRUE;
			local_proceed = MY_FALSE;
		}
		
		
		if(local_proceed == MY_TRUE)
		{
			if(*(line_buffer + how_many_read_sum) == '/')
			{
				how_many_read_sum++;
			}
			else if(
				*(line_buffer + how_many_read_sum)
				==
				' '
				)
			{
				simplest_log(
					"line_buf_256_obj_style_"
					"indices_to_3_int_vectors "
					"only 3-entry indices supported, "
					"convert model in blender, "
					"returning MY_FALSE"
					);
				error = MY_TRUE;
				local_proceed = MY_FALSE;
			}
			else
			{
				simplest_log(
					"line_buf_256_obj_style_"
					"indices_to_3_int_vectors "
					"unknown sign, "
					"returning MY_FALSE"
					);
				error = MY_TRUE;
				local_proceed = MY_FALSE;
			}
		}
		
		if(local_proceed == MY_TRUE)
		{
			if(
				simplest_decimal_char_to_int(
					&((*cur_where_to_put).by_index[1]),
					line_buffer + how_many_read_sum,
					MY_TRUE,
					&how_many_read
					)
				==
				MY_TRUE
				)
			{
				how_many_read_sum += how_many_read;
			}
			else
			{
				simplest_log(
					"line_buf_256_obj_style_"
					"indices_to_3_int_vectors "
					"incorrect int, "
					"returning MY_FALSE"
					);
				error = MY_TRUE;
				local_proceed = MY_FALSE;
			}
		}
		
		if(local_proceed == MY_TRUE)
		{
			if(*(line_buffer + how_many_read_sum) == '/')
			{
				how_many_read_sum++;
			}
			else if(
				*(line_buffer + how_many_read_sum)
				==
				' '
				)
			{
				simplest_log(
					"line_buf_256_obj_style_"
					"indices_to_3_int_vectors "
					"only 3-entry indices supported, "
					"convert model in blender, "
					"returning MY_FALSE"
					);
				error = MY_TRUE;
				local_proceed = MY_FALSE;
			}
			else
			{
				simplest_log(
					"line_buf_256_obj_style_"
					"indices_to_3_int_vectors "
					"unknown sign, "
					"returning MY_FALSE"
					);
				error = MY_TRUE;
				local_proceed = MY_FALSE;
			}
		}
		
		if(local_proceed == MY_TRUE)
		{
			if(
				simplest_decimal_char_to_int(
					&((*cur_where_to_put).by_index[2]),
					line_buffer + how_many_read_sum,
					MY_TRUE,
					&how_many_read
					)
				==
				MY_TRUE
				)
			{
				how_many_read_sum += how_many_read;
			}
			else
			{
				simplest_log(
					"line_buf_256_obj_style_"
					"indices_to_3_int_vectors "
					"incorrect int, "
					"returning MY_FALSE"
					);
				error = MY_TRUE;
				local_proceed = MY_FALSE;
			}
		}
		
		if(local_proceed == MY_TRUE)
		{
			if(i < 2)
			{
				if(
					*(line_buffer + how_many_read_sum)
					==
					' '
					)
				{
					how_many_read_sum++;
				}
				else
				{
					simplest_log(
						"line_buf_256_obj_style_"
						"indices_to_3_int_vectors "
						"wrong sign, "
						"returning MY_FALSE"
						);
					error = MY_TRUE;
					local_proceed = MY_FALSE;
				}
			}
			else if(i == 2)
			{
				if(
					*(line_buffer + how_many_read_sum)
					==
					'\n'
					)
				{
					how_many_read_sum++;
					ULTRA_STRICT_LOG(
						"line_buf_256_obj_style_"
						"indices_to_3_int_vectors "
						"correct triangle read"
						)
				}
				else
				{
					simplest_log(
						"line_buf_256_obj_style_"
						"indices_to_3_int_vectors "
						"wrong sign, "
						"returning MY_FALSE"
						);
					error = MY_TRUE;
					local_proceed = MY_FALSE;
				}
			}
			else
			{
				;//shouldnt happen
			}
		}
	}//end of loop
	
	if(how_many_read_sum > 255)
	{
		simplest_log(
			"line_buf_256_obj_style_"
			"indices_to_3_int_vectors "
			"line too long, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	if(error == MY_FALSE)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}
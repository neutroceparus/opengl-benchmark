//	Copyright (c) 2025 Paul Jankowski

#ifndef PHASE_02_MODELS_FILES
#define PHASE_02_MODELS_FILES

#include "global_defines.h"
#include "text_constants.h"

#define NUM_OF_MODELS 27

/*
	in this file is hardcoding
*/
#define LOAD_CURRENT_MODEL \
	if(cur_model == 0) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "hero_mountain.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "hero_mountain_base_color.png", \
			PATH_TO_ASSETS_FOLDER "hero_mountain_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 5); \
	} \
	else if(cur_model == 1) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "rostlinka_01.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "rostlinka_01_base_color.png", \
			PATH_TO_ASSETS_FOLDER "rostlinka_01_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.17); \
	} \
	else if(cur_model == 2) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "rostlinka_02.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "rostlinka_02_base_color.png", \
			PATH_TO_ASSETS_FOLDER "rostlinka_02_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.07); \
	} \
	else if(cur_model == 3) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "high_poly_tree_01.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_base_color.png", \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.040); \
	} \
	else if(cur_model == 4) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "high_poly_tree_02.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_base_color.png", \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.040); \
	} \
	else if(cur_model == 5) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "high_poly_tree_03.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_base_color.png", \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.040); \
	} \
	else if(cur_model == 6) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "high_poly_tree_leaves.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_leaves_base_color.png", \
			PATH_TO_ASSETS_FOLDER "high_poly_tree_leaves_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.040); \
	} \
	else if(cur_model == 7) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_hull_01.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_hull_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_hull_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 8) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_hull_02.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_hull_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_hull_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 9) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_tracks_01.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 10) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_tracks_02.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 11) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_tracks_03.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 12) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_tracks_04.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_tracks_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 13) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_turret.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_turret_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_turret_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 14) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_wheels_01.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_wheels_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_wheels_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 15) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "kv_2_wheels_02.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "kv_2_wheels_base_color.png", \
			PATH_TO_ASSETS_FOLDER "kv_2_wheels_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.126228); \
	} \
	else if(cur_model == 16) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "broadcast_tower_part_1.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "broadcast_tower_part_1_base_color.png", \
			PATH_TO_ASSETS_FOLDER "broadcast_tower_part_1_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.000057); \
	} \
	else if(cur_model == 17) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "broadcast_tower_part_2.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "broadcast_tower_part_2_base_color.png", \
			PATH_TO_ASSETS_FOLDER "broadcast_tower_part_2_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.000057); \
	} \
	else if(cur_model == 18) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "fantasy_castle_part_1.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "fantasy_castle_main_texture.png", \
			PATH_TO_ASSETS_FOLDER "default_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.009090); \
	} \
	else if(cur_model == 19) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "fantasy_castle_part_2.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "fantasy_castle_wind_frame_texture.png", \
			PATH_TO_ASSETS_FOLDER "default_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.009090); \
	} \
	else if(cur_model == 20) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "fantasy_castle_part_3.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "fantasy_castle_castle_base_texture.png", \
			PATH_TO_ASSETS_FOLDER "default_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.009090); \
	} \
	else if(cur_model == 21) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "fantasy_castle_part_4.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "fantasy_castle_roofs_texture.png", \
			PATH_TO_ASSETS_FOLDER "default_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.009090); \
	} \
	else if(cur_model == 22) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "fantasy_castle_part_5.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "fantasy_castle_castle_base_texture.png", \
			PATH_TO_ASSETS_FOLDER "default_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.009090); \
	} \
	else if(cur_model == 23) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "knight_part_1.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "knight_part_1_base_color.png", \
			PATH_TO_ASSETS_FOLDER "knight_part_1_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.000392); \
	} \
	else if(cur_model == 24) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "knight_part_2.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "knight_part_2_base_color.png", \
			PATH_TO_ASSETS_FOLDER "knight_part_2_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.000392); \
	} \
	else if(cur_model == 25) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "riot_mech_down.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "riot_mech_down_base_color.png", \
			PATH_TO_ASSETS_FOLDER "riot_mech_down_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.006704); \
	} \
	else if(cur_model == 26) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER "riot_mech_up.model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER "riot_mech_up_base_color.png", \
			PATH_TO_ASSETS_FOLDER "riot_mech_up_normal_map.png" \
			); \
		model_3d_uniform_scale(&(context.model), 0.006704); \
	} \
	else \
	{ \
		simplest_log( \
			"LOAD_CURRENT_MODEL " \
			"cur_model_out_of_range" \
			); \
		signal_error(); \
		local_proceed = MY_FALSE; \
		thread_loop_running = MY_FALSE; \
	}
	
/*
	else if(cur_model == ) \
	{ \
		context.model = \
			parse_single_custom_model_3d( \
				PATH_TO_ASSETS_FOLDER ".model_3d" \
				); \
		model_3d_append_textures( \
			&(context.model), \
			PATH_TO_ASSETS_FOLDER ".png", \
			PATH_TO_ASSETS_FOLDER ".png" \
			); \
		model_3d_uniform_scale(&(context.model), 1); \
	}
*/

/*
	here triangles
*/
#define FILL_TRIANGLES_TABLE \
	result.triangles_numbers[0] = 522242; \
	result.triangles_numbers[1] = 18478; \
	result.triangles_numbers[2] = 4551; \
	result.triangles_numbers[3] = 4551; \
	result.triangles_numbers[4] = 82389; \
	result.triangles_numbers[5] = 41435; \
	result.triangles_numbers[6] = 51990; \
	result.triangles_numbers[7] = 39206; \
	result.triangles_numbers[8] = 71207; \
	result.triangles_numbers[9] = 46802; \
	result.triangles_numbers[10] = 102967; \
	result.triangles_numbers[11] = 102602; \
	result.triangles_numbers[12] = 102929; \
	result.triangles_numbers[13] = 15552; \
	result.triangles_numbers[14] = 33771; \
	result.triangles_numbers[15] = 68345; \
	result.triangles_numbers[16] = 1804; \
	result.triangles_numbers[17] = 12006; \
	result.triangles_numbers[18] = 3102; \
	result.triangles_numbers[19] = 1726; \
	result.triangles_numbers[20] = 5532; \
	result.triangles_numbers[21] = 9926; \
	result.triangles_numbers[22] = 3952; \
	result.triangles_numbers[23] = 33374; \
	result.triangles_numbers[24] = 55252; \
	result.triangles_numbers[25] = 46849; \
	result.triangles_numbers[26] = 36570;
	//result.triangles_numbers[] = ;

/*
	positions
*/
#define FILL_MODELS_LOCATIONS_TABLE \
	result.models_locations[0] = \
		null_column_vector_3_x_1_float(); \
	result.models_locations[1] = \
		column_vector_3_x_1_float_direct_values(-0.310001, 0.345000, 6.630060); \
	result.models_locations[2] = \
		column_vector_3_x_1_float_direct_values(-0.124001, 0.347000, 6.825066); \
	result.models_locations[3] = \
		column_vector_3_x_1_float_direct_values(-0.244002, 0.336000, 6.571822); \
	result.models_locations[4] = \
		column_vector_3_x_1_float_direct_values(-0.244002, 0.336000, 6.571822); \
	result.models_locations[5] = \
		column_vector_3_x_1_float_direct_values(-0.244002, 0.336000, 6.571822); \
	result.models_locations[6] = \
		column_vector_3_x_1_float_direct_values(-0.244002, 0.336000, 6.571822); \
	result.models_locations[7] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[8] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[9] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[10] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[11] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[12] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[13] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[14] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[15] = \
		column_vector_3_x_1_float_direct_values(0.160000, 0.390001, 6.700039); \
	result.models_locations[16] = \
		column_vector_3_x_1_float_direct_values(1.159999, 1.079999, 3.159998); \
	result.models_locations[17] = \
		column_vector_3_x_1_float_direct_values(1.159999, 1.079999, 3.159998); \
	result.models_locations[18] = \
		column_vector_3_x_1_float_direct_values(-0.750000, 0.919999, 2.409998); \
	result.models_locations[19] = \
		column_vector_3_x_1_float_direct_values(-0.750000, 0.919999, 2.409998); \
	result.models_locations[20] = \
		column_vector_3_x_1_float_direct_values(-0.750000, 0.919999, 2.409998); \
	result.models_locations[21] = \
		column_vector_3_x_1_float_direct_values(-0.750000, 0.919999, 2.409998); \
	result.models_locations[22] = \
		column_vector_3_x_1_float_direct_values(-0.750000, 0.919999, 2.409998); \
	result.models_locations[23] = \
		column_vector_3_x_1_float_direct_values(-0.130000, 0.390000, 6.340051); \
	result.models_locations[24] = \
		column_vector_3_x_1_float_direct_values(-0.130000, 0.390000, 6.340051); \
	result.models_locations[25] = \
		column_vector_3_x_1_float_direct_values(-0.260000, 0.370000, 6.190047); \
	result.models_locations[26] = \
		column_vector_3_x_1_float_direct_values(-0.260000, 0.370000, 6.190047);
	//result.models_locations[] =
	//	column_vector_3_x_1_float_direct_values(0,0,0);
		
/*
	rotations
*/
#define FILL_ROTATIONS_TABLE \
	result.models_rotations[0] = 135.000305; \
	result.models_rotations[1] = 0; \
	result.models_rotations[2] = 0; \
	result.models_rotations[3] = 0; \
	result.models_rotations[4] = 0; \
	result.models_rotations[5] = 0; \
	result.models_rotations[6] = 0; \
	result.models_rotations[7] = -55.399727; \
	result.models_rotations[8] = -55.399727; \
	result.models_rotations[9] = -55.399727; \
	result.models_rotations[10] = -55.399727; \
	result.models_rotations[11] = -55.399727; \
	result.models_rotations[12] = -55.399727; \
	result.models_rotations[13] = -55.399727; \
	result.models_rotations[14] = -55.399727; \
	result.models_rotations[15] = -55.399727; \
	result.models_rotations[16] = 0; \
	result.models_rotations[17] = 0; \
	result.models_rotations[18] = -11.900002; \
	result.models_rotations[19] = -11.900002; \
	result.models_rotations[20] = -11.900002; \
	result.models_rotations[21] = -11.900002; \
	result.models_rotations[22] = -11.900002; \
	result.models_rotations[23] = 38; \
	result.models_rotations[24] = 38; \
	result.models_rotations[25] = 31.300083; \
	result.models_rotations[26] = 31.300083;
	//result.models_rotations[] = 0;

/*
	face culling
*/
#define FILL_FACE_CULLING_READINESS \
	result.face_culling_readiness[0] = MY_TRUE; \
	result.face_culling_readiness[1] = MY_FALSE; \
	result.face_culling_readiness[2] = MY_FALSE; \
	result.face_culling_readiness[3] = MY_FALSE; \
	result.face_culling_readiness[4] = MY_FALSE; \
	result.face_culling_readiness[5] = MY_TRUE; \
	result.face_culling_readiness[6] = MY_FALSE; \
	result.face_culling_readiness[7] = MY_TRUE; \
	result.face_culling_readiness[8] = MY_TRUE; \
	result.face_culling_readiness[9] = MY_TRUE; \
	result.face_culling_readiness[10] = MY_TRUE; \
	result.face_culling_readiness[11] = MY_TRUE; \
	result.face_culling_readiness[12] = MY_TRUE; \
	result.face_culling_readiness[13] = MY_TRUE; \
	result.face_culling_readiness[14] = MY_TRUE; \
	result.face_culling_readiness[15] = MY_TRUE; \
	result.face_culling_readiness[16] = MY_TRUE; \
	result.face_culling_readiness[17] = MY_TRUE; \
	result.face_culling_readiness[18] = MY_TRUE; \
	result.face_culling_readiness[19] = MY_TRUE; \
	result.face_culling_readiness[20] = MY_TRUE; \
	result.face_culling_readiness[21] = MY_TRUE; \
	result.face_culling_readiness[22] = MY_TRUE; \
	result.face_culling_readiness[23] = MY_TRUE; \
	result.face_culling_readiness[24] = MY_TRUE; \
	result.face_culling_readiness[25] = MY_TRUE; \
	result.face_culling_readiness[26] = MY_TRUE;
	//result.face_culling_readiness[] = MY_TRUE;

/*
	render order
*/
#define FILL_RENDER_ORDER \
	result.render_order[0] = 0; \
	result.render_order[1] = 18; \
	result.render_order[2] = 20; \
	result.render_order[3] = 22; \
	result.render_order[4] = 21; \
	result.render_order[5] = 19; \
	result.render_order[6] = 16; \
	result.render_order[7] = 17; \
	result.render_order[8] = 25; \
	result.render_order[9] = 26; \
	result.render_order[10] = 23; \
	result.render_order[11] = 24; \
	result.render_order[12] = 7; \
	result.render_order[13] = 8; \
	result.render_order[14] = 9; \
	result.render_order[15] = 10; \
	result.render_order[16] = 11; \
	result.render_order[17] = 12; \
	result.render_order[18] = 13; \
	result.render_order[19] = 14; \
	result.render_order[20] = 15; \
	result.render_order[21] = 3; \
	result.render_order[22] = 4; \
	result.render_order[23] = 5; \
	result.render_order[24] = 6; \
	result.render_order[25] = 1; \
	result.render_order[26] = 2;
	//result.render_order[] = ;

#endif //PHASE_02_MODELS_FILES
//	Copyright (c) 2025 Paul Jankowski

#include "displays_utils.h"

/*
definitions
*/
/*
	initialization
*/
/*
		prepare_empty_displays_config
*/
struct displays_config \
	prepare_empty_displays_config(
		void
		)
{
	struct displays_config result;
	
	result.num_of_displays = -1;
	result.positions_and_dims = NULL;
	
	return result;
}

/*
		init_displays_config
			
			for ease its made to run it first,
			then updating with update_displays_config
			if needed
*/
struct displays_config \
	init_displays_config(
		void
		)
{
	struct displays_config result =
		prepare_empty_displays_config();
	
	int num_disp = SDL_GetNumVideoDisplays();
	
	if(num_disp < 0)
	{
		simplest_log(
			"init_displays_config "
			"error while retrieving displays"
			);
		result.num_of_displays = -1;
		return result;
	}
	
	if(num_disp == 0)
	{
		result.num_of_displays = 0;
		return result;
	}
	
	result.positions_and_dims =
		calloc(num_disp, sizeof(SDL_Rect));
	
	if(result.positions_and_dims == NULL)
	{
		simplest_log(
			"init_displays_config "
			"cannot calloc"
			);
		return prepare_empty_displays_config();
	}
	
	int i, err_in_loop = -1;
	for(i = 0; i < num_disp && err_in_loop == -1; i++)
	{
		if(
			SDL_GetDisplayBounds(
				i,
				result.positions_and_dims + i
				)
			==
			0
			)
		{
			;
		}
		else
		{
			err_in_loop = i;
		}
	}
	
	if(err_in_loop == -1)
	{
		result.num_of_displays = num_disp;
		return result;
	}
	else
	{
		simplest_log(
			"init_displays_config "
			"error while retrieving dims"
			);
		free(result.positions_and_dims);
		return prepare_empty_displays_config();
	}
}

/*
	updating
*/
/*
		update_displays_config
		
			for ease its made to update,
			earlier it should be run
			init_displays_config
*/
int update_displays_config(
	struct displays_config * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"update_displays_config "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	int new_num;
	SDL_Rect * new_dims;
	
	new_num = SDL_GetNumVideoDisplays();
	
	if(new_num < 0)
	{
		simplest_log(
			"update_displays_config "
			"error while retrieving displays"
			);
		return MY_FALSE;
	}
	
	if(new_num == 0)
	{
		if((*context).num_of_displays > 0)
		{
			free((*context).positions_and_dims);
			(*context).positions_and_dims = NULL;
		}
		(*context).num_of_displays = 0;
		return MY_TRUE;
	}
	
	new_dims =
		calloc(new_num, sizeof(SDL_Rect));
	
	if(new_dims == NULL)
	{
		simplest_log(
			"update_displays_config "
			"error while calloc"
			);
		return MY_FALSE;
	}
	
	int i, err_in_loop = -1;
	for(i = 0; i < new_num && err_in_loop == -1; i++)
	{
		if(
			SDL_GetDisplayBounds(
				i,
				new_dims + i
				)
			==
			0
			)
		{
			;
		}
		else
		{
			err_in_loop = i;
		}
	}
	
	if(err_in_loop == -1)
	{
		if((*context).num_of_displays > 0)
		{
			free((*context).positions_and_dims);
		}
		
		(*context).positions_and_dims = new_dims;
		(*context).num_of_displays = new_num;
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"update_displays_config "
			"error while retrieving dims"
			);
		free(new_dims);
		return MY_FALSE;
	}
}

/*
	cleanup
*/
/*
		displays_config_cleanup
			
			remember to run it at program end
*/
void displays_config_cleanup(
	struct displays_config * what_to_clean
	)
{
	if(what_to_clean == NULL)
	{
		simplest_log(
			"displays_config_cleanup "
			"provided NULL"
			);
	}
	else
	{
		if((*what_to_clean).num_of_displays > 0)
		{
			free((void *) (*what_to_clean).positions_and_dims);
		}
		
		(*what_to_clean).positions_and_dims = NULL;
		(*what_to_clean).num_of_displays = -1;
	}
}

/*
	utils
*/
/*
		displays_check_800_600
			
			all checked in loop
*/
int displays_check_800_600(
	struct displays_config * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"displays_check_800_600 "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).num_of_displays < 0)
	{
		simplest_log(
			"displays_check_800_600 "
			"num_of_displays < 0"
			);
		return MY_FALSE;
	}
	
	if((*context).num_of_displays == 0)
	{
		return MY_FALSE;
	}
	
	int i, condition_not_met = MY_FALSE;
	
	for(
		i = 0;
			i < (*context).num_of_displays
			&&
			condition_not_met == MY_FALSE;
		i++
		)
	{
		if(
			(*((*context).positions_and_dims)).w < 800
			||
			(*((*context).positions_and_dims)).h < 600
			)
		{
			condition_not_met = MY_TRUE;
		}
	}
	
	if(condition_not_met == MY_FALSE)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		x_display_by_index
*/
int x_display_by_index(
	struct displays_config * context,
	unsigned int index
	)
{
		if(context == NULL)
	{
		simplest_log(
			"x_display_by_index "
			"provided NULL"
			);
		return -1;
	}
	
	if((*context).num_of_displays <= 0)
	{
		simplest_log(
			"x_display_by_index "
			"no displays"
			);
		return -1;
	}
	
	if(index >= 0 && index < (*context).num_of_displays)
	{
		return (*((*context).positions_and_dims + index)).x;
	}
	else
	{
		simplest_log(
			"x_display_by_index "
			"wrong index"
			);
		return -1;
	}
}
	
/*
		y_display_by_index
*/
int y_display_by_index(
	struct displays_config * context,
	unsigned int index
	)
{
		if(context == NULL)
	{
		simplest_log(
			"y_display_by_index "
			"provided NULL"
			);
		return -1;
	}
	
	if((*context).num_of_displays <= 0)
	{
		simplest_log(
			"y_display_by_index "
			"no displays"
			);
		return -1;
	}
	
	if(index >= 0 && index < (*context).num_of_displays)
	{
		return (*((*context).positions_and_dims + index)).y;
	}
	else
	{
		simplest_log(
			"y_display_by_index "
			"wrong index"
			);
		return -1;
	}
}

/*
		width_display_by_index
*/
int width_display_by_index(
	struct displays_config * context,
	unsigned int index
	)
{
	if(context == NULL)
	{
		simplest_log(
			"width_display_by_index "
			"provided NULL"
			);
		return -1;
	}
	
	if((*context).num_of_displays <= 0)
	{
		simplest_log(
			"width_display_by_index "
			"no displays"
			);
		return -1;
	}
	
	if(index >= 0 && index < (*context).num_of_displays)
	{
		return (*((*context).positions_and_dims + index)).w;
	}
	else
	{
		simplest_log(
			"width_display_by_index "
			"wrong index"
			);
		return -1;
	}
}

/*
		height_display_by_index
*/
int height_display_by_index(
	struct displays_config * context,
	unsigned int index
	)
{
	if(context == NULL)
	{
		simplest_log(
			"height_display_by_index "
			"provided NULL"
			);
		return -1;
	}
	
	if((*context).num_of_displays <= 0)
	{
		simplest_log(
			"height_display_by_index "
			"no displays"
			);
		return -1;
	}
	
	if(index >= 0 && index < (*context).num_of_displays)
	{
		return (*((*context).positions_and_dims + index)).h;
	}
	else
	{
		simplest_log(
			"height_display_by_index "
			"wrong index"
			);
		return -1;
	}
}
//	Copyright (c) 2025 Paul Jankowski

#include "system_info.h"

/*
		get_processor_name
*/

int get_processor_name(
	char * buffer_at_least_64
	)
{
	if(buffer_at_least_64 == NULL)
	{
		simplest_log(
			"get_processor_name "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	int i;
	for(i = 0; i < 64; i++)
	{
		*(buffer_at_least_64 + i) = '\0';
	}
	
#if defined _WIN32
	unsigned long int buf_size = 64;
	if(
		RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
			"ProcessorNameString",
			RRF_RT_REG_SZ,
			NULL,
			buffer_at_least_64,
			&(buf_size)
			)
		==
		ERROR_SUCCESS
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"get_processor_name "
			"cannot retrieve from "
			"RegGetValueA"
			);
		return MY_FALSE;
	}
#else
	
	FILE * stream_for_checking = popen(
		"lscpu | grep 'Model name' | cut -f 2 -d \":\" | awk '{$1=$1}1'", "r"
		);

	if(stream_for_checking != NULL)
	{
		char c = fgetc(stream_for_checking);
		int i = 0;
		while(c != EOF && c != '\n')
		{
			*(buffer_at_least_64 + i) = c;
			i+= 1;
			
			c = fgetc(stream_for_checking);
		}
		
		pclose(stream_for_checking);
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"get_processor_name "
			"cannot retrieve from "
			"lscpu"
			);
		return MY_FALSE;
	}

#endif

}

/*
		get_physical_ram_kilobytes
*/
#if defined _WIN32
unsigned long long int get_physical_ram_kilobytes(void)
#else
unsigned long long int get_physical_ram_kilobytes(
	const char * relative_script_location_if_not_current_folder_or_null
	)
#endif
{
	
	unsigned long long int result = 0ull;
	
	#if defined _WIN32
	if(
		GetPhysicallyInstalledSystemMemory(
			&(result)
			)
		==
		TRUE //winapi bool
		)
	{
		;
	}
	else
	{
		simplest_log(
			"get_physical_ram_kilobytes "
			"cannot retrieve from "
			"GetPhysicallyInstalledSystemMemory"
			);
	}
	
	#else
	// ./assets/memory.sh
	enum get_physical_ram_kilobytes_linux_checklist
	{
		initial_value = 0,
		path_to_script,
		exists,
		guid_ok,
		number_ok
	} get_physical_ram_kilobytes_linux_checklist =
		initial_value;
	
	char * buf = NULL;
	int use_default_path = MY_FALSE;
	
	FILE * output_from_script = NULL;
	
	if(
		get_physical_ram_kilobytes_linux_checklist
		==
		initial_value
		)
	{
		if(
			relative_script_location_if_not_current_folder_or_null
			!=
			NULL
			)
		{
			buf = calloc(
				strlen(relative_script_location_if_not_current_folder_or_null)
				+
				11,
				sizeof(char)
				);
			
			if(buf != NULL)
			{
				strcpy(buf, relative_script_location_if_not_current_folder_or_null);
				strcat(buf, "memory.sh");
				
				get_physical_ram_kilobytes_linux_checklist =
					path_to_script;
			}
			else
			{
				simplest_log(
					"get_physical_ram_kilobytes "
					"cannot alloc place for path "
					"trying default location "
					);
				
				get_physical_ram_kilobytes_linux_checklist =
					path_to_script;
				
				use_default_path = MY_TRUE;
			}
		}
		else
		{
			get_physical_ram_kilobytes_linux_checklist =
				path_to_script;
			
			use_default_path = MY_TRUE;
		}
	}
	
	if(
		get_physical_ram_kilobytes_linux_checklist
		==
		path_to_script
		)
	{
		FILE * checking = NULL;
		
		if(use_default_path == MY_FALSE)
		{
			checking = fopen(buf, "rb");
		}
		else
		{
			checking = fopen("./memory.sh", "rb");
		}
		
		if(checking != NULL)
		{
			get_physical_ram_kilobytes_linux_checklist =
				exists;
				
			fclose(checking);
		}
		else
		{
			simplest_log(
				"get_physical_ram_kilobytes "
				"seems that file not exists: %s",
				use_default_path == MY_FALSE ? buf : "memory.sh"
				);
		}
	}
	
	if(
		get_physical_ram_kilobytes_linux_checklist
		==
		exists
		)
	{
		if(use_default_path == MY_FALSE)
		{
			output_from_script = popen(buf, "r");
		}
		else
		{
			output_from_script = popen("./memory.sh", "r");
		}
		
		if(output_from_script != NULL)
		{
			char guid_check_buf[37];
			int i;
			for(i = 0; i < 37; i++)
			{
				guid_check_buf[i] = '\0';
			}
			
			char c = fgetc(output_from_script);
			for(i = 0; i < 37 && isalnum_or_hyphen_ascii_2d(c) == MY_TRUE ; i++)
			{
				guid_check_buf[i] = c;
				c = fgetc(output_from_script);
			}
			
			if(c == '\n')
			{
				if(
					strcmp(
						guid_check_buf,
						"0ca24ee1-ec84-4da0-99ec-772ae82b4cce"
						)
					==
					0
					)
				{
					get_physical_ram_kilobytes_linux_checklist =
						guid_ok;
				}
				else
				{
					simplest_log(
						"get_physical_ram_kilobytes "
						"guid of script is wrong"
						);
				}
			}
			else
			{
				simplest_log(
					"get_physical_ram_kilobytes "
					"guid of script doesnt ends with newline "
					"or script not found"
					);
			}
		}
		else
		{
			simplest_log(
				"get_physical_ram_kilobytes "
				"cannot run script %s",
				use_default_path == MY_FALSE ? buf : "memory.sh"
				);
		}
	}
	
	if(
		get_physical_ram_kilobytes_linux_checklist
		==
		guid_ok
		)
	{
		char num_buf[21];//see ULLONG_MAX
		
		int i;
		for(i = 0; i < 21; i++)
		{
			num_buf[i] = '\0';
		}
		
		i = 0;
		char c = fgetc(output_from_script);
		while(isdigit(c) != 0 && i < 21)
		{
			num_buf[i] = c;
			i += 1;
			c = fgetc(output_from_script);
		}
		
		if(i < 21)
		{
			if(
				simplest_decimal_char_to_ullint(
					&result,
					num_buf
					)
				==
				MY_TRUE
				)
			{
				get_physical_ram_kilobytes_linux_checklist =
					number_ok;
			}
			else
			{
				simplest_log(
					"get_physical_ram_kilobytes "
					"cannot read value from script"
					);
			}
		}
		else
		{
			simplest_log(
				"get_physical_ram_kilobytes "
				"too much numbers from script means error"
				);
		}
	}
	
	//cleanup
	if(output_from_script != NULL)
	{
		pclose(output_from_script);
	}
	
	if(
			get_physical_ram_kilobytes_linux_checklist
			==
			path_to_script
		&&
			relative_script_location_if_not_current_folder_or_null
			!=
			NULL
		&&
			buf != NULL
		)
	{
		free(buf);
	}
	
	if(
		get_physical_ram_kilobytes_linux_checklist
		==
		number_ok
		)
	{
		result /= 1024;
		
	#endif
	
		return result;
	
	#if !(defined _WIN32)
	}
	else
	{
		simplest_log(
			"get_physical_ram_kilobytes "
			"cannot retrieve exact value "
			"trying to use approx"
			);
		return get_physical_ram_kilobytes_approx();
	}
	#endif
}

#if !(defined _WIN32)
	
/*
		get_physical_ram_kilobytes_approx
*/
unsigned long long int get_physical_ram_kilobytes_approx(void)
{

	unsigned long long int result = 0ull;
	
	FILE * command_output =
		popen("grep MemTotal /proc/meminfo", "r");
	
	if(command_output != NULL)
	{
		char c = fgetc(command_output);
		
		//to first digit
		while(isdigit(c) == 0 && c != '\0' && c != EOF)
		{
			c = fgetc(command_output);
		}
		
		if(isdigit(c) != 0)
		{
			char num_buf[21];//see ULLONG_MAX
			
			int i;
			for(i = 0; i < 21; i++)
			{
				num_buf[i] = '\0';
			}
			
			i = 0;
			
			while(isdigit(c) != 0 && i < 21)
			{
				num_buf[i] = c;
				i += 1;
				c = fgetc(command_output);
			}
			
			if(i < 21)
			{
				if(
					simplest_decimal_char_to_ullint(
						&result,
						num_buf
						)
					==
					MY_TRUE
					)
				{
					pclose(command_output);
					return result;
				}
				else
				{
					simplest_log(
						"get_physical_ram_kilobytes_approx "
						"cannot read value from command"
						);
				}
			}
			else
			{
				simplest_log(
					"get_physical_ram_kilobytes_approx "
					"too much numbers in command means error"
					);
			}
		}
		else
		{
			simplest_log(
				"get_physical_ram_kilobytes_approx "
				"no numbers in command output means error"
				);
		}
		
		pclose(command_output);
	}
	else
	{
		simplest_log(
			"get_physical_ram_kilobytes_approx "
			"no numbers in command output means error"
			);
	}
	
	return 0;
}

#endif

/*
		print_display_devices_to_file_handle
*/
int print_display_devices_to_file_handle(
	FILE * handle_to_a_opened_file
	)
{
	if(handle_to_a_opened_file == NULL)
	{
		simplest_log(
			"print_display_devices_to_file_handle "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
#if defined _WIN32
	DISPLAY_DEVICEA device_info_struct;
	
	ZeroMemory(&device_info_struct, sizeof(device_info_struct));
	device_info_struct.cb = sizeof(device_info_struct);
	
	unsigned long int device_index = 0;
	
	fprintf(handle_to_a_opened_file, "\nDisplay devices:\n\n");
	
	while (EnumDisplayDevicesA(NULL, device_index, &device_info_struct, 0) != 0)
	{
		fprintf(
			handle_to_a_opened_file,
			"%s\n%s\n\n",
			device_info_struct.DeviceName,
			device_info_struct.DeviceString
			);
		
		DISPLAY_DEVICEA device_info_struct_for_subchecking;
		
		ZeroMemory(&device_info_struct_for_subchecking, sizeof(device_info_struct_for_subchecking));
		device_info_struct_for_subchecking.cb = sizeof(device_info_struct_for_subchecking);
		
		unsigned long int sub_device_index = 0;
		
		while (EnumDisplayDevicesA(device_info_struct.DeviceName, sub_device_index, &device_info_struct_for_subchecking, 0) != 0)
		{
			fprintf(
				handle_to_a_opened_file,
				"\t%s\n\t%s\n\n",
				device_info_struct_for_subchecking.DeviceName,
				device_info_struct_for_subchecking.DeviceString
				);

			sub_device_index++;
		}
		
		device_index++;
	}
#else
	
	FILE * command_output =
		popen("lspci | grep VGA", "r");
	
	if(command_output != NULL)
	{
		char buf[256];
		int i;
		for(i = 0; i < 256; i++)
		{
			buf[i] = '\0';
		}
		
		char c = fgetc(command_output);
		for(i = 0; i < 255 && c != EOF; i++)
		{
			buf[i] = c;
			c = fgetc(command_output);
		}
		
		fprintf(
			handle_to_a_opened_file,
			"%s\n",
			buf
			);
		
		pclose(command_output);
	}
	else
	{
		simplest_log(
			"print_display_devices_to_file_handle "
			"cannot popen lspci"
			);
		return MY_FALSE;
	}
	
	//optional hwinfo
	
	if(WEXITSTATUS(system("hwinfo --version")) != 0x7f)
	{
		command_output =
			popen("hwinfo --monitor | grep Model", "r");
			
		if(command_output != NULL)
		{
			int i;
			char line_buf[256], name_buf[256];
			for(i = 0; i < 256; i++)
			{
				line_buf[i] = '\0';
				name_buf[i] = '\0';
			}
			int names_loop = MY_TRUE;
			char c;
			
			c = fgetc(command_output);
			while(names_loop == MY_TRUE)
			{
				for(i = 0; i < 255 && c != EOF && c != '\n'; i++)
				{
					line_buf[i] = c;
					c = fgetc(command_output);
				}
				
				if(c == EOF || c == '\n')
				{
					char * name_worker;
					name_worker = line_buf;
					
					while((*name_worker) != '\"')
					{
						name_worker++;
					}
					name_worker++;
					
					int j = 0;
					while((*name_worker) != '\"')
					{
						name_buf[j] = (*name_worker);
						name_worker++;
						j += 1;
					}
					
					fprintf(
						handle_to_a_opened_file,
						"Monitor: %s\n",
						name_buf
						);
					
					if(c == '\n')
					{
						for(i = 0; i < 256; i++)
						{
							line_buf[i] = '\0';
							name_buf[i] = '\0';
						}
						c = fgetc(command_output);
					}
				}
				
				if(c == EOF)
				{
					names_loop = MY_FALSE;
				}
			}
			
			pclose(command_output);
		}
		else
		{
			simplest_log(
				"print_display_devices_to_file_handle "
				"cannot popen hwinfo, skipping monitors"
				);
		}
	}
	else
	{
		simplest_log(
			"print_display_devices_to_file_handle "
			"no hwinfo command, skipping monitors"
			);
	}

#endif

	return MY_TRUE;
}

/*
		get_os_version_string
*/
int get_os_version_string(
	char * buffer_at_least_256
	)
{
	if(buffer_at_least_256 == NULL)
	{
		simplest_log(
			"get_os_version_string "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	int i;
	for(i = 0; i < 256; i++)
	{
		*(buffer_at_least_256 + i) = '\0';
	}
	
#if defined _WIN32
	int ret_buf_pos = 0;
	
	char working_buf[128];
	for(i = 0; i < 128; i++)
	{
		working_buf[i] = '\0';
	}
	unsigned long int buf_size = 128;
	
	if(
		RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
			"ProductName",
			RRF_RT_REG_SZ,
			NULL,
			working_buf,
			&(buf_size)
			)
		==
		ERROR_SUCCESS
		)
	{
		strcpy(buffer_at_least_256 + ret_buf_pos, working_buf);
		
		ret_buf_pos = strlen(buffer_at_least_256);
		
		*(buffer_at_least_256 + ret_buf_pos) = ' ';
		ret_buf_pos+=1;
	}
	else
	{
		simplest_log(
			"get_os_version_string "
			"cannot retrieve from "
			"RegGetValueA "
			"ProductName"
			);
	}
	
	//reset because in RegGetValueA its in/out
	buf_size = 128;
	
	if(
		RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
			"DisplayVersion",
			RRF_RT_REG_SZ,
			NULL,
			working_buf,
			&(buf_size)
			)
		==
		ERROR_SUCCESS
		)
	{
		strcpy(buffer_at_least_256 + ret_buf_pos, working_buf);
		
		ret_buf_pos += strlen(working_buf);
		
		*(buffer_at_least_256 + ret_buf_pos) = ' ';
		ret_buf_pos += 1;
	}
	else
	{
		simplest_log(
			"get_os_version_string "
			"cannot retrieve from "
			"RegGetValueA "
			"DisplayVersion"
			);
	}
	
	//reset
	buf_size = 128;

	if(
		RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
			"CurrentBuild",
			RRF_RT_REG_SZ,
			NULL,
			(void *) working_buf,
			&(buf_size)
			)
		==
		ERROR_SUCCESS
		)
	{	
		strcpy(buffer_at_least_256 + ret_buf_pos, working_buf);
		
		ret_buf_pos += strlen(working_buf);
		
		int temp, dummy;
		if(
			simplest_decimal_char_to_int(
				&(temp),
				working_buf,
				MY_TRUE,
				&(dummy)
				)
			==
			MY_TRUE
			)
		{
			if(temp >= 22000)
			{
				strcpy(
					buffer_at_least_256 + ret_buf_pos,
					"(win 11)"
					);
				ret_buf_pos += 8;
			}
		}
		else
		{
			simplest_log(
				"get_os_version_string "
				"cannot parse "
				"CurrentBuild"
				);
		}
	}
	else
	{
		simplest_log(
			"get_os_version_string "
			"cannot retrieve from "
			"RegGetValueA "
			"CurrentBuild"
			);
	}
	
#else
	
	FILE * command_output =
		popen("hostnamectl | grep \"Operating System\"", "r");
	
	char c, c_prev = 0;
	int read_name = MY_FALSE;
	i = 0;
	
	if(command_output != NULL)
	{
		c = fgetc(command_output);
		while(c != EOF && i < 255)
		{
			if(read_name == MY_TRUE)
			{
				*(buffer_at_least_256 + i) = c;
				i++;
			}
			
			if(c == ' ' && c_prev == ':')
			{
				read_name = MY_TRUE;
			}
			
			c_prev = c;
			c = fgetc(command_output);
			
			if(c == '\n')
			{
				read_name = MY_FALSE;
			}
		}
		
		if(i < 255)
		{
			*(buffer_at_least_256 + i) = ' ';
			i++;
		}
		
		pclose(command_output);
	}
	else
	{
		simplest_log(
			"get_os_version_string "
			"cannot popen hostnamectl"
			);
	}

	read_name = MY_FALSE;
	//dont resetting i !
	
	command_output =
		popen("hostnamectl | grep Kernel", "r");
	
	if(command_output != NULL)
	{
		c = fgetc(command_output);
		while(c != EOF && i < 255)
		{
			if(read_name == MY_TRUE)
			{
				*(buffer_at_least_256 + i) = c;
				i++;
			}
			
			if(c == ' ' && c_prev == ':')
			{
				read_name = MY_TRUE;
			}
			
			c_prev = c;
			c = fgetc(command_output);
			
			if(c == '\n')
			{
				read_name = MY_FALSE;
			}
		}
		
		if(i < 255)
		{
			*(buffer_at_least_256 + i) = ' ';
			i++;
		}
		
		pclose(command_output);
	}
	else
	{
		simplest_log(
			"get_os_version_string "
			"cannot popen hostnamectl"
			);
	}
	
	read_name = MY_FALSE;
	//dont resetting i !
	
	command_output =
		popen("hostnamectl | grep Architecture", "r");
	
	if(command_output != NULL)
	{
		c = fgetc(command_output);
		while(c != EOF && i < 255)
		{
			if(read_name == MY_TRUE)
			{
				*(buffer_at_least_256 + i) = c;
				i++;
			}
			
			if(c == ' ' && c_prev == ':')
			{
				read_name = MY_TRUE;
			}
			
			c_prev = c;
			c = fgetc(command_output);
			
			if(c == '\n')
			{
				read_name = MY_FALSE;
			}
		}
		
		if(i < 255)
		{
			*(buffer_at_least_256 + i) = ' ';
			i++;
		}
		
		pclose(command_output);
	}
	else
	{
		simplest_log(
			"get_os_version_string "
			"cannot popen hostnamectl"
			);
	}

#endif

	return MY_TRUE;
}

/*
		get_motherboard_string
*/
int get_motherboard_string(
	char * buffer_at_least_256
	)
{
	if(buffer_at_least_256 == NULL)
	{
		simplest_log(
			"get_motherboard_string "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	int i;
	for(i = 0; i < 256; i++)
	{
		*(buffer_at_least_256 + i) = '\0';
	}
	
#if defined _WIN32
	int ret_buf_pos = 0;
	
	char working_buf[128];
	for(i = 0; i < 128; i++)
	{
		working_buf[i] = '\0';
	}
	unsigned long int buf_size = 128;
	
	if(
		RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\BIOS",
			"SystemProductName",
			RRF_RT_REG_SZ,
			NULL,
			working_buf,
			&(buf_size)
			)
		==
		ERROR_SUCCESS
		)
	{
		strcpy(buffer_at_least_256 + ret_buf_pos, working_buf);
		
		ret_buf_pos = strlen(buffer_at_least_256);
		
		*(buffer_at_least_256 + ret_buf_pos) = ' ';
		ret_buf_pos+=1;
	}
	else
	{
		if(
			RegGetValueA(
				HKEY_LOCAL_MACHINE,
				"HARDWARE\\DESCRIPTION\\System\\BIOS",
				"BaseBoardProduct",
				RRF_RT_REG_SZ,
				NULL,
				working_buf,
				&(buf_size)
				)
			==
			ERROR_SUCCESS
			)
		{
			strcpy(buffer_at_least_256 + ret_buf_pos, working_buf);
			
			ret_buf_pos = strlen(buffer_at_least_256);
			
			*(buffer_at_least_256 + ret_buf_pos) = ' ';
			ret_buf_pos+=1;
		}
		else
		{
			simplest_log(
				"get_motherboard_string "
				"cannot retrieve from "
				"RegGetValueA "
				"SystemProductName or BaseBoardProduct "
				);
		}
	}
	
	//reset because in RegGetValueA its in/out
	buf_size = 128;
	
	if(
		RegGetValueA(
			HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\BIOS",
			"SystemManufacturer",
			RRF_RT_REG_SZ,
			NULL,
			working_buf,
			&(buf_size)
			)
		==
		ERROR_SUCCESS
		)
	{
		strcpy(buffer_at_least_256 + ret_buf_pos, working_buf);
		
		ret_buf_pos += strlen(working_buf);
		
		*(buffer_at_least_256 + ret_buf_pos) = ' ';
		ret_buf_pos += 1;
	}
	else
	{
		if(
			RegGetValueA(
				HKEY_LOCAL_MACHINE,
				"HARDWARE\\DESCRIPTION\\System\\BIOS",
				"BaseBoardManufacturer",
				RRF_RT_REG_SZ,
				NULL,
				working_buf,
				&(buf_size)
				)
			==
			ERROR_SUCCESS
			)
		{
			strcpy(buffer_at_least_256 + ret_buf_pos, working_buf);
			
			ret_buf_pos += strlen(working_buf);
			
			*(buffer_at_least_256 + ret_buf_pos) = ' ';
			ret_buf_pos += 1;
		}
		else
		{
			simplest_log(
				"get_motherboard_string "
				"cannot retrieve from "
				"RegGetValueA "
				"SystemManufacturer or BaseBoardManufacturer"
				);
		}
	}
	
#else
	
	FILE * command_output =
		popen("hostnamectl | grep \"Hardware Vendor\"", "r");
	
	char c, c_prev = 0;
	int read_name = MY_FALSE;
	i = 0;
	
	if(command_output != NULL)
	{
		c = fgetc(command_output);
		while(c != EOF && i < 255)
		{
			if(read_name == MY_TRUE)
			{
				*(buffer_at_least_256 + i) = c;
				i++;
			}
			
			if(c == ' ' && c_prev == ':')
			{
				read_name = MY_TRUE;
			}
			
			c_prev = c;
			c = fgetc(command_output);
			
			if(c == '\n')
			{
				read_name = MY_FALSE;
			}
		}
		
		if(i < 255)
		{
			*(buffer_at_least_256 + i) = ' ';
			i++;
		}
		
		pclose(command_output);
	}
	else
	{
		simplest_log(
			"get_motherboard_string "
			"cannot popen hostnamectl"
			);
	}

	read_name = MY_FALSE;
	//dont resetting i !
	
	command_output =
		popen("hostnamectl | grep \"Hardware Model\"", "r");
	
	if(command_output != NULL)
	{
		c = fgetc(command_output);
		while(c != EOF && i < 255)
		{
			if(read_name == MY_TRUE)
			{
				*(buffer_at_least_256 + i) = c;
				i++;
			}
			
			if(c == ' ' && c_prev == ':')
			{
				read_name = MY_TRUE;
			}
			
			c_prev = c;
			c = fgetc(command_output);
			
			if(c == '\n')
			{
				read_name = MY_FALSE;
			}
		}
		
		pclose(command_output);
	}
	else
	{
		simplest_log(
			"get_motherboard_string "
			"cannot popen hostnamectl"
			);
	}
	
#endif

	return MY_TRUE;
}

/*
		get_endianness
*/
int get_endianness(void)
{
	union uint32_4_uint8_end_det
	{
		unsigned int uint32;
		unsigned char uint8[4];
	} test_val;
	
	test_val.uint32 = 0x12345678;
	
	if(
		test_val.uint8[0] == 0x78
		&&
		test_val.uint8[1] == 0x56
		&&
		test_val.uint8[2] == 0x34
		&&
		test_val.uint8[3] == 0x12
		)
	{
		return ENDIANNES_LITTLE;
	}
	else if(
		test_val.uint8[0] == 0x12
		&&
		test_val.uint8[1] == 0x34
		&&
		test_val.uint8[2] == 0x56
		&&
		test_val.uint8[3] == 0x78
		)
	{
		return ENDIANNES_BIG;
	}
	else
	{
		return ENDIANNES_UNKNOWN;
	}
}

#if !(defined _WIN32)
/*
		tty_vs_x11_vs_wayland
			
			0 - tty
			1 - x11
			2 - wayland
*/
int tty_vs_x11_vs_wayland(void)
{
	/*
		1. try with command
	*/
	FILE * command_trial =
		popen(
			"loginctl show-session $(loginctl | grep $(whoami) | awk '{print $1}') -p Type",
			"r"
			);
	
	if(command_trial != NULL)
	{
		char buf[256];
		int i;
		for(i = 0; i < 256; i++)
		{
			buf[i] = 0;
		}
		
		i = 0;
		char c = fgetc(command_trial);
		while(c != EOF && i < 255)
		{
			buf[i] = c;
			i++;
			c = fgetc(command_trial);
		}
		
		if(i >= 255)
		{
			simplest_log(
				"tty_vs_x11_vs_wayland "
				"output from command shouldnt be that long"
				);
		}
		else
		{
			int result = -1;
			
			if(
				strstr(
					buf,
					"wayland"//TBD check
					)
				!=
				NULL
				)
			{
				result = 2;
			}
			else if(
				strstr(
					buf,
					"x11"
					)
				!=
				NULL
				)
			{
				result = 1;
			}
			else if(
				strstr(
					buf,
					"tty"//TBD check
					)
				!=
				NULL
				)
			{
				result = 0;
			}
			
			if(result != -1)
			{
				pclose(command_trial);
				return result;
			}
		}
		
		pclose(command_trial);
	}
	
	simplest_log(
		"tty_vs_x11_vs_wayland "
		"not found with command, "
		"trying environment variables"
		);
	
	/*
		2. try with environment variables
	*/
	
	//	trials showed that XDG_SESSION_TYPE must be first,
	//	then WAYLAND_DISPLAY then DISPLAY
	//	because DISPLAY showed on wayland
	
	char * xdg_session_type = getenv("XDG_SESSION_TYPE");
	
	if(strstr(xdg_session_type, "wayland") != NULL)
	{
		return 2;
	}
	else if(strstr(xdg_session_type, "x11") != NULL)
	{
		return 1;
	}
	else if(strstr(xdg_session_type, "tty") != NULL)
	{
		return 0;
	}
	
	char * wayland_display = getenv("WAYLAND_DISPLAY");
	char * display = getenv("DISPLAY");
	
	if(wayland_display != NULL)
	{
		// TBD check
		return 2;
	}
	else if(display != NULL)
	{
		// TBD check
		return 1;
	}
	else
	{
		//trying xdg
	}
	
	
	
	simplest_log(
		"tty_vs_x11_vs_wayland "
		"not found with environment variables, "
		"trying tty command"
		);
	
	/*
		3. tty command
			
			last stand
	*/
	
	command_trial =
		popen("tty", "r");
	
	if(command_trial != NULL)
	{
		char buf[6] = {0,0,0,0,0,0};
		
		int i;
		char c = fgetc(command_trial);
		for(i = 0; i < 5 && c != EOF; i++)
		{
			buf[i] = c;
			c = fgetc(command_trial);
		}
		
		pclose(command_trial);
		
		//this should be enough
		if(strcmp(buf,"/dev/") == 0)
		{
			return 0;
		}
		else
		{
			simplest_log(
				"tty_vs_x11_vs_wayland "
				"no wayland, no x11, no tty, "
				"returning -1"
				);
			
		}
	}
	else
	{
		simplest_log(
			"tty_vs_x11_vs_wayland "
			"cant popen tty command, "
			"returning -1"
			);
	}
	
	return -1;
}

#endif
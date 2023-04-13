/*
 * Driver program for image plugins
 * CSF Assignment 4
 * Joy Neuberger, Shaina Gabala
 * jneuber6@jhu.edu, sgabala2@jhu.edu
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/types.h>

#include "image.h"
#include "pnglite.h"
#include "image_plugin.h"

char missing_args[] = "invalid (number of) command line arguments.";
char invalid_args[] = "invalid command line arguments.";
char plugin_error[] = "image processing plugin failed to load";
char api_error[] = "a required API function can’t be found within a loaded plugin";
char memory_error[] = "memory allocation error occurred";

//Definition of Plugin struct
struct Plugin {
    void *handle;
    const char *(*get_plugin_name)(void);
    const char *(*get_plugin_desc)(void);
    void *(*parse_arguments)(int num_args, char *args[]);
    struct Image *(*transform_image)(struct Image *source, void *arg_data);
};

//Prints usage message,
//Called if have no/invalid command line args
void print_usage() { //char *argv[]) {
	printf("Usage: imgproc <command> [<command args...>]\nCommands are:\n    list\n    exec <plugin> <input img> <output img> [<plugin args...>]\n");
}

//Prints passed error message, calls print_usage if necessary, and exits
//Parameters: mssg: specific error message
void error_handling(char mssg[]) {
	printf("Error: %s\n", mssg);
	if (strcmp(mssg, missing_args) == 0 || strcmp(mssg, invalid_args)) {
		print_usage();
	}
	exit(1);
}

//Prints list of loaded Plugins and their names and descriptions, 
//Called if have list command line argument
//Parameters:
//      plg: pointer to first loaded plugin
//      size: number of Plugins within struct
void drive_list(struct Plugin *plg, int size) { 
	printf("Loaded %d plugin(s):\n", size);
	for(int i = 0; i < size; i++) {
		printf("    %s: %s\n", plg->get_plugin_name(), plg->get_plugin_desc());
		plg++;
	}

	return;
} 

//Uses named plugin to change source image into specified destination image
//Called if have exec command line argument
//Parameters:
//      argc: Number of remaining command line args
//      argv: pointer to first remaining command line argument
//      plg: pointer to Plugin to be used for transformation
void drive_exec(int argc, char *argv[], struct Plugin *plg) {
	//the exec command should:
	//1. find a plugin whose name matches the specified plugin name,
	
	while(plg != NULL) {
		if (strcmp(plg->get_plugin_name(), argv[2]) == 0) {
			break;
		}
		plg++;
	}

	if(plg == NULL){ //exec command not found
		error_handling(invalid_args);
	}

	//2. load the specified input image (using img_read_png)
	struct Image *input = img_read_png(argv[3]);
	
	//3. pass any command line arguments (past the input and output filenames) to the plugin’s parse_arguments function to produce an argument object, 
	char **rest = (argv + 5);

	void *data = plg->parse_arguments(argc, rest);
	if(data == NULL) {
		error_handling(missing_args);
	} else {
		//4. call the plugin’s transform_image function to perform the image transformation (passing the argument object returned by parse_arguments), and then save the resulting image to the named output file (using img_write_png). Note that it is the plugin’s responsibility to free the argument object.
		struct Image *output = plg->transform_image(input, data);
		img_write_png(output, argv[4]);
	}
}

int main(int argc, char *argv[]) {
	char *plugin_dir = getenv("PLUGIN_DIR");
	if (!plugin_dir) {
  // use default plugin directory
  	plugin_dir = "./plugins";
	}
	
	DIR *dir = opendir(plugin_dir);
	struct dirent *next = readdir(dir);

	struct Plugin plugins[100];
	struct Plugin *ptr = plugins;
	int found = 0;

	while(next != NULL) {
		if(next->d_name[0] == '.') { //ignore hidden files
			next = readdir(dir);
			continue;
		}
		
		char path[256]; //correct path to file, to use dlopen
		strcpy(path, plugin_dir);
		strcat(strcat(path, "/"), next->d_name);
		
		ptr->handle = dlopen(path, RTLD_LAZY);
		if (!ptr->handle) {
    	/* fail to load the library */
    	error_handling(plugin_error);
  	}

		*(void **) (&ptr->get_plugin_name) = dlsym(ptr->handle, "get_plugin_name");
		*(void **) (&ptr->get_plugin_desc) = dlsym(ptr->handle, "get_plugin_desc");
		*(void **) (&ptr->parse_arguments) = dlsym(ptr->handle, "parse_arguments");
		*(void **) (&ptr->transform_image) = dlsym(ptr->handle, "transform_image");
		
		next = readdir(dir);
		ptr++;
		found++;
	}
	

	if(argc == 1) {
		//If invoked without command line arguments, or with incorrect command line arguments, imgproc should print a usage message.
		print_usage(); //argv);
		return 0;
	} 

	if(strcmp(argv[1], "list") == 0) {
		ptr = plugins;
		drive_list(ptr, found);
	} else if(strcmp(argv[1], "exec")==0) {
			if(argc < 5) {
				error_handling(missing_args);
			} else {
				ptr = plugins;
				drive_exec(argc - 5, argv, ptr);
			}
	} else {
		error_handling(invalid_args);
	}

	//close files in Plugin array
	closedir(dir);
	return 0;
}


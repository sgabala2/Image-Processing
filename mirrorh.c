/*
 * Implementation of mirrorh plugin
 * CSF Assignment 4
 * Joy Neuberger, Shaina Gabala
 * jneuber6@jhu.edu, sgabala2@jhu.edu
 */
#include "image_plugin.h"
#include <stdlib.h>
#include <stdio.h>

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	// just define a single dummy field.
	int dummy;
}; 

//Returns mirrorh as name of plugin
const char *get_plugin_name(void) {
	return "mirrorh";
}

//Returns description of mirrorc plugin
const char *get_plugin_desc(void) {
	return "mirror image horizontally";
}

//default implementation of parse_arguments since mirrorh has no expected command line arguments
//Returns: NULL if there are invalid extra commands, pointer to Arguments struct sized memory otherwise
void *parse_arguments(int num_args, char *args[]){
(void) args; // this is just to avoid a warning about an unused parameter
	if (num_args != 0) {
		return NULL;
	}
	return calloc(1, sizeof(struct Arguments));
}

//Mirrors a passed image horizontally
//Parameters: 
//    source: image to be mirrored
//    arg_data: pointer to empty memory b/c mirrorh has no command line args
//Returns: pointer to mirrored image, NULL if mirroring is unsuccessful
struct Image *transform_image(struct Image *source, void *arg_data) {
	struct Arguments *args = arg_data;
	struct Image *mirrored = img_create(source->width, source->height);
	if (!mirrored) {
		free(args);
		return NULL;
	}
	
	int pix = 0;
	for(unsigned int h = 1; h <= source->height; h++) {
		for(unsigned int i = 1; i <= source->width; i++) {
			mirrored->data[pix] = source->data[(h*source->width) - i];
			pix++;
		}
	}

	free(args);
	return mirrored;
}
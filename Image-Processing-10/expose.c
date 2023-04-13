/*
 * Implementation of expose plugin
 * CSF Assignment 4
 * Joy Neuberger, Shaina Gabala
 * jneuber6@jhu.edu, sgabala2@jhu.edu
 */
#include "image_plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	// just define a single dummy field.
	float factor;
}; 

//Returns expose as plugin's name
const char *get_plugin_name(void) {
	return "expose";
}

//Returns description of expose plugin
const char *get_plugin_desc(void) {
	return "adjust the intensity of all pixels";
}

//Reads expose factor command line argument
//Parameters: 
//        num_args: number of command line arguments left
//        args: pointer to the command line argument
//Returns: NULL if command line args are invalid, pointer to initialized Arguments struct otherwise
void *parse_arguments(int num_args, char *args[]){
(void) args; 
	if ((num_args != 1) || (!isdigit(**args))) {
		return NULL;
	}

	struct Arguments *current = calloc(1, sizeof(struct Arguments));
	current->factor = atof(*args);
	return current;
}

// Helper function to swap the blue and green color component values.
// Parameters:
//        pix: Pixel that will be unpacked and have its components swapped
//        f: Expose factor
// Returns: New pixel value with swapped components
static uint32_t expose_f(uint32_t pix, float f) {
	uint8_t r, g, b, a;
	img_unpack_pixel(pix, &r, &g, &b, &a);
	
	if(r*f > 255) {
		r = 255;
	} else { r *= f;}
	
	if(g*f > 255) {
		g = 255;
	} else { g *= f;}
	
	if(b*f > 255) {
		b = 255;
	} else { b *= f;}

	return img_pack_pixel(r, g, b, a);
}

// Changes all RGB values by a specified factor
//Parameters: 
//    source: image to be exposed
//    arg_data: pointer containing expose Factor, used to initialize Arguments struct
//Returns: pointer to exposed image, NULL if exposing image is unsuccessful
struct Image *transform_image(struct Image *source, void *arg_data) {
	struct Arguments *args = arg_data;

	// Allocate a result Image
	struct Image *exposed = img_create(source->width, source->height);
	if (!exposed) {
		free(args);
		return NULL;
	}

	unsigned num_pixels = source->width * source->height;
	for (unsigned i = 0; i < num_pixels; i++) {
		exposed->data[i] = expose_f(source->data[i], args->factor);
	}

	free(args);
	return exposed;
}
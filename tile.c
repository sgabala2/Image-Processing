/*
 * Implementation of tile plugin
 * CSF Assignment 4
 * Joy Neuberger, Shaina Gabala
 * jneuber6@jhu.edu, sgabala2@jhu.edu
 */
#include "image_plugin.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct Arguments {
	int tileFactor; //command line argument, tiling factor of image
}; 



//Returns tile as name of plugin
const char *get_plugin_name(void) {
	return "tile";
}

//Returns description of tile plugin
const char *get_plugin_desc(void) {
	return "tile source image in an NxN arrangement";
}

//Reads tiling factor command line argument
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
	current->tileFactor = atof(*args);
	return current;
}

//Turns a passed image into a tiling factor by tiling factor version of itself
//Parameters: 
//    source: image to be tiled
//    arg_data: pointer containing tiling Factor, used to initialize Arguments struct
//Returns: pointer to tiled image, NULL if tiling is unsuccessful
struct Image *transform_image(struct Image *source, void *arg_data) {
	
  struct Arguments *args = arg_data;
	struct Image *tiled = img_create(source->width, source->height);
	if (!tiled) {
		free(args);
		return NULL;
	}

	int tileFactor = args->tileFactor;
  int normWidth = source->width/tileFactor;
  int widthExcess = source->width % tileFactor;
	int heightExcess = source->height % tileFactor;
	int rowNum = 0;
	int index = 0;
	int offset = 0;

	while (rowNum < tileFactor) {
		int skip = 0;
		for(int i = 0; i < (source->height) - heightExcess; i++){
			if((i != 0) && (i % tileFactor == 0)) { skip += tileFactor; }
			int offset = source->width*skip; //*i;
			for(int w = 0; w < normWidth; w++) {
          tiled->data[index] = source->data[(w*tileFactor) + offset];
				  index++;
      }
      //adds extra horizontal pixel to tiles when called for
      if (((i % tileFactor)-widthExcess) < 0) {
        tiled->data[index] = source->data[(normWidth*tileFactor) + offset];
        index++;
      }

	  }
    
    
		rowNum++;
	}
 
	free(args);
	return tiled;
}


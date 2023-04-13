/*
 * Definition of Image data type, support for reading/writing PNG files
 * CSF Assignment 4
 * Joy Neuberger, Shaina Gabala
 * jneuber6@jhu.edu, sgabala2@jhu.edu
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Image {
	// Image width and height
	unsigned width, height;

	// pixel data in row-major order; pixel representation is:
	//   Bits 0-7 are the red color component value
	//   Bits 8-15 are the green color component value
	//   Bits 16-23 are the blue color component value
	//   Bits 24-31 are the alpha channel value (0=completely transparent, 255=completely opaque)
	uint32_t *data;
};


struct Image *img_create(unsigned width, unsigned height);


struct Image *img_duplicate(struct Image *img);


void img_destroy(struct Image *img);


struct Image *img_read_png(const char *filename);
int img_write_png(struct Image *img, const char *filename);
void img_unpack_pixel(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);
uint32_t img_pack_pixel(unsigned r, unsigned g, unsigned b, unsigned a);

#ifdef __cplusplus
}
#endif

#endif 

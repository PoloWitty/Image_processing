#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct centroid
{
    uint32_t red;
    uint32_t green;
    uint32_t blue;
};

typedef struct centroid Centroid;

void centroid_add(Centroid *c1, Centroid *c2);
void centroid_divide(Centroid *centroid, uint32_t num);
void centroid_times(Centroid *centroid, uint32_t num);

void set_centroid(Centroid *centroid, uint32_t value);

#define LOADBMP_IMPLEMENTATION
#include "loadbmp.h"

typedef uint32_t bmp_byte;
typedef bmp_byte *bmp_bytep;

struct image
{
    uint32_t width;
    uint32_t height;
    bmp_bytep *image;
};

typedef struct image Image;

bmp_bytep get_rgb(Image *img, uint32_t x, uint32_t y);
void set_rgb(Image *img, uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue);
void set_rgba(Image *img, uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void set_label(Image *img, uint32_t x, uint32_t y, uint8_t label);

uint8_t gamma_correct(uint32_t num);
uint8_t gamma_reset(uint32_t num);

void print_info(uint32_t width, uint32_t height, uint16_t bits_per_px);

int8_t not_in_bounds(Image *img, uint32_t x, uint32_t y);

Image *open_image(char *filename);
void save_image(Image *img, char *filename);

void *free_image(Image *img);

#define MAP_SIZE 64

typedef uint32_t ***Map;

Map create_map(void);
uint8_t get_index(uint8_t num);

uint8_t clip(int8_t num);

uint32_t combine(uint8_t red, uint8_t green, uint8_t blue, uint8_t climb);

void segment(Image *img, uint8_t bandwidth, uint32_t max_gens);

struct pixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    struct pixel *next;
};

typedef struct pixel Pixel;

Pixel *add_pixel(Pixel *last, uint8_t red, uint8_t green, uint8_t blue);
void free_pixels(Pixel *root);

void error(char *msg);
void check_null(void *ptr, char *msg);

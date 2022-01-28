#include "header.h"

void centroid_add(Centroid *c1, Centroid *c2)
{
	c1->red += c2->red;
	c1->green += c2->green;
	c1->blue += c2->blue;
}

void centroid_divide(Centroid *c1, uint32_t num)
{
	if (num == 0)
		return;
	c1->red /= num;
	c1->green /= num;
	c1->blue /= num;
}

void centroid_times(Centroid *c1, uint32_t num)
{
	c1->red *= num;
	c1->green *= num;
	c1->blue *= num;
}

void set_centroid(Centroid *c1, uint32_t value)
{
	c1->red = value >> 24;
	c1->green = (value >> 16) & 63;
	c1->blue = (value >> 8) & 63;
}

// Get a pointer to RGB array of (x, y) pixel
bmp_bytep get_rgb(Image *img, uint32_t x, uint32_t y)
{
	if (not_in_bounds(img, x, y))
		return NULL;
	return &(img->image[y][x]);
}

// Set the RGB values of an (x, y) pixel
void set_rgb(Image *img, uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	if (not_in_bounds(img, x, y))
		return;

	uint8_t *pixel = &(img->image[y][x]);
	pixel[0] = red;
	pixel[1] = green;
	pixel[2] = blue;
}

// Set the RGBA values of an (x, y) pixel
void set_rgba(Image *img, uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	if (not_in_bounds(img, x, y))
		return;

	uint8_t *pixel = &(img->image[y][x]);
	pixel[0] = red;
	pixel[1] = green;
	pixel[2] = blue;
	pixel[3] = alpha;
}

// Set the Alpha value of an (x, y) pixel, acts as label for kmeans
void set_label(Image *img, uint32_t x, uint32_t y, uint8_t label)
{
	if (not_in_bounds(img, x, y))
		return;
	uint8_t *pixel = &(img->image[y][x]);
	pixel[3] = label;
}

// Print image info
void print_info(uint32_t width, uint32_t height, uint16_t bits_per_px)
{
	printf("\twidth:  %d\n", width);
	printf("\theight: %d\n", height);
	printf("\tbits_per_px:  %d\n", bits_per_px);
}

// Check if a point (x, y) is in the image
int8_t not_in_bounds(Image *img, uint32_t x, uint32_t y)
{
	return (x < 0 || y < 0 || x > img->width || y > img->height);
}

// Gamma correction and normalization
uint8_t gamma_correct(uint32_t num)
{
	return (num * num) >> 8;
}

uint8_t gamma_reset(uint32_t num)
{
	return sqrt(num << 8);
}

// Open and read an image
Image *open_image(char *filename)
{
	int num_row_bytes, i = 0, j = 0;

	// Allocate space
	Image *img = (Image *)malloc(sizeof(Image));
	// check_null(img, "Unable to malloc Image in open_png");

	printf("Opening %s...\n", filename);
	// check_null(fp, "Unable to open file");

	unsigned char *pixels = NULL;

	unsigned int err = loadbmp_decode_file(filename, &pixels, &(img->width), &(img->height), LOADBMP_RGBA);

	if (err)
		printf("LoadBMP Load Error: %u\n", err);

	// uint16_t filetype;
	// uint32_t data_offset;
	// uint16_t bits_per_px;

	// // uint32_t offset_file_type = 0;
	// uint32_t offset_data_offset = 10;
	// uint32_t offset_width = 14 + 4;
	// // uint32_t offset_height = 14 + 8;
	// uint32_t offset_bits_per_px = 14 + 14;
	// // uint32_t offset_is_compressed = 14 + 16;

	// fread(&filetype, sizeof(uint16_t), 1, fp);
	// fseek(fp, offset_data_offset, SEEK_SET);
	// fread(&data_offset, sizeof(uint32_t), 1, fp);
	// fseek(fp, offset_width, SEEK_SET);
	// fread(&(img->width), sizeof(int32_t), 1, fp);
	// fread(&(img->height), sizeof(int32_t), 1, fp);
	// fseek(fp, offset_bits_per_px, SEEK_SET);
	// fread(&bits_per_px, sizeof(uint16_t), 1, fp);

	// print_info(img->width, img->height,bits_per_px);

	// Allocate the image space
	img->image = (bmp_bytep *)malloc(sizeof(bmp_bytep) * img->height);
	// check_null(img->image, "Unable to malloc bmp_byte for Image in open_png");

	for (i = 0; i < img->height; i++)
	{
		img->image[i] = (bmp_byte *)malloc(sizeof(bmp_byte) * (img->width));
		// check_null(img, "Unable to malloc bmp_byte for Image in open_png");
	}

	// bmp_bytep color_palette=(bmp_byte*)malloc(256*sizeof(bmp_byte));
	// uint32_t offset_palette_data = 14 + 40;
	// uint8_t r, g, b, reserved;
	// uint8_t* pixel;
	// fseek(fp, offset_palette_data, SEEK_SET);
	// for (i = 0; i < 256; i++)
	// {
	// 	fread(&b, 1, 1, fp);
	// 	fread(&g, 1, 1, fp);
	// 	fread(&r, 1, 1, fp);
	// 	fread(&reserved, 1, 1, fp);
	// 	// r = r >> 2;
	// 	// g = g >> 2;
	// 	// b = b >> 2;

	// 	pixel = &(color_palette[i]);
	// 	// color_palette[i] = (r << 24) | (g << 16) | (b << 8) | 0;
	// 	pixel[0]=r;
	// 	pixel[1]=g;
	// 	pixel[2]=b;
	// 	pixel[3]=0;
	// }

	// fseek(fp, data_offset, SEEK_SET);

	// uint8_t color;
	// for (i = 0; i < img->height; i++)
	// {
	// 	fseek(fp, (img->width + 3) / 4 * i * 4 + data_offset, SEEK_SET);
	// 	for (j = 0; j < img->width; j++)
	// 	{
	// 		fread(&color, 1, 1, fp);
	// 		// put_pixel(j, bmp.height - i - 1, color);
	// 		img->image[img->height - i - 1][j] = color_palette[color];
	// 	}
	// }
	uint32_t x, y;
	uint8_t *pixel;
	for (y = 0; y < img->height; y++)
	{
		for (x = 0; x < img->width; x++)
		{
			pixel = &(pixels[(y * (img->width) + x) * LOADBMP_RGBA]);
			set_rgba(img, x, y, pixel[0], pixel[1], pixel[2], 0);
		}
	}
	free(pixels);

	return img;
}

// Free image memory allocations
void *free_image(Image *img)
{
	int i;

	if (img == NULL)
		return NULL;
	if (img->image == NULL)
		return NULL;

	for (i = 0; i < img->height; i++)
		free(img->image[i]);

	free(img->image);
	free(img);

	return NULL;
}

void save_image(Image *img, char *filename)
{
	printf("Saving %s\n", filename);

	uint32_t width = img->width, height = img->height;
	uint8_t *pixels = (uint8_t *)malloc(sizeof(uint8_t) * LOADBMP_RGBA * width * height);
	// uint8_t *pixels =img->image;
	if (pixels == NULL)
	{
		puts("can't malloc pixels when saving");
	}
	uint32_t i = 0, x, y;
	uint8_t *pixel;

	for (y = 0; y < img->height; y++)
	{
		for (x = 0; x < img->width; x++)
		{
			pixel = &(img->image[y][x]);
			pixels[i] = pixel[0];
			pixels[i + 1] = pixel[1];
			pixels[i + 2] = pixel[2];
			pixels[i + 3] = pixel[3];
			i += LOADBMP_RGBA;
		}
	}
	unsigned int err = loadbmp_encode_file(filename, pixels, width, height, LOADBMP_RGBA);

	if (err)
		printf("LoadBMP Load Error: %u\n", err);
	free(pixels);
}

// int main(){
// 	Image* img=open_image("test.bmp");
// 	save_image(img,"rst.bmp");
// 	free_image(img);
// 	// uint8_t *pixel;
// 	// int i,j;
// 	// bmp_bytep tmp=get_rgb(img,100,75);
// 	// unsigned char *pixels = NULL;
// 	// unsigned int width, height;

// 	// unsigned int err = loadbmp_decode_file("test.bmp", &pixels, &width, &height, LOADBMP_RGBA);

// 	// if (err)
// 	// 	printf("LoadBMP Load Error: %u\n", err);
// 	// for( i=0;i<10;i++)
// 	// {
// 	// 	for(j=0;j<10;j++)
// 	// 	{
// 	// 		pixel=&(img->image[i][j]);
// 	// 		// pixel=&(pixels[(j*img->width+i)*LOADBMP_RGBA]);

// 	// 		printf("%2d %2d %2d %2d",pixel[0],pixel[1],pixel[2],pixel[3]);
// 	// 		printf("   ");
// 	// 		// printf("%d",;
// 	// 	}
// 	// 	printf("\n");
// 	// }

// 	// Remember to free the pixel data when it isn't needed anymore.
// 	// free(pixels);

// 	return 0;
// }

Map create_map(void)
/*
malloc一个矩阵
*/
{
	uint8_t i, j;

	uint32_t ***temp = (uint32_t ***)calloc(MAP_SIZE, sizeof(uint32_t **));
	// check_null(temp, "creating map failed");
	for (i = 0; i < MAP_SIZE; i++)
	{
		temp[i] = (uint32_t **)calloc(MAP_SIZE, sizeof(uint32_t *));
		// check_null(temp[i], "creating map failed");
		for (j = 0; j < MAP_SIZE; j++)
		{
			temp[i][j] = (uint32_t *)calloc(MAP_SIZE, sizeof(uint32_t));
			// check_null(temp[i][j], "creating map failed");
		}
	}

	return temp;
}

uint8_t get_index(uint8_t num)
{
	return gamma_correct(num) >> 2;
}

uint8_t clip(int8_t num)
{
	if (num < 0)
		return 0;
	if (num > 63)
		return 63;
	return num;
}

uint32_t combine(uint8_t red, uint8_t green, uint8_t blue, uint8_t climb)
{
	return (red << 24) | (green << 16) | (blue << 8) | climb;
}

void segment(Image *img, uint8_t bandwidth, uint32_t max_gens)
{
	Map color_map;
	Map counts;

	uint32_t x, y, gens = 0, count = 0;
	uint8_t red, green, blue, ru, rl, gu, gl, bu, bl;
	uint8_t cont = 1;

	uint8_t *rgb = NULL;

	Pixel *first_pixel = NULL;
	Pixel *current_pixel = NULL;

	Centroid current_centroid = {0, 0, 0};
	Centroid average_centroid = {0, 0, 0};

	color_map = create_map();
	counts = create_map();

	printf("Initializing pixel maps\n");
	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			red = get_index(rgb[0]);
			green = get_index(rgb[1]);
			blue = get_index(rgb[2]);

			if (counts[red][green][blue] == 0)
			{
				color_map[red][green][blue] = combine(red, green, blue, 0);
				current_pixel = add_pixel(current_pixel, red, green, blue);
				if (first_pixel == NULL)
					first_pixel = current_pixel;
			}
			counts[red][green][blue]++;
		}
	}//扫一遍, 构造图片像素链表和映射color_map

	while (cont == 1 && gens < max_gens)
	{
		printf("gen %d\n", gens);

		cont = 0;
		current_pixel = first_pixel;

		while (current_pixel != NULL)
		{
			if ((color_map[current_pixel->red][current_pixel->green][current_pixel->blue] & 1) == 0)
			{
				set_centroid(&average_centroid, 0);
				set_centroid(&current_centroid, color_map[current_pixel->red][current_pixel->green][current_pixel->blue]);
				count = 0;

				rl = clip(current_centroid.red - bandwidth);
				ru = clip(current_centroid.red + bandwidth);

				gl = clip(current_centroid.green - bandwidth);
				gu = clip(current_centroid.green + bandwidth);

				bl = clip(current_centroid.blue - bandwidth);
				bu = clip(current_centroid.blue + bandwidth);

				for (red = rl; red <= ru; red++)
				{
					for (green = gl; green <= gu; green++)
					{
						for (blue = bl; blue <= bu; blue++)
						{
							set_centroid(&current_centroid, color_map[red][green][blue]);
							centroid_times(&current_centroid, counts[red][green][blue]);
							centroid_add(&average_centroid, &current_centroid);
							count += counts[red][green][blue];
						}
					}
				}

				set_centroid(&current_centroid, color_map[current_pixel->red][current_pixel->green][current_pixel->blue]);
				centroid_divide(&average_centroid, count);

				if (average_centroid.red != current_centroid.red || average_centroid.green != current_centroid.green || average_centroid.blue != current_centroid.blue)
				{
					cont = 1;
					color_map[current_pixel->red][current_pixel->green][current_pixel->blue] = combine(average_centroid.red, average_centroid.green, average_centroid.blue, 0);
				}
				else
				{
					color_map[current_pixel->red][current_pixel->green][current_pixel->blue] |= 1;
				}
			}

			current_pixel = current_pixel->next;
		}

		gens++;
	}

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			red = get_index(rgb[0]);
			green = get_index(rgb[1]);
			blue = get_index(rgb[2]);

			count = color_map[red][green][blue];

			red = count >> 24;
			green = (count >> 16) & 63;
			blue = (count >> 8) & 63;

			red <<= 2;
			green <<= 2;
			blue <<= 2;

			red = gamma_reset(red);
			green = gamma_reset(green);
			blue = gamma_reset(blue);

			set_rgb(img, x, y, red, green, blue);
		}
	}

	free_pixels(first_pixel);
}

Pixel *add_pixel(Pixel *last, uint8_t red, uint8_t green, uint8_t blue)
{
	Pixel *ptr = (Pixel *)malloc(sizeof(Pixel));
	// check_null(ptr, "malloc failed to find space for a Pixel");

	ptr->red = red;
	ptr->green = green;
	ptr->blue = blue;
	ptr->next = NULL;

	if (last != NULL)
		last->next = ptr;
	return ptr;
}

void free_pixels(Pixel *root)
{
	if (root != NULL)
	{
		free_pixels(root->next);
		free(root);
	}
}

void error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(0);
}

void check_null(void *ptr, char *msg)
{
	if (ptr == NULL)
		error(msg);
}

int main(int argc, char **argv)
{
	Image *img = NULL;
	char *source, *save;
	uint8_t bandwidth;
	uint32_t gens = 50;

	if (argc < 4) error("Too few command line arguments. Need [source filename] [output filename] [bandwidth]. Optional [Max gens]");

	img = open_image(argv[1]);
	bandwidth = atoi(argv[3]);

	if (argc == 5) gens = atoi(argv[4]);

	segment(img, bandwidth, gens);
	save_image(img, argv[2]);

	img = free_image(img);

	return 0;
}
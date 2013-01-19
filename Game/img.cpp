#include "img.h"

png_byte* readPNG(const char* file, unsigned int& width, unsigned int& height)
{
	png_byte header[8];
	FILE *fp = fopen(file, "rb");
	if (!fp)
	{
		fprintf(stderr, "error: Couldn't open %s\n", file);
		return 0;
	}
	// read the header
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8)) //Test if not png
	{
		fprintf(stderr, "error: %s is not a PNG.\n", file);
		fclose(fp);
		return 0;
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fprintf(stderr, "error: png_create_read_struct returned 0.\n");
		fclose(fp);
		return 0;
	}
	// create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}
	// create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return 0;
	}

	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "error from libpng\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	// init png reading
	png_init_io(png_ptr, fp);

	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr,info_ptr);
	height = png_get_image_height(png_ptr,info_ptr);

	png_set_packing(png_ptr); //expand bit depths lower than 8
	png_set_strip_16(png_ptr); //reduce bit depths higher than 8
	png_set_add_alpha(png_ptr,255,PNG_FILLER_AFTER); //add alpha if not present
	png_set_expand(png_ptr); //

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	int depth = png_get_bit_depth(png_ptr,info_ptr);
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	png_byte* image_data = (png_byte*)malloc(height*rowbytes);
	png_byte** row_pointers = (png_byte**)malloc(height*sizeof(png_byte*));
	for (unsigned int i=0; i<height; i++)
		row_pointers[i]=image_data + i*rowbytes;
	png_set_rows(png_ptr, info_ptr, row_pointers);
	
	// read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr,end_info);

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(fp);
	free(row_pointers);
	return image_data;
}

GLuint loadTexture(const char * file, unsigned int & width, unsigned int & height, unsigned int minFilter, unsigned int magFilter, unsigned int wrap)
{
	png_byte* image_data = readPNG(file,width,height);
	 
	// Generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap); 
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);


	// clean up
	free(image_data);
	return texture;
}
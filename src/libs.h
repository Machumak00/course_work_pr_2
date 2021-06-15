#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <png.h>
#include <math.h>

union RGBA{
    png_byte ptr[4];
};

struct pixel{
    int x;
    int y;
};

struct PNG{
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
};
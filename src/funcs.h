#pragma once

#include "libs.h"

void help();

void init_png(struct PNG *image);

void img_info(struct PNG *image);

int read_png_file(char *file_name, struct PNG *image);

int write_png_file(char *file_name, struct PNG *image);

void clean_row(struct PNG* image);

void pixel(struct PNG *image, int x, int y, int thickness, union RGBA * rgba1);

void line_hex(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA * rgba1);

void line_rect(struct PNG *image, int x, int y, union RGBA *rgba1);

void rect(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1);

void fill_rect(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1, union RGBA *rgba2);

void hex(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1);

void fill_hex(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1, union RGBA *rgba2);

int cmp(const png_byte *ptr1, union RGBA *rgba1);

void fill_checker(struct PNG *image, struct pixel **ptrbuf, int *i, png_byte *ptr2, int x1, int y1, union RGBA *rgba1, union RGBA *rgba2);

void f_hex(struct PNG *image, int x, int y, union RGBA *rgba1, union RGBA *rgba2);

void copy(struct PNG *image, int x1, int y1, int x2, int y2, int x3, int y3);

int char_to_int(char *a);

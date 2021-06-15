#include "funcs.h"

void help(){
    printf("You use a program that works with png images.\n"
           "First of all, you need to select the input file with the --input long key.\n"
           "Then you need to select the output file with the --output long key.\n"
           "Be careful entering file names.\n"
           "Short keys are also allowed in the program.\n\n"
           "Valid long keys with short ones in the brackets:\n"
           "1) --help (-h), to read the help information about the program;\n"
           "For example: ./main --help\n"
           "2) --input (-i), to specify the input file that must be entered;\n"
           "3) --output (-o), to specify the output file that must be entered;\n"
           "4) --begin (-b), to set the start coordinates;\n"
           "5) --end (-e), to set the end coordinates;\n"
           "6) --thickness (-t), to set the thickness of the line;\n"
           "7) --line (-l), to set the color of the line (RGBA);\n"
           "8) --fill (-f), to set the fill color (RGBA);\n"
           "9) --radius (-r), to set the radius and coordinates of the hexagon for the hexagon;\n"
           "10) --destination (-d), to set the destination coordinates for the copy of the area;\n"
           "11) --Rectangle (-R), marks an active state as the rectangle. This flag must be used with the following flags:\n"
           "    a) -b <x1>_<y1> -e <x2>_<y2> -t <thickness> -l <red_value>_<green_value>_<blue_value>_<alpha_value>\n"
           "    b) -b <x1>_<y1> -e <x2>_<y2> -t <thickness> -l <red_value>_<green_value>_<blue_value>_<alpha_value> -f <red_value>_<green_value>_<blue_value>_<alpha_value>\n"
           "Example 1 (without fill): ./main -i 1.png -o 12.png -R -b 0_0 -e 300_300 -t 4 -l 255_45_0_255\n"
           "Example 2 (with fill): ./main -i 1.png -o 12.png -R -b 25_76 -e 255_154 -t 1 -l 255_45_0_255 -f 0_243_54_120\n"
           "12) --Hexagon (-H), marks an active state as the hexagon. This flag must be used with the following flags:\n"
           "    a) -b <x1>_<y1> -e <x2>_<y2> -t <thickness> -l <red_value>_<green_value>_<blue_value>_<alpha_value>\n"
           "    b) -r <radius>_<x0>_<y0> -t <thickness> -l <red_value>_<green_value>_<blue_value>_<alpha_value>\n"
           "    c) -b <x1>_<y1> -e <x2>_<y2> -t <thickness> -l <red_value>_<green_value>_<blue_value>_<alpha_value> -f <red_value>_<green_value>_<blue_value>_<alpha_value>\n"
           "    d) -r <radius>_<x0>_<y0> -t <thickness> -l <red_value>_<green_value>_<blue_value>_<alpha_value> -f <red_value>_<green_value>_<blue_value>_<alpha_value>\n"
           "Example 1 (without fill, without radius): ./main -i 1.png -o 12.png -H -b 0_0 -e 300_300 -t 4 -l 255_45_0_255\n"
           "Example 2 (without fill, with radius): ./main -i 1.png -o 12.png -H -r 5_50_70 -t 4 -l 255_45_0_255\n"
           "Example 3 (with fill, without radius): ./main -i 1.png -o 12.png -H -b 25_76 -e 255_154 -t 1 -l 255_45_0_255 -f 0_243_54_120\n"
           "Example 4 (with fill, with radius): ./main -i 1.png -o 12.png -H -r 3_76_154 -t 1 -l 255_45_0_255 -f 0_243_54_120\n"
           "13) --Copy (-C), marks an active state as the copy of the area. This flag must be used with the following flags:\n"
           "    a) -b <x1>_<y1> -e <x2>_<y2> -d <x3>_<y3>\n"
           "Example 1: -i 1.png -o 12.png -C -b 0_0 -e 100_100 -d 200_200\n\n"
           "Be careful when you enter the coordinates and the color values. Between them there must be the underscore (_), otherwise the coordinates will not be recognized.\n");
}

void init_png(struct PNG *image){
    image->height = 0;
    image->width = 0;
    image->png_ptr = NULL;
    image->bit_depth = 0;
    image->color_type = 0;
    image->info_ptr = NULL;
    image->number_of_passes = 0;
    image->row_pointers = NULL;
}

void img_info(struct PNG *image){
    printf("Width is %d px.\nHeight is %d px.\nColor depth is %d bit.\nColor type is %d.\n\n",
           image->width, image->height, image->bit_depth, image->color_type);
}

int read_png_file(char *input, struct PNG *image){
    int y;
    png_byte header[8];

    FILE *fp = fopen(input, "rb");
    if (!fp){
        printf("Error. The input file is not found.\n");
        return 1;
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        printf("Error. The file is not a PNG image!\n");
        fclose(fp);
        return 1;
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        printf("The PNG file is invalid.\n");
        fclose(fp);
        return 1;
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        printf("The PNG file is invalid.\n");
        fclose(fp);
        return 1;
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        printf("The PNG file is invalid.\n");
        fclose(fp);
        return 1;
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    if (setjmp(png_jmpbuf(image->png_ptr))){
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        printf("The PNG file is invalid.\n");
        fclose(fp);
        return 1;
    }

    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
    return 0;
}

int write_png_file(char *file_name, struct PNG *image){
    int y;
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        printf("Error writing file.\n");
        fclose(fp);
        return 1;
    }

    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        printf("Error writing file.");
        fclose(fp);
        return 1;
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        printf("Error writing file.");
        fclose(fp);
        return 1;
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        printf("Error writing file.");
        fclose(fp);
        return 1;
    }

    png_init_io(image->png_ptr, fp);

    if (setjmp(png_jmpbuf(image->png_ptr))){
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        printf("Error writing file.");
        fclose(fp);
        return 1;
    }

    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);

    if (setjmp(png_jmpbuf(image->png_ptr))){
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        printf("Error writing file.");
        fclose(fp);
        return 1;
    }

    png_write_image(image->png_ptr, image->row_pointers);

    if (setjmp(png_jmpbuf(image->png_ptr))){
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        printf("Error writing file.");
        fclose(fp);
        return 1;
    }

    png_write_end(image->png_ptr, NULL);

    for (y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);

    fclose(fp);

    return 0;
}

void clean_row(struct PNG* image){
    for (int i = 0; i < image->height; i++)
        free(image->row_pointers[i]);
    free(image->row_pointers);
}

void pixel(struct PNG *image, int x, int y, int thickness, union RGBA * rgba1){
    for(int i = -thickness/2; i < thickness - thickness/2; i++)
        for(int j = -thickness/2; j < thickness - thickness/2; j++){
            if(y+i >= 0 && y+i < image->height && x+j >= 0 && x+j < image->width && i*i+j*j<=thickness*thickness/4){
                png_byte *ptr = &(image->row_pointers[y+i][(x+j)*4]);
                ptr[0] = rgba1->ptr[0];
                ptr[1] = rgba1->ptr[1];
                ptr[2] = rgba1->ptr[2];
                ptr[3] = rgba1->ptr[3];
            }
        }
}

void line_vd_hex(struct PNG *image, int x, int y, int length, int lengthX, int lengthY, int dx, int dy, int thickness, union RGBA * rgba1, int k){
    int d = -lengthX;
    length++;
    while(length--){
        if (k == 0)
            pixel(image, x, y, thickness, rgba1);
        else
            pixel(image, y, x, thickness, rgba1);
        x += dx;
        d += 2 * lengthY;
        if (d > 0){
            d -= 2 * lengthX;
            y += dy;
        }
    }
}

void line_hex(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA * rgba1){
    int dx = (x2 - x1 >= 0 ? 1 : -1);
    int dy = (y2 - y1 >= 0 ? 1 : -1);
    int lengthX = abs(x2 - x1);
    int lengthY = abs(y2 - y1);
    int length;
    int x = x1, y = y1, k = 0;
    if (lengthX > lengthY)
        length = lengthX;
    else
        length = lengthY;
    if (length == 0)
        pixel(image, x1, y1, thickness, rgba1);
    if (lengthY <= lengthX)
        line_vd_hex(image, x, y, length, lengthX, lengthY, dx, dy, thickness, rgba1, k);
    else {
        k = 1;
        line_vd_hex(image, y, x, length, lengthY, lengthX, dy, dx, thickness, rgba1, k);
    }
}

void line_rect(struct PNG *image, int x, int y, union RGBA *rgba1){
    png_byte *ptr = &(image->row_pointers[y][x * 4]);
    ptr[0] = rgba1->ptr[0];
    ptr[1] = rgba1->ptr[1];
    ptr[2] = rgba1->ptr[2];
    ptr[3] = rgba1->ptr[3];
}

void rect(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1) {
    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++){
            if (x < x1 + thickness || x > x2 - thickness || y < y1 + thickness || y > y2 - thickness){
                line_rect(image, x, y, rgba1);
            }
        }
}

void fill_rect(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1, union RGBA *rgba2){
    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++){
            if (x < x1 + thickness || x > x2 - thickness || y < y1 + thickness || y > y2 - thickness){
                line_rect(image, x, y, rgba1);
            }
        }
    for (int y = y1 + thickness; y < y2 - thickness + 1; y++) {
        for (int x = x1 + thickness; x < x2 - thickness + 1; x++) {
            line_rect(image, x, y, rgba2);
        }
    }
}

void hex(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1){
    int center[2] = {(x2+x1)/2, (y2+y1)/2};
    int radius = (x2-x1)/2;
    int arr[6][2];
    arr[0][0] = center[0]; arr[0][1] = center[1]-radius;
    arr[1][0] = (int)(center[0]-radius*cos(M_PI/6)); arr[1][1] = center[1]-radius/2;
    arr[2][0] = (int)(center[0]-radius*cos(M_PI/6)); arr[2][1] = center[1]+radius/2;
    arr[3][0] = center[0]; arr[3][1] = center[1]+radius;
    arr[4][0] = (int)(center[0]+radius*cos(M_PI/6)); arr[4][1] = center[1]+radius/2;
    arr[5][0] = (int)(center[0]+radius*cos(M_PI/6)); arr[5][1] = center[1]-radius/2;
    for (int i = 0, j = 1; i < 6; i++, j++){
        if (i == 5)
            j = 0;
        line_hex(image, arr[i][0], arr[i][1], arr[j][0], arr[j][1], thickness, rgba1);
    }
}

void fill_hex(struct PNG *image, int x1, int y1, int x2, int y2, int thickness, union RGBA *rgba1, union RGBA *rgba2){
    hex(image, x1, y1, x2, y2, thickness, rgba1);
    f_hex(image, x2 - (x2 - x1) / 2, y2 - (y2 - y1) / 2, rgba1, rgba2);
}

int cmp(const png_byte *ptr1, union RGBA *rgba1){
    if (ptr1[0] == rgba1->ptr[0] && ptr1[1] == rgba1->ptr[1] && ptr1[2] == rgba1->ptr[2] &&
        ptr1[3] == rgba1->ptr[3])
        return 0;
    else return 1;
}

void fill_checker(struct PNG *image, struct pixel **ptrbuf, int *i, png_byte *ptr2, int x1, int y1, union RGBA *rgba1, union RGBA *rgba2){
    ptr2 = &(image->row_pointers[y1][x1 * 4]);
    if (cmp(ptr2, rgba1) && cmp(ptr2, rgba2)){
        *ptrbuf = realloc(*ptrbuf, ((*i) + 1) * sizeof(struct pixel));
        ((*ptrbuf)[*i]).x = x1;
        ((*ptrbuf)[(*i)++]).y = y1;
        line_rect(image, x1, y1, rgba2);
    }
}

void f_hex(struct PNG *image, int x, int y, union RGBA *rgba1, union RGBA *rgba2){
    struct pixel *ptrbuf = malloc(sizeof(struct pixel));
    png_byte *ptr2;
    int i = 0, x1, y1, n = 0;
    ptrbuf[i].x = x;
    ptrbuf[i++].y = y;
    do {
        x = ptrbuf[n].x;
        y = ptrbuf[n++].y;
        if (y > 0){
            x1 = x;
            y1 = y - 1;
            fill_checker(image, &ptrbuf, &i, ptr2, x1, y1, rgba1, rgba2);
        }
        if (y < image->height - 1){
            x1 = x;
            y1 = y + 1;
            fill_checker(image, &ptrbuf, &i, ptr2, x1, y1, rgba1, rgba2);
        }
        if (x > 0){
            x1 = x - 1;
            y1 = y;
            fill_checker(image, &ptrbuf, &i, ptr2, x1, y1, rgba1, rgba2);
        }
        if (x < image->width - 1){
            x1 = x + 1;
            y1 = y;
            fill_checker(image, &ptrbuf, &i, ptr2, x1, y1, rgba1, rgba2);
        }
    } while (n < i);
    free(ptrbuf);
}

void copy(struct PNG *image, int x1, int y1, int x2, int y2, int x3, int y3){
    png_byte buf[y2-y1][(x2-x1)*4];
    for (int i = 0; i < y2-y1; i++)
        for (int j = 0; j < x2-x1; j++){
            png_byte *ptr1 = &(image->row_pointers[i+y1][(j+x1) * 4]);
            buf[i][j*4] = ptr1[0];
            buf[i][j*4+1] = ptr1[1];
            buf[i][j*4+2] = ptr1[2];
            buf[i][j*4+3] = ptr1[3];
        }
    for (int i = 0; i < y2-y1; i++)
        for (int j = 0; j < x2-x1; j++){
            png_byte *ptr1 = &(image->row_pointers[i + y3][(j + x3) * 4]);
            png_byte *ptr2 = &buf[i][j*4];
            ptr1[0] = ptr2[0];
            ptr1[1] = ptr2[1];
            ptr1[2] = ptr2[2];
            ptr1[3] = ptr2[3];
        }
}

int char_to_int(char *a) {
    if (!a) {
        return -1;
    }
    for (int i = 0; i < strlen(a); i++) {
        if (!isdigit(a[i])) {
            return -1;
        }
    }
    return atoi(a);
}

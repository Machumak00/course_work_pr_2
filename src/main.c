#include "funcs.h"

int main(int argc, char **argv){
    if (argc == 1) {
        help();
        return 0;
    }
    if (argc < 5 && !((!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))){
        printf("Please read the reference information carefully.\n");
        help();
        return 0;
    }
    if (!(!strcmp(argv[1], "--input") && !strcmp(argv[3], "--output")) &&
        !(!strcmp(argv[1], "-i") && !strcmp(argv[3], "-o")) && !(!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))){
        printf("Please read the reference information carefully.\n");
        help();
        return 0;
    }

    char *short_opts = "hRHCi:o:t:b:e:l:f:r:d:";
    struct option long_opts[] = {
            {"help", no_argument,       NULL, 'h'},
            {"Rectangle", no_argument,       NULL, 'R'},
            {"Hexagon", no_argument,       NULL, 'H'},
            {"Copy", no_argument,       NULL, 'C'},
            {"input", required_argument,       NULL, 'i'},
            {"output", required_argument,       NULL, 'o'},
            {"thickness", required_argument,       NULL, 't'},
            {"begin", required_argument,       NULL, 'b'},
            {"end", required_argument,       NULL, 'e'},
            {"line", required_argument,       NULL, 'l'},
            {"fill", required_argument,       NULL, 'f'},
            {"radius", required_argument,       NULL, 'r'},
            {"destination", required_argument,       NULL, 'd'},
            {NULL, no_argument, NULL, 0}
    };

    int opt, long_index = 0;
    int begin[2] = {-1, -1}, end[2] = {-1, -1}, dest[2] = {-1, -1},
            radius = -1, center[2] = {-1, -1},thickness = -1,  fill = 0;
    union RGBA rgba1, rgba2;
    rgba1.ptr[0] = 0, rgba1.ptr[1] = 0, rgba1.ptr[2] = 0, rgba1.ptr[3] = 0;
    rgba2.ptr[0] = 0, rgba2.ptr[1] = 0, rgba2.ptr[2] = 0, rgba2.ptr[3] = 0;
    char *input = NULL, *output = NULL, *state = NULL;

    struct PNG image;
    init_png(&image);

    opt = getopt_long(argc, argv, short_opts, long_opts, &long_index);
    while (opt != -1) {
        switch (opt) {
            case 'h':
                help();
                return 0;
            case 'R':
                state = "rectangle";
                printf("You selected the next figure: %s.\n", state);
                break;
            case 'H':
                state = "hexagon";
                printf("You selected the next figure: %s.\n", state);
                break;
            case 'C':
                state = "copy";
                printf("You selected the copy of the area.\n");
                break;
            case 'i':
                input = optarg;
                printf("Input file is: %s\n", input);
                break;
            case 'o':
                output = optarg;
                printf("Output file is: %s\n", output);
                break;
            case 't':
                if (!state || strcmp(state, "copy") == 0){
                    printf("In order to use this function, you need to select the rectangle or the hexagon.\n");
                    return 0;
                }
                thickness = char_to_int(optarg);
                if (thickness == -1){
                    printf("The thickness is wrong.\n");
                    return 0;
                }
                printf("Thickness of the line is: %d\n", thickness);
                break;
            case 'b':
                if (!state){
                    printf("In order to use this function, you need to select the rectangle, or the hexagon, or the copy of the area.\n");
                    return 0;
                }
                begin[0] = char_to_int(strtok(optarg, "_"));
                if (begin[0] == -1){
                    printf("The x begin coordinate is wrong.\n");
                    return 0;
                }
                begin[1] = char_to_int(strtok(NULL, "_"));
                if (begin[1] == -1){
                    printf("The y begin coordinate is wrong.\n");
                    return 0;
                }
                printf("Starting coordinates are: x = %d, y = %d\n", begin[0], begin[1]);
                break;
            case 'e':
                if (!state){
                    printf("In order to use this function, you need to select the rectangle, "
                           "or the hexagon, or the copy of the area.\n");
                    return 0;
                }
                end[0] = char_to_int(strtok(optarg, "_"));
                if (end[0] == -1){
                    printf("The x ending coordinate is wrong.\n");
                    return 0;
                }
                end[1] = char_to_int(strtok(NULL, "_"));
                if (end[1] == -1){
                    printf("The y ending coordinate is wrong.\n");
                    return 0;
                }
                printf("Ending coordinates are: x = %d, y = %d\n", end[0], end[1]);
                break;
            case 'l':
                if (!state || !strcmp(state, "copy")){
                    printf("In order to use this function, you need to select the rectangle or the hexagon.\n");
                    return 0;
                }
                rgba1.ptr[0] = char_to_int(strtok(optarg, "_"));
                rgba1.ptr[1] = char_to_int(strtok(NULL, "_"));
                rgba1.ptr[2] = char_to_int(strtok(NULL, "_"));
                rgba1.ptr[3] = char_to_int(strtok(NULL, "_"));
                printf("Color of the line is: %d %d %d %d\n", rgba1.ptr[0], rgba1.ptr[1], rgba1.ptr[2], rgba1.ptr[3]);
                break;
            case 'f':
                if (!state || !strcmp(state, "copy")){
                    printf("In order to use this function, you need to select the rectangle or the hexagon.\n");
                    return 0;
                }
                fill = 1;
                rgba2.ptr[0] = char_to_int(strtok(optarg, "_"));
                rgba2.ptr[1] = char_to_int(strtok(NULL, "_"));
                rgba2.ptr[2] = char_to_int(strtok(NULL, "_"));
                rgba2.ptr[3] = char_to_int(strtok(NULL, "_"));
                printf("Filled color is: %d %d %d %d\n", rgba2.ptr[0], rgba2.ptr[1], rgba2.ptr[2], rgba2.ptr[3]);
                break;
            case 'r':
                if (!state || !strcmp(state, "copy")){
                    printf("In order to use this function, you need to select the rectangle or the hexagon.\n");
                    return 0;
                }
                radius = char_to_int(strtok(optarg, "_"));
                if (radius == -1){
                    printf("The radius is wrong.\n");
                    return 0;
                }
                center[0] = char_to_int(strtok(NULL, "_"));
                if (center[0] == -1){
                    printf("The x center coordinate is wrong.\n");
                    return 0;
                }
                center[1] = char_to_int(strtok(NULL, "_"));
                if (center[1] == -1){
                    printf("The y center coordinate is wrong.\n");
                    return 0;
                }
                printf("Radius and center coordinates are: %d %d %d\n", radius, center[0], center[1]);
                break;
            case 'd':
                if (!state || !strcmp(state, "rectangle") || !strcmp(state, "hexagon")){
                    printf("In order to use this function, you need to select the copy of the area.\n");
                    return 0;
                }
                if (begin[0] == -1 || begin[1] == -1 || end[0] == -1 || end[1] == -1){
                    printf("In order to use this function, you need to enter the start and the end coordinates.\n");
                    return 0;
                }
                dest[0] = char_to_int(strtok(optarg, "_"));
                if (dest[0] == -1){
                    printf("The x destination coordinate is wrong.\n");
                    return 0;
                }
                dest[1] = char_to_int(strtok(NULL, "_"));
                if (dest[1] == -1){
                    printf("The y destination coordinate is wrong.\n");
                    return 0;
                }
                printf("Destination coordinates are: x = %d, y = %d\n", dest[0], dest[1]);
                break;
            default:
                break;
        }
        opt = getopt_long(argc, argv, short_opts, long_opts, &long_index);
    }
    if(!state){
        printf("Error. Please use -C, -R or -H keys to select the function.\n");
        return 0;
    }
    if ((begin[0] < 0 || begin[1] < 0 || end[0] < 0 || end[1] < 0) && !strcmp(state, "rectangle") && !strcmp(state, "copy")){
        printf("Error. Incorrect coordinates were entered. The coordinates "
               " must not be <0 value.\n");
        return 0;
    }
    if (((begin[0] < 0 || begin[1] < 0 || end[0] < 0 || end[1] < 0)) && !strcmp(state, "hexagon") && radius <= 0){
        printf("Error. Incorrect coordinates were entered. The coordinates "
               "must not be <0 value.\n");
        return 0;
    }
    if (thickness <= 0 && strcmp(state, "copy") != 0){
        printf("Error. The thickness of the line must not be <=0 value.\n");
        return 0;
    }
    if (!strcmp(state, "copy") && (dest[0] < 0 || dest[1] < 0)){
        printf("Error. Please enter the correct value of the destination coordinates.\n");
        return 0;
    }
    if ((begin[0] > end[0] || begin[1] > end[1]) && !radius){
        printf("Error. Incorrect coordinates were entered. The starting coordinates must be less "
               "than the ending coordinates. Maybe there are radius with 0 value.\n");
        return 0;
    }
    if ((begin[0] == end[0] || begin[1] == end[1]) && radius <= 0){
        printf("Error. Incorrect coordinates were entered. The starting "
               "coordinates must not be equal to the ending ones.\n");
        return 0;
    }

    if (read_png_file(input, &image))
        return 0;
    if (png_get_color_type(image.png_ptr, image.info_ptr) == PNG_COLOR_TYPE_RGB){
        printf("This program only works with PNG files of the RGBA type.\n");
        clean_row(&image);
        return 0;
    }

    printf("\nInput file info:\n");
    img_info(&image);

    if (!strcmp(state, "copy") && ((end[0] >= image.width) || (end[1] >= image.height) ||
                                   (dest[0] + (end[0] - begin[0]) >= image.width) || (dest[1] + (end[1] - begin[1])  >= image.height))){
        printf("Error. Please enter the correct value of the coordinates.\n");
        return 0;
    }
    if ((thickness/2 > abs(image.height - end[1]) && end[1] >= 0 || thickness/2 > begin[1] && begin[1] >= 0
         || thickness/2 > abs(image.width - end[0]) && end[0] >= 0 || thickness/2 >= begin[0] && begin[0] > 0) && !strcmp(state, "hexagon")){
        printf("Error. The frames are too big. The width of the frames "
               "should be a maximum of half the height of the figure.\n");
        clean_row(&image);
        return 0;
    }
    if (((thickness/2 > abs(image.height - center[1]) && center[1] > 0 ||
          thickness/2 > abs(image.width - center[0]) && center[0] > 0)
         || ((thickness/2 > center[0]-radius) && center[0] > 0)) && !strcmp(state, "hexagon")){
        printf("Error. The frames are too big. The width of the frames "
               "should be a maximum of half the height of the figure.\n");
        clean_row(&image);
        return 0;
    }
    if ((center[0] + radius >= image.width) || (center[1] + radius >= image.height)){
        printf("Error. The frames are too big.\n");
        clean_row(&image);
        return 0;
    }
    if (begin[0] >= image.width || begin[1] >= image.height || end[0] >= image.width || end[1] >= image.height){
        printf("Error. Incorrect coordinates were entered. Exited file array.\n");
        clean_row(&image);
        return 0;
    }

    if (!strcmp(state, "rectangle") && fill == 1){
        state = "filledRect";
    }
    if (!strcmp(state, "hexagon") && fill == 1){
        state = "filledHex";
    }

    if (!strcmp(state, "rectangle")){
        rect(&image, begin[0], begin[1], end[0], end[1], thickness, &rgba1);
    } else if (!strcmp(state, "filledRect")){
        fill_rect(&image, begin[0], begin[1], end[0], end[1], thickness, &rgba1, &rgba2);
    } else if (!strcmp(state, "hexagon")){
        if (center[0] > 0 && center[1] > 0){
            begin[0] = center[0];
            begin[1] = center[1];
        }
        if (radius > 0){
            end[0] = begin[0] + radius;
            end[1] = begin[1] + radius;
            begin[0] = begin[0] - radius;
            begin[1] = begin[1] - radius;
        } else
        if (end[0] - begin[0] != end[1] - begin[1]){
            printf("Error. It is not a square.\n");
            clean_row(&image);
            return 0;
        }
        if (begin[0] < 0 || end[0] < 0 || begin[1] < 0 || end[1] < 0){
            printf("Error. Incorrect coordinates were entered. Exited file array.\n");
            clean_row(&image);
            return 0;
        }
        hex(&image, begin[0], begin[1], end[0], end[1], thickness, &rgba1);
    } else if (!strcmp(state, "filledHex")){
        if (center[0] > 0 && center[1] > 0){
            begin[0] = center[0];
            begin[1] = center[1];
        }
        if (radius > 0){
            end[0] = begin[0] + radius;
            end[1] = begin[1] + radius;
            begin[0] = begin[0] - radius;
            begin[1] = begin[1] - radius;
        } else
        if (end[0] - begin[0] != end[1] - begin[1]){
            printf("Error. It is not a square.\n");
            clean_row(&image);
            return 0;
        }
        if (begin[0] < 0 || end[0] < 0 || begin[1] < 0 || end[1] < 0){
            printf("Error. Incorrect coordinates were entered. Exited file array.\n");
            clean_row(&image);
            return 0;
        }
        fill_hex(&image, begin[0], begin[1], end[0], end[1], thickness, &rgba1, &rgba2);
    } else if (!strcmp(state, "copy"))
        copy(&image, begin[0], begin[1], end[0], end[1], dest[0], dest[1]);

    if (write_png_file(output, &image)){
        clean_row(&image);
        return 0;
    }
    printf("Output file info:\n");
    img_info(&image);

    return 0;
}

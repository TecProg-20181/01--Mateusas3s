#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} Image;

int searchSmaller(int value1, int value2) {
    if (value1 > value2 )
        return value2;
    return value1;
}

int searchBigger(int value1, int value2) {
    if (value1 > value2 )
        return value1;
    return value2;
}

Image applyGrayFilter(Image image) {
    
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            int mean = image.pixel[column][line].red +
                        image.pixel[column][line].green +
                        image.pixel[column][line].blue;
            mean /= 3;
            image.pixel[column][line].red = mean;
            image.pixel[column][line].green = mean;
            image.pixel[column][line].blue = mean;
            
        }
    }
    return image;
}

Image applyBlurFilter(Image image, int size) {
   
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            Pixel mean = {0, 0, 0};
            int smaller_height = searchSmaller(image.height - 1, column + size/2);
            int smaller_width = searchSmaller(image.width - 1, line + size/2);
            int x_column = searchBigger(0, column - size/2);
            
            for(x_column; x_column <= smaller_height; ++x_column) {
                int y_line = searchBigger(0, line - size/2);
                for(y_line; y_line <= smaller_width; ++y_line) {
                    mean.red += image.pixel[x_column][y_line].red;
                    mean.green += image.pixel[x_column][y_line].green;
                    mean.blue += image.pixel[x_column][y_line].blue;
                }
            }

            mean.red /= size * size;
            mean.green /= size * size;
            mean.blue /= size * size;

            image.pixel[column][line].red = mean.red;
            image.pixel[column][line].green = mean.green;
            image.pixel[column][line].blue = mean.blue;
            
        }
    }
    return image;
}

Image applyRotation90Right(Image image) {
    
    Image image_rotate;

    image_rotate.width = image.height;
    image_rotate.height = image.width;

    for (unsigned int column = 0, y_line = 0; column < image_rotate.height; ++column, ++y_line) {
        for (int line = image_rotate.width - 1, x_column = 0; line >= 0; --line, ++x_column) {
            
            image_rotate.pixel[column][line].red = image.pixel[x_column][y_line].red;
            image_rotate.pixel[column][line].green = image.pixel[x_column][y_line].green;
            image_rotate.pixel[column][line].blue = image.pixel[x_column][y_line].blue;
            
        }
    }
    return image_rotate;
}

Image applyReverseFilter(Image image) {
    
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            image.pixel[column][line].red = 255 - image.pixel[column][line].red;
            image.pixel[column][line].green = 255 - image.pixel[column][line].green;
            image.pixel[column][line].blue = 255 - image.pixel[column][line].blue;
            
        }
    }
    return image;
}

Image applySepiaFilter(Image image){

    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            Pixel pixels;
            pixels.red = image.pixel[column][line].red;
            pixels.green = image.pixel[column][line].green;
            pixels.blue = image.pixel[column][line].blue;
            int pixel =  pixels.red * .393 + pixels.green * .769 + pixels.blue * .189;
            int smaller_pixel = (255 >  pixel) ? pixel : 255;
            
            image.pixel[column][line].red = smaller_pixel;
            pixel =  pixels.red * .349 + pixels.green * .686 + pixels.blue * .168;
            smaller_pixel = (255 >  pixel) ? pixel : 255;
            image.pixel[column][line].green = smaller_pixel;
            pixel =  pixels.red * .272 + pixels.green * .534 + pixels.blue * .131;
            smaller_pixel = (255 >  pixel) ? pixel : 255;
            image.pixel[column][line].blue = smaller_pixel;
            
        }
    }
    return image;
}

Image applyDropImage(Image image, int axis_x, int axis_y, int width_drop, int height_drop) {
    
    Image image_drop;

    image_drop.width = width_drop;
    image_drop.height = height_drop;

    for(int column = 0; column < height_drop; ++column) {
        for(int line = 0; line < width_drop; ++line) {
           
            image_drop.pixel[column][line].red = image.pixel[column + axis_y][line + axis_x].red;
            image_drop.pixel[column][line].green = image.pixel[column + axis_y][line+ axis_x].green;
            image_drop.pixel[column][line].blue = image.pixel[column + axis_y][line + axis_x].blue;
        
        }
    }
    return image_drop;
}

Image applyMirroring(Image image){

    int axis_x = 0;
    scanf("%d", &axis_x);

    int width = image.width, height = image.height;

    if (axis_x == 1) width /= 2;
    else height /= 2;   
    
    for (int column = 0; column < height; ++column) {
       for (int line = 0; line < width; ++line) {
            
            int x_column = column, y_line = line; 
            
            if (axis_x == 1) y_line = image.width - 1 - line;
            else x_column = image.height - 1 - column;
            
            Pixel pixel_temporary;
            pixel_temporary.red = image.pixel[column][line].red;
            pixel_temporary.green = image.pixel[column][line].green;
            pixel_temporary.blue = image.pixel[column][line].blue; 
            image.pixel[column][line].red = image.pixel[x_column][y_line].red;
            image.pixel[column][line].green = image.pixel[x_column][y_line].green;
            image.pixel[column][line].blue = image.pixel[x_column][y_line].blue;  
            image.pixel[x_column][y_line].red = pixel_temporary.red;
            image.pixel[x_column][y_line].green = pixel_temporary.green;
            image.pixel[x_column][y_line].blue = pixel_temporary.blue;

       }
    }  
    return image;
}


int main() {
    Image image;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int maximum_color;
    scanf("%u %u %d", &image.width, &image.height, &maximum_color);

    // read all pixels of image
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            scanf("%hu %hu %hu", &image.pixel[column][line].red,
                                 &image.pixel[column][line].green,
                                 &image.pixel[column][line].blue);

        }
    }

    int number_options;
    scanf("%d", &number_options);

    for(int counter = 0; counter < number_options; ++counter) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Gray Scale Filter

                image = applyGrayFilter(image);
                break;

            }
            case 2: { // Sepia Filter

                image = applySepiaFilter(image);
                break;

            }
            case 3: { // Blur Filter

                int size = 0;
                scanf("%d", &size);
                image = applyBlurFilter(image, size);
                break;

            }
            case 4: { // Rotation Image

                int number_time = 0;
                scanf("%d", &number_time);
                number_time %= 4;
                for (int counter = 0; counter < number_time; ++counter) {
                    image = applyRotation90Right(image);
                }
                break;

            }
            case 5: { // Mirroring Image

                image = applyMirroring(image);
                break;

            }
            case 6: { // Reverse Filter

                image = applyReverseFilter(image);
                break;

            }
            case 7: { // Drop Image

                int axis_x, axis_y;
                scanf("%d %d", &axis_x, &axis_y);
                int width_drop, height_drop;
                scanf("%d %d", &width_drop, &height_drop);

                image = applyDropImage(image, axis_x, axis_y, width_drop, height_drop);
                break;

            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", image.width, image.height);

    // print pixels of image
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j].red,
                                   image.pixel[i][j].green,
                                   image.pixel[i][j].blue);

        }
        printf("\n");
    }
    return 0;
}

#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    /* 
     Estrutura de um pixel:
        pixel[width][height][rgb]
     Índice das cores:
        0 -> red
        1 -> green
        2 -> blue
    */
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

Image applyGrayFilter(Image image) {
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            int mean = image.pixel[column][line][0] +
                        image.pixel[column][line][1] +
                        image.pixel[column][line][2];
            mean /= 3;
            image.pixel[column][line][0] = mean;
            image.pixel[column][line][1] = mean;
            image.pixel[column][line][2] = mean;
            
        }
    }
    return image;
}

Image applyBlurFilter(Image image, int size) {
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            Pixel mean = {0, 0, 0};
            int smaller_height = (image.height - 1 > column + size/2) ? column + size/2 : image.height - 1;
            int smaller_width = (image.width - 1 > line + size/2) ? line + size/2 : image.width - 1;
            for(int x_column = (0 > column - size/2 ? 0 : column - size/2); x_column <= smaller_height; ++x_column) {
                for(int y_line = (0 > line - size/2 ? 0 : line - size/2); y_line <= smaller_width; ++y_line) {
                    mean.red += image.pixel[x_column][y_line][0];
                    mean.green += image.pixel[x_column][y_line][1];
                    mean.blue += image.pixel[x_column][y_line][2];
                }
            }

            mean.red /= size * size;
            mean.green /= size * size;
            mean.blue /= size * size;

            image.pixel[column][line][0] = mean.red;
            image.pixel[column][line][1] = mean.green;
            image.pixel[column][line][2] = mean.blue;
            
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
            
            image_rotate.pixel[column][line][0] = image.pixel[x_column][y_line][0];
            image_rotate.pixel[column][line][1] = image.pixel[x_column][y_line][1];
            image_rotate.pixel[column][line][2] = image.pixel[x_column][y_line][2];
            
        }
    }

    return image_rotate;
}

Image applyReverseFilter(Image image) {
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            image.pixel[column][line][0] = 255 - image.pixel[column][line][0];
            image.pixel[column][line][1] = 255 - image.pixel[column][line][1];
            image.pixel[column][line][2] = 255 - image.pixel[column][line][2];
            
        }
    }
    return image;
}

Image applySepiaFilter(Image image){
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            unsigned short int pixels[3];
            pixels[0] = image.pixel[column][line][0];
            pixels[1] = image.pixel[column][line][1];
            pixels[2] = image.pixel[column][line][2];
            int pixel =  pixels[0] * .393 + pixels[1] * .769 + pixels[2] * .189;
            int smaller_pixel = (255 >  pixel) ? pixel : 255;
            
            image.pixel[column][line][0] = smaller_pixel;
            pixel =  pixels[0] * .349 + pixels[1] * .686 + pixels[2] * .168;
            smaller_pixel = (255 >  pixel) ? pixel : 255;
            image.pixel[column][line][1] = smaller_pixel;
            pixel =  pixels[0] * .272 + pixels[1] * .534 + pixels[2] * .131;
            smaller_pixel = (255 >  pixel) ? pixel : 255;
            image.pixel[column][line][2] = smaller_pixel;
            
        }
    }
    return image;
}

Image cortar_imagem(Image img, int x, int y, int w, int h) {
    Image cortada;

    cortada.width = w;
    cortada.height = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cortada;
}


int main() {
    Image image;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &image.width, &image.height, &max_color);

    // read all pixels of image
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            
            scanf("%hu %hu %hu", &image.pixel[column][line][0],
                                 &image.pixel[column][line][1],
                                 &image.pixel[column][line][2]);

        }
    }

    int number_options;
    scanf("%d", &number_options);

    for(int column = 0; column < number_options; ++column) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Escala de Cinza
                image = applyGrayFilter(image);
                break;
            }
            case 2: { // Filtro Sepia
                image = applySepiaFilter(image);
                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                image = applyBlurFilter(image, size);
                break;
            }
            case 4: { // Rotacao
                int number_time = 0;
                scanf("%d", &number_time);
                number_time %= 4;
                for (int j = 0; j < number_time; ++j) {
                    image = applyRotation90Right(image);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = image.width, h = image.height;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i2 = 0; i2 < h; ++i2) {
                    for (int j = 0; j < w; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = image.width - 1 - j;
                        else x = image.height - 1 - i2;

                        Pixel aux1;
                        aux1.red = image.pixel[i2][j][0];
                        aux1.green = image.pixel[i2][j][1];
                        aux1.blue = image.pixel[i2][j][2];

                        image.pixel[i2][j][0] = image.pixel[x][y][0];
                        image.pixel[i2][j][1] = image.pixel[x][y][1];
                        image.pixel[i2][j][2] = image.pixel[x][y][2];

                        image.pixel[x][y][0] = aux1.red;
                        image.pixel[x][y][1] = aux1.green;
                        image.pixel[x][y][2] = aux1.blue;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                image = applyReverseFilter(image);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                image = cortar_imagem(image, x, y, w, h);
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
            printf("%hu %hu %hu ", image.pixel[i][j][0],
                                   image.pixel[i][j][1],
                                   image.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}

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
// T ?
Image applyBlurFilter(Image image, int T) {
    for (unsigned int column = 0; column < image.height; ++column) {
        for (unsigned int line = 0; line < image.width; ++line) {
            Pixel mean = {0, 0, 0};

            int smaller_height = (image.height - 1 > column + T/2) ? column + T/2 : image.height - 1;
            int smaller_width = (image.width - 1 > line + T/2) ? line + T/2 : image.width - 1;
            for(int x_column = (0 > column - T/2 ? 0 : column - T/2); x_column <= smaller_height; ++x_column) {
                for(int y_line = (0 > line - T/2 ? 0 : line - T/2); y_line <= smaller_width; ++y_line) {
                    mean.red += image.pixel[x_column][y_line][0];
                    mean.green += image.pixel[x_column][y_line][1];
                    mean.blue += image.pixel[x_column][y_line][2];
                }
            }

            mean.red /= T * T;
            mean.green /= T * T;
            mean.blue /= T * T;

            image.pixel[column][line][0] = mean.red;
            image.pixel[column][line][1] = mean.green;
            image.pixel[column][line][2] = mean.blue;
        }
    }
    return image;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void inverter_cores(unsigned short int pixel[512][512][3],
                    unsigned int w, unsigned int h) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
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
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            scanf("%hu %hu %hu", &image.pixel[i][j][0],
                                 &image.pixel[i][j][1],
                                 &image.pixel[i][j][2]);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                image = applyGrayFilter(image);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < image.height; ++x) {
                    for (unsigned int j = 0; j < image.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = image.pixel[x][j][0];
                        pixel[1] = image.pixel[x][j][1];
                        pixel[2] = image.pixel[x][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        image.pixel[x][j][0] = menor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        menor_r = (255 >  p) ? p : 255;
                        image.pixel[x][j][1] = menor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        menor_r = (255 >  p) ? p : 255;
                        image.pixel[x][j][2] = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                image = applyBlurFilter(image, size);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    image = rotacionar90direita(image);
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
                inverter_cores(image.pixel, image.width, image.height);
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

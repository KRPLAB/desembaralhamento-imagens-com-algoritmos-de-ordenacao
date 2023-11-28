/* Este programa extrai os dados de cada pixel de uma imagem
 * referentes às cores em escala cinza e os imprime em forma de 
 * um vetor bidimensional (matriz) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define HEIGHT 100
#define WIDTH 100

void extractPixelValues(SDL_Surface *imageSurface, Uint8 matrix[HEIGHT][WIDTH]) {
    int imgWidth = imageSurface->w;
    int imgHeight = imageSurface->h;

    if (imageSurface->format->BytesPerPixel != 4) {
        fprintf(stderr, "Formato de imagem não suportado. Certifique-se de que a imagem tem 32 bits por pixel.\n");
        return;
    }

    Uint32 *pixels = (Uint32 *)imageSurface->pixels;

    for (int i = 0; i < imgHeight; i++) {
        for (int j = 0; j < imgWidth; j++) {
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixels[i * imgWidth + j], imageSurface->format, &r, &g, &b, &a);
            matrix[i][j] = r;
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    /* Carregar imagem de um arquivo BMP */
    SDL_Surface *imageSurface = SDL_LoadBMP("./mona100x100.bmp");

    /* Verificar se a imagem foi carregada com sucesso */
    if (!imageSurface) {
        fprintf(stderr, "Erro ao carregar imagem: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Uint8 pixelValues[HEIGHT][WIDTH];

    /* Extrair os valores dos pixels para a matriz */
    extractPixelValues(imageSurface, pixelValues);

    /* Exibir os valores da matriz (apenas para visualização) */
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%3u ", pixelValues[i][j]);
        }
        printf("\n");
    }

    /* Liberar recursos */
    SDL_FreeSurface(imageSurface);
    SDL_Quit();

    return 0;
}
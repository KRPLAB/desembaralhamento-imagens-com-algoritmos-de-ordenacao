/* Este programa extrai os dados de cada pixel de uma imagem
 * referentes às cores em escala RGB e os imprime em forma de
 * um vetor bidimensional (matriz) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define HEIGHT 1526
#define WIDTH 1024

typedef struct
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
} RGB;

void extractRGBValues(SDL_Surface *imageSurface, RGB matrix[HEIGHT][WIDTH])
{
    Uint32 *pixels = (Uint32 *)imageSurface->pixels;
    int imgWidth = imageSurface->w;
    int imgHeight = imageSurface->h;

    for (int i = 0; i < imgHeight; i++)
    {
        for (int j = 0; j < imgWidth; j++)
        {
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixels[i * imgWidth + j], imageSurface->format, &r, &g, &b, &a);
            matrix[i][j].r = r;
            matrix[i][j].g = g;
            matrix[i][j].b = b;
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    /* Carregar imagem de um arquivo BMP */
    SDL_Surface *imageSurface = SDL_LoadBMP("mona.bmp");

    /* Verificar se a imagem foi carregada com sucesso */
    if (!imageSurface)
    {
        fprintf(stderr, "Erro ao carregar imagem: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    RGB rgbValues[HEIGHT][WIDTH];

    /* Extrair os valores RGB dos pixels para a matriz */
    extractRGBValues(imageSurface, rgbValues);

    /* Exibir os valores RGB da matriz (apenas para visualização) */
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("(%3u, %3u, %3u) ", rgbValues[i][j].r, rgbValues[i][j].g, rgbValues[i][j].b);
        }
        printf("\n");
    }

    /* Liberar recursos */
    SDL_FreeSurface(imageSurface);
    SDL_Quit();

    return 0;
}

/* Este programa cria uma imagem atraves de um arquivo .txt
 * que contenha os dados referentes aos pixels da imagem em
 * forma de um vetor bidimensional (matriz) */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define HEIGHT 500
#define WIDTH 500

void generateImageFromMatrix(const char *inputFileName, const char *outputFileName) {
    // Abrir o arquivo para leitura
    FILE *inputFile = fopen(inputFileName, "r");
    if (!inputFile) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", inputFileName);
        return;
    }

    // Criar uma matriz para armazenar os valores do arquivo
    int matrix[HEIGHT][WIDTH];
    memset(matrix, 0, sizeof(matrix)); // Inicializar com zeros

    // Ler a matriz do arquivo
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (fscanf(inputFile, "%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Erro ao ler a matriz do arquivo.\n");
                fclose(inputFile);
                return;
            }
        }
    }

    // Fechar o arquivo
    fclose(inputFile);

    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erro ao inicializar SDL: %s\n", SDL_GetError());
        return;
    }

    // Criar uma janela e renderer (para gerar a imagem)
    SDL_Window *window = SDL_CreateWindow("Image from Matrix", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Verificar se a janela e o renderer foram criados com sucesso
    if (!window || !renderer) {
        fprintf(stderr, "Erro ao criar janela ou renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Criar uma superfície para a imagem
    SDL_Surface *imageSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);

    if (!imageSurface) {
        fprintf(stderr, "Erro ao criar a superfície da imagem: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            Uint8 r = (Uint8)(matrix[i][j] * 1.0); // Red
            Uint8 g = (Uint8)(matrix[i][j] * 1.0); // Green
            Uint8 b = (Uint8)(matrix[i][j] * 1.0); // Blue

            Uint32 pixelColor = SDL_MapRGB(imageSurface->format, r, g, b);
            SDL_Rect pixelRect = {j, i, 1, 1};
            SDL_FillRect(imageSurface, &pixelRect, pixelColor);
        }
    }

    // Salvar a superfície como imagem BMP
    SDL_SaveBMP(imageSurface, outputFileName);

    // Liberar recursos
    SDL_FreeSurface(imageSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    const char *inputFileName = "matriz.txt";
    const char *outputFileName = "mona_gerada.bmp";

    generateImageFromMatrix(inputFileName, outputFileName);

    return 0;
}

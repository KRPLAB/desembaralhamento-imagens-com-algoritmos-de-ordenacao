/* Este programa abre um arquivo de imagem com extensao
 * .bmp, leh os dados dos pixels interpretando-os como
 * um vetor bidimensional (matriz) e os embaralha em tempo
 * real abrindo uma janela extra. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define HEIGHT 1526
#define WIDTH 1024

void shuffleImage(SDL_Surface *imageSurface)
{
    Uint32 *pixels = (Uint32 *)imageSurface->pixels;
    int imgWidth = imageSurface->w;
    int imgHeight = imageSurface->h;

    for (int i = 0; i < imgHeight; i++)
    {
        for (int j = 0; j < imgWidth; j++)
        {
            int randomX = rand() % imgWidth;
            int randomY = rand() % imgHeight;

            Uint32 temp = pixels[i * imgWidth + j];
            pixels[i * imgWidth + j] = pixels[randomY * imgWidth + randomX];
            pixels[randomY * imgWidth + randomX] = temp;
        }
    }
}

void renderTimer(SDL_Renderer *renderer, TTF_Font *font, Uint32 startTime)
{
    // Obter o tempo atual
    Uint32 currentTime = SDL_GetTicks();

    // Calcular o tempo decorrido em segundos
    float elapsedTimeInSeconds = (currentTime - startTime) / 1000.0f;

    // Renderizar o tempo na janela
    char timeText[30];
    sprintf(timeText, "Tempo: %.4f s", elapsedTimeInSeconds);

    SDL_Color textColor = {255, 255, 0, 255}; // Cor amarela
    SDL_Color outlineColor = {0, 0, 0, 255};  // Cor preta para o contorno

    // Renderizar o texto com contorno preto
    SDL_Surface *outlineSurface = TTF_RenderText_Solid(font, timeText, outlineColor);
    SDL_Texture *outlineTexture = SDL_CreateTextureFromSurface(renderer, outlineSurface);

    SDL_Rect outlineRect = {10 - 2, 10 - 2, outlineSurface->w, outlineSurface->h};
    SDL_RenderCopy(renderer, outlineTexture, NULL, &outlineRect);

    SDL_DestroyTexture(outlineTexture);
    SDL_FreeSurface(outlineSurface);

    // Renderizar o texto real
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, timeText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Shuffling Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(time(NULL));

    // Inicializar a fonte TTF (TrueType Font)
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("/home/kauan/Inter/static/Inter-Regular.ttf", 24); // Use uma fonte adequada

    // Carregar imagem de um arquivo BMP
    SDL_Surface *imageSurface = SDL_LoadBMP("./mona.bmp");

    // Verificar se a imagem foi carregada com sucesso
    if (!imageSurface)
    {
        fprintf(stderr, "Erro ao carregar imagem: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Obter o tempo de início
    Uint32 startTime = SDL_GetTicks();

    // Definir a duração máxima do embaralhamento em segundos
    float maxShuffleDuration = 10.0f;

    // Loop principal
    int quit = 0;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        // Obter o tempo atual
        Uint32 currentTime = SDL_GetTicks();

        // Calcular o tempo decorrido em segundos
        float elapsedTimeInSeconds = (currentTime - startTime) / 1000.0f;

        // Sair do loop se o tempo decorrido ultrapassar a duração máxima
        if (elapsedTimeInSeconds > maxShuffleDuration)
        {
            quit = 1;
        }

        // Embaralhar a imagem
        shuffleImage(imageSurface);

        // Criar textura a partir da imagem
        SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

        // Limpar o renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Renderizar a imagem
        SDL_RenderCopy(renderer, imageTexture, NULL, NULL);

        // Renderizar o timer na janela
        renderTimer(renderer, font, startTime);

        // Atualizar a janela
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(imageTexture);

        SDL_Delay(100); // Ajuste conforme necessário
    }

    // Aguarda um tempo antes de fechar a janela (apenas para visualização)
    SDL_Delay(3000);

    // Liberar recursos da fonte TTF
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_FreeSurface(imageSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

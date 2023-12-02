#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define HEIGHT 250
#define WIDTH 250

struct imagemVetor
{
    int idx;
    int valor;
};

void renderizar(SDL_Renderer *renderer, struct imagemVetor *vetor, TTF_Font *font, Uint32 startTime)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            int index = i * WIDTH + j;
            Uint8 r = (Uint8)(vetor[index].valor * 1.0);
            Uint8 g = (Uint8)(vetor[index].valor * 1.0);
            Uint8 b = (Uint8)(vetor[index].valor * 1.0);

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, j, i);
        }
    }

    /* Renderizar o timer na janela */
    Uint32 currentTime = SDL_GetTicks();
    float elapsedTimeInSeconds = (currentTime - startTime) / 1000.0f;
    char timeText[30];
    sprintf(timeText, "%.4f s", elapsedTimeInSeconds);

    SDL_Color textColor = {255, 255, 0, 255}; /* Cor amarela */

    /* Criar textura a partir do texto */
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, timeText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    /* Definir a posição do texto */
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};

    /* Renderizar o texto */
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    /* Limpar recursos da textura e superfície */
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    SDL_RenderPresent(renderer);
}

void merge(struct imagemVetor *v, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    /* Criação de arrays temporários */
    struct imagemVetor L[n1], R[n2];

    /* Copiar dados para arrays temporários L[] e R[] */
    for (i = 0; i < n1; i++)
        L[i] = v[left + i];
    for (j = 0; j < n2; j++)
        R[j] = v[mid + 1 + j];

    /* Merge dos arrays temporários de volta para v[left..right] */
    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].idx <= R[j].idx)
        {
            v[k] = L[i];
            i++;
        }
        else
        {
            v[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copiar os elementos restantes de L[], se houver algum */
    while (i < n1)
    {
        v[k] = L[i];
        i++;
        k++;
    }

    /* Copiar os elementos restantes de R[], se houver algum */
    while (j < n2)
    {
        v[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(struct imagemVetor *v, int left, int right, SDL_Renderer *renderer, Uint32 startTime, TTF_Font *font)
{
    if (left < right)
    {
        /* Mesmo que (left+right)/2, mas evita overflow para
           valores grandes de left e right */
        int mid = left + (right - left) / 2;

        /* Ordenar as metades esquerda e direita */
        mergeSort(v, left, mid, renderer, startTime, font);
        mergeSort(v, mid + 1, right, renderer, startTime, font);

        /* Mesclar as metades ordenadas */
        merge(v, left, mid, right);

        /* Renderizar após cada etapa de merge */
        renderizar(renderer, v, font, startTime);

        /* Aguardar um curto período para dar tempo ao SDL de exibir a imagem */
        SDL_Delay(0); /* Ajuste conforme necessário */
    }
}

void shuffle(struct imagemVetor *vetor, int tamanho)
{
    for (int i = tamanho - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        struct imagemVetor temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

int main()
{
    struct imagemVetor *vetor, *v2;
    FILE *arquivo;
    int tamanho;

    srand(0);

    tamanho = WIDTH * HEIGHT;

    if (!(vetor = malloc(tamanho * sizeof(struct imagemVetor))))
    {
        fprintf(stderr, "Erro ao alocar memória para vetor.\n");
        return 0;
    }

    if (!(v2 = malloc(tamanho * sizeof(struct imagemVetor))))
    {
        fprintf(stderr, "Erro ao alocar memória para v2.\n");
        return 0;
    }

    // trocar pelo arquivo criado ao rodar o programa ./extrai
    if (!(arquivo = fopen("matriz.txt", "r")))
    {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        free(vetor);
        return 0;
    }

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (fscanf(arquivo, "%d", &(vetor[i * WIDTH + j].valor)) != 1)
            {
                fprintf(stderr, "Erro ao ler valor do arquivo.\n");
                fclose(arquivo);
                free(vetor);
                return 1;
            }
            vetor[i * WIDTH + j].idx = i * WIDTH + j;
        }
    }

    fclose(arquivo);

    memcpy(v2, vetor, tamanho * sizeof(struct imagemVetor));

    shuffle(v2, tamanho);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Image Sorting with Timer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer)
    {
        fprintf(stderr, "Erro ao criar janela ou renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    /* Inicializar a fonte TTF (TrueType Font) */
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("./Nunito-Regular.ttf", 16); /* Substitua pelo caminho da sua fonte */

    if (!font)
    {
        fprintf(stderr, "Erro ao carregar a fonte: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        free(vetor);
        free(v2);
        return 0;
    }

    /* Iniciar o timer */
    Uint32 startTime = SDL_GetTicks();

    /* Ordenar o vetor e renderizar ao final */
    mergeSort(v2, 0, tamanho - 1, renderer, startTime, font);

    /* Aguardar por 5 segundos após a ordenação */
    SDL_Delay(5000);

    /* Renderizar uma última vez antes de fechar a janela */
    renderizar(renderer, v2, font, startTime);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    TTF_CloseFont(font);
    TTF_Quit();

    free(vetor);
    free(v2);

    return 0;
}

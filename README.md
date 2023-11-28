# Desembaralhamento de imagem com algoritmos de ordenação
Este projeto consiste na aplicação dos algoritmos Insertion Sort, Selection Sort, Merge Sort, Quick Sort e Heap Sort para desembaralhar uma imagem embaralhada.

## Processamento da imagem
A extensão de imagem escolhida é .bmp. Para processa-la é utilizado o programa "extrai.c" com a saída redirecionada para um arquivo .txt. O programa utiliza a biblioteca SDLS2 para abrir o arquivo de imagem, percorrer sua superfície e extrair o valor de cada pixel para uma matriz de dimensões iguais às da imagem. 
Para compilar deve-se ter a biblioteca SDLS2 instalada com o comando:
```
gcc -Wall -Werror -Wextra -pedantic extrai.c -o extrai -lSDL2 -I/usr/include/SDL2
```

Algumas observações:
1. Existe mais de um arquivo de imagem, sendo eles: mona100x100.bmp, mona500x500.bmp, original.bmp. Os dois primeiros são variações do arquivo original.bmp com as dimensões alteradas para 100px por 100px e 500px por 500px, respectivamente. Portanto deve-se observar os defines WIDTH e HEIGHT no cabeçalho do programa e também a imagem que está sendo aberta e lida na função main() do programa para ver se estão de acordo;
2. O programa extrai os dados dos pixels referentes às cores para uma matriz em uma escala cinza. Para extrair os dados com a matriz RGB deve-se usar o programa "extrai_rgb". Entretanto, este projeto ainda não está processando os dados desta forma.
3. Reinterando que o programa NÃO cria o arquivo .txt automaticamente. Portanto o comando para rodar deve ser ./extrai > nome_para_arquivo.txt.

## Renderização da imagem embaralhada e início da ordenação
Tendo efetuado a extração com a saída direcionada para um arquivo .txt pode-se compilar o programa main.c com o comando:
```
gcc -Wall -Werror -Wextra -pedantic main.c -o main -lSDL2 -lSDL2_ttf -I/usr/include/SDL2
```

Algumas observações:
1. Importante observar os defines WIDTH e HEIGHT no cabeçalho do programa e também são iguais aos do programa extrai.c e se o arquivo .txt que está sendo aberto e lido é aquele criado após a execução do ./extrai.

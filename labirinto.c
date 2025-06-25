#include "labirinto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
pq usar o malloc? quando fui criar a matriz para o labirinto o código deu erro, depois de pesquisar sobre o erro
vi que, ou eu colocava o segundo valor da matriz [][esse], como um valor pré definido ou eu usava ponteiro e malloc
acabei optando por essa opção pois o professor disse que o código teria que funcionar com um labirinto de qualquer 
tamanho
*/
char** alocar_matriz (uint n, uint m){
    char** labirinto = malloc (n * sizeof(char*)); //Aloca um vetor de ponteiros (char*), onde cada ponteiro representará uma linha da matriz.
    if (!labirinto) 
        return NULL;
    
        for (uint i = 0; i < n; i++) { //Para cada linha i 
            labirinto[i] = malloc(m * sizeof(char)); //aloca um vetor de m caracteres (char), que representa as colunas da matriz.
            if (!labirinto[i]) {
                // desaloca tudo em caso de erro
                for (uint j = 0; j < i; j++) free(labirinto[j]);
                free(labirinto);
                return NULL;
        }
    }
    return labirinto;
}

bool criar_matrizLab(char** labirinto, uint n, uint m, FILE* f) {
    if (!f) return false;

    for (uint i = 0; i < n; i++) {
        for (uint j = 0; j < m; j++) {
            int c = fgetc(f);
            // Remove tratamento extra de newline que pode pular caracteres válidos
            if (c == EOF) {
                return false;
            }
            labirinto[i][j] = (char)c;
        }
        // Pular newline no final de cada linha
        if (fgetc(f) != '\n' && !feof(f)) {
            return false;
        }
    }
    return true;
}

void liberar_matriz(char** matriz, uint n) {
    for (uint i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

bool encontrar_posicoes_SE(Labirinto* lab, Posicao* inicio, Posicao* saida) {
    bool encontrouS = false, encontrouE = false;
    
    for(uint i = 0; i < lab->n; i++) {
        for(uint j = 0; j < lab->m; j++) {
            if(lab->labirinto[i][j] == 'S') {
                inicio->i = i;
                inicio->j = j;
                encontrouS = true;
            }
            if(lab->labirinto[i][j] == 'E') {
                saida->i = i;
                saida->j = j;
                encontrouE = true;
            }
        }
    }
    
    return encontrouS && encontrouE;
}

// Criar uma vez e reutilizar
Labirinto* criar_contexto(char** labirinto, uint n, uint m, int penalidade) {
    Labirinto* lab = malloc(sizeof(Labirinto));
    if (!lab) return NULL;
    
    lab->labirinto = labirinto;
    lab->n = n;
    lab->m = m;
    lab->penalidade = penalidade;
    
    if (!encontrar_posicoes_SE(lab, &lab->inicio, &lab->saida)) {
        free(lab);
        return NULL;
    }
    return lab;
}

int labirinto_print(const Labirinto* lab){ //const indica que o ponteiro lab aponta para dados que não devem ser modificados dentro da função.
    //ou seja, a função é só para leitura
    for (uint i = 0; i < lab->n; i++) {
        for (uint j = 0; j < lab->m; j++) {
            printf("%c", lab->labirinto[i][j]);
        }
        printf("\n");
    }
    printf("=== LABIRINTO CARREGADO ===\n");
    printf("\nDimensoes: %ux%u\n", lab->n, lab->m);
    printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
    printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);

    return 0;
}

char **copiar_matriz(char **original, uint n, uint m) {
  char **copia = malloc(n * sizeof(char *));
  if (!copia) return NULL;
  
  for (uint i = 0; i < n; i++) {
    copia[i] = malloc(m * sizeof(char));
    if (!copia[i]) {
      for (uint j = 0; j < i; j++) free(copia[j]);
      free(copia);
      return NULL;
    }
    for (uint j = 0; j < m; j++) {
      copia[i][j] = original[i][j];
    }
  }
  return copia;
}
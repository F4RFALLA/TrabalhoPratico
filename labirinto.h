#ifndef LABIRINTO_H
#define LABIRINTO_H
#include "config.h"
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned int uint;

typedef struct {
    unsigned int i, j;
} Posicao;

typedef struct _labirinto {
    char** labirinto;
    uint n, m;
    Posicao inicio;
    Posicao saida;
    int penalidade;
} Labirinto;

/**
 * Aloca uma matriz de caracteres com dimensões n x m.
 * Por exemplo, para um labirinto 6x10, aloca 6 linhas de 10 caracteres cada.
 * @param uint número de linhas da matriz
 * @param uint número de colunas da matriz
 * @return char** ponteiro para a matriz alocada ou NULL em caso de falha
 */
char** alocar_matriz(uint, uint);

/**
 * Libera a memória de uma matriz previamente alocada.
 * Por exemplo, libera uma matriz 6x10 alocada por alocar_matriz().
 * @param char** ponteiro para a matriz
 * @param uint número de linhas da matriz
 */
void liberar_matriz(char**, uint);

/**
 * @brief Exibe informações completas sobre o labirinto carregado
 * 
 * @param Labirinto* Ponteiro para a estrutura Labirinto com metadados
 */
int labirinto_print(const Labirinto*);

/**
 * Carrega um labirinto a partir de um arquivo já aberto para a matriz.
 * @param char** Matriz destino (já alocada)
 * @param uint Número de linhas da matriz
 * @param uint Número de colunas da matriz
 * @param FILE Ponteiro para arquivo já aberto no modo leitura ("r")
 * @return bool true se o labirinto foi carregado com sucesso
 */
bool criar_matrizLab(char**, uint, uint, FILE*);

/**
 * Encontra as posições de 'S' (início) e 'E' (saída) no labirinto.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param Posicao* inicio - ponteiro para armazenar a posição de 'S'
 * @param Posicao* saida - ponteiro para armazenar a posição de 'E'
 * @return bool - true se ambas as posições foram encontradas
 */
bool encontrar_posicoes_SE(Labirinto*, Posicao*, Posicao*);

/**
 * Cria e inicializa uma estrutura de contexto do labirinto com todas as informações necessárias.
 * Por exemplo, armazena as dimensões do labirinto, posições de início/saída e penalidade.
 * @param char** labirinto - matriz do labirinto
 * @param uint n - número de linhas do labirinto
 * @param uint m - número de colunas do labirinto
 * @param int penalidade - valor da penalidade por colisão com paredes
 * @return ContextoLabirinto* - ponteiro para a estrutura alocada ou NULL em caso de falha
 */
Labirinto* criar_contexto(char**, uint, uint, int);

#endif
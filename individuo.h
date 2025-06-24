#ifndef INDIVIDUO_H
#define INDIVIDUO_H
#include "labirinto.h"

#include <stdint.h>
#include <stdio.h>

typedef struct _labirinto Labirinto;
typedef struct _individuo Individuo;

/**
 * Gera um movimento aleatório para o caminho.
 * Por exemplo, pode retornar 'C' (Cima), 'B' (Baixo), etc.
 * @return char movimento aleatório ('C', 'B', 'E' ou 'D')
 */
char gerar_movimento_aleatorio();
/**
 * Gera movimento válido 
 * Exemplo: EX: no start tem parede em cima e na esquerda, as opções de movimento são só baixo e direita
 * @return char movimento aleatório 
 */
char movimento_valido_aleatorio(Labirinto* lab, Posicao atual);

/**
 * Simula o movimento de um indivíduo no labirinto e retorna sua posição final.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param Individuo* indiv - Ponteiro para o indivíduo que tem os movimentos
 * @param int* colisoes - ponteiro que calcula a quantidade de colisões para fazer o calculo do fitness
 * @return Posicao - posição final após aplicar os movimentos
 */
Posicao simular_movimentos(const Labirinto*, Individuo*, int*, char**);

/**
 * Calcula o fitness de um indivíduo no labirinto
 * @param Labirinto* Ponteiro para o contexto do labirinto
 * @param Individuo* Ponteiro para o indivíduo a ser avaliado
 */
void calcular_fitness(const Labirinto*, Individuo*, int);


#endif
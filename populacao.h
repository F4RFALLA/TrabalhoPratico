#ifndef POPULACAO_H
#define POPULACAO_H
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "lista.h"
#include "config.h"

/**
 * Cria uma população inicial de indivíduos com caminhos aleatórios.
 * Por exemplo, gera uma população com tamanho baseado na distância de Manhattan.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param uint - tamanho da população dado pelo usuário
 * @param FormaCaminho - tipo de movimento: aleatório ou somente válido
 * @param int - valor da penalidade da distancia
 * @return TLinkedList* - lista encadeada contendo a população ou NULL em caso de erro
 * @see calcular_distancia_manhattan(), TSList_create(), list_create()
 */
TLinkedList* criar_populacao(Labirinto*, uint, FormaCaminho, int);

/**
 * Libera toda a memória alocada para uma população.
 * Por exemplo, libera todos os indivíduos e seus caminhos.
 * @param TLinkedList* ponteiro para a lista de população
 */
void liberar_populacao(TLinkedList*);

/**
 * Imprime toda a população no console, mostrando cada indivíduo com seu caminho e fitness.
 * Por exemplo, imprime "Indivíduo 1: Caminho: [C, B, E], Fitness: 10".
 * @param TLinkedList* ponteiro para a lista de população
 */
void print_populacao(TLinkedList*);

/**
 * Simula e avalia toda a população.
 * Calcula fitness e posições finais para todos os indivíduos.
 * 
 * @param Labirinto* Ponteiro para o contexto do labirinto já inicializado, contendo:
 *- labirinto: matriz do labirinto
 *- n, m: dimensões
 *- inicio: posição inicial (S)
 *- saida: posição destino (E)
 *- penalidade: valor da penalidade por colisão
 * @param TLinkedList Lista encadeada contendo a população a ser avaliada
 */
void simular_populacao(const Labirinto*, TLinkedList*);


#endif
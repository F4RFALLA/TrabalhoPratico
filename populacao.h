#ifndef POPULACAO_H
#define POPULACAO_H
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "lista.h"
#include "config.h"

/**
 * Cria uma população inicial de indivíduos com caminhos aleatórios.
 * @param Labirinto* ponteiro para o labirinto
 * @param uint tamanho da população
 * @param FormaCaminho tipo de geração de caminho
 * @param int peso da distância
 * @return TLinkedList* lista da população
 */
TLinkedList* criar_populacao(Labirinto*, uint, FormaCaminho, int);

/**
 * Libera toda a memória alocada para uma população.
 * @param TLinkedList* ponteiro para a lista de população
 */
void liberar_populacao(TLinkedList*);

/**
 * Imprime toda a população no console.
 * @param TLinkedList* ponteiro para a lista de população
 */
void print_populacao(TLinkedList*);

/**
 * Simula e avalia toda a população.
 * @param const Labirinto* ponteiro para o labirinto
 * @param TLinkedList* lista da população
 */
void simular_populacao(const Labirinto*, TLinkedList*);

/**
 * Calcula o somatório de fitness da população.
 * @param TLinkedList* ponteiro para a lista de população
 * @return int soma total dos fitness
 */
int somar_fitness(TLinkedList*);

/**
 * Sorteia um valor para roleta viciada.
 * @param TLinkedList* ponteiro para a lista de população
 * @return int valor sorteado
 */
int sortear_valor_roleta(TLinkedList*);

/**
 * Seleciona um pai usando roleta viciada.
 * @param TLinkedList* ponteiro para a lista de população
 * @return int índice do pai selecionado
 */
int selecionar_pai(TLinkedList*);

/**
 * Seleciona os melhores indivíduos (elitismo).
 * @param TLinkedList* ponteiro para a população
 * @param float taxa de elitismo
 * @return TLinkedList* lista com elite
 */
TLinkedList* elitismo(TLinkedList*, float);

/**
 * Clona um indivíduo.
 * @param const Individuo* indivíduo a ser clonado
 * @return Individuo cópia do indivíduo
 */
Individuo clonar_individuo(const Individuo*);

/**
 * Realiza crossover entre duas populações.
 * @param TLinkedList* população elite
 * @param TLinkedList* população total
 * @param Labirinto* ponteiro para labirinto
 * @param int peso da distância
 * @return TLinkedList* nova geração
 */
TLinkedList* crossover(TLinkedList*, TLinkedList*, Labirinto*, int);

/**
 * Verifica condição de parada (chegada na saída).
 * @param const Labirinto* ponteiro para labirinto
 * @param TLinkedList* lista da população
 * @return int 1 se condição satisfeita, 0 caso contrário
 */
int condicao_parada(const Labirinto*, TLinkedList*);

/**
 * Imprime detalhes de um indivíduo.
 * @param Labirinto* ponteiro para labirinto
 * @param Individuo* ponteiro para indivíduo
 * @param int geração atual
 * @param int peso da distância
 */
void imprimir_individuo(Labirinto*, Individuo*, int, int);

#endif
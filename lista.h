#ifndef LISTA_H
#define LISTA_H
#include "individuo.h"
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned int uint;

typedef struct _stack{
    char* data;
    unsigned int capacidade;
    unsigned int qty;//Quantidade de elementos na pilha
    //topo = elemento no indice qty-1
} Stack;

struct _individuo {
    Stack* caminho;      // Lista de movimentos ('C', 'B', 'E', 'D')
    int fitness;          // Valor que indica a qualidade da solução
    int tamanho_caminho;  // Número de movimentos no caminho
};

typedef struct _no {
    Individuo info;
    struct _no *prox;
} TNo;

typedef struct _list {
    TNo* inicio;
} TLinkedList;

/**
 * Cria uma nova lista encadeada vazia.
 * Por exemplo, cria uma lista com ponteiro inicio apontando para NULL.
 * @return TLinkedList* ponteiro para a lista alocada ou NULL em caso de falha
 */
TLinkedList* list_create();

/**
 * Insere um elemento no início da lista.
 * Por exemplo, inserindo 1, 2, 3 resulta em [3] -> [2] -> [1].
 * @param TLinkedList* ponteiro para a lista
 * @param Individuo informação a ser armazenada
 * @return bool true se a inserção foi bem-sucedida
 */
bool list_insert_begin(TLinkedList*, Individuo);

/**
 * Insere um elemento no final da lista.
 * Por exemplo, inserindo 1, 2, 3 resulta em [1] -> [2] -> [3].
 * @param TLinkedList* ponteiro para a lista
 * @param Individuo informação a ser armazenada
 * @return bool true se a inserção foi bem-sucedida
 */
bool list_insert_end(TLinkedList*, Individuo);

/**
 * Insere um elemento em ordem decrescente na lista.
 * Por exemplo, inserindo 1, 2, 3 resulta em [3] -> [2] -> [1].
 * @param TLinkedList* ponteiro para a lista
 * @param Individuo informação a ser armazenada
 * @return bool true se a inserção foi bem-sucedida
 */
bool list_insert_sorted(TLinkedList*, Individuo);

/**
 * Imprime todos os elementos da lista no console.
 * Por exemplo, imprime [1] -> [2] -> [3] para a lista acima.
 * @param TLinkedList* ponteiro para a lista
 */
void list_print(TLinkedList*);

/**
 * Remove o primeiro elemento da lista.
 * Por exemplo, remove [1] de [1] -> [2] -> [3].
 * @param TLinkedList* ponteiro para a lista
 * @return bool true se a remoção foi bem-sucedida
 */
bool list_delete_begin(TLinkedList*);

/**
 * Remove os primeiros n elementos da lista.
 * Por exemplo, remover 2 elementos de [1,2,3] resulta em [3].
 * @param TLinkedList* ponteiro para a lista
 * @param unsigned int número de elementos a remover
 * @return bool true se a remoção foi bem-sucedida
 */
bool list_delete_n(TLinkedList*, unsigned int);

/**
 * Cria instâncias de pilhas. 
 * Retorna NULL caso não seja possível alocar memória para uma nova pilha.
 * @return Stack* ponteiro para uma pilha possivelmente alocada. 
 */
Stack* Stack_create();
/**
 * Tenta inserir um dado na pilha.
 * @param Stack* ponteiro para a pilha
 * @param char valor para ser inserido na pilha
 * @return bool informando o status da operação.
 */
bool Stack_push(Stack*, char);
/**
 * Tenta remover o elemento do topo da pilha.
 * @param Stack* ponteiro para a pilha
 * @param int* ponteiro para um inteiro para armazenar 
 * o valor removido da pilha.
 * @return bool informando o status da operação.
 */
bool Stack_pop(Stack*, char*);
/**
 * Retorna se a pilha está cheia.
 * @param Stack*
 * @return bool informando se está cheia.
 */
bool Stack_is_full(Stack*);
/**
 * Retorna se a pilha está vazia.
 * @param Stack*
 * @return bool informando se está vazia.
 */
bool Stack_is_empty(Stack*);
/**
 * Retorna a quantidade de elementos da pilha.
 * @param Stack*
 * @return unsigned int número de elementos da pilha.
 */
unsigned int Stack_size(Stack*);

/**
 * Da print na pilha
 * @param Stack* pilha a ser printada
 */

void Stack_print(Stack*);

#endif
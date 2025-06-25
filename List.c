#include "individuo.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

TNo* TNo_createNFill(Individuo);

TLinkedList* list_create() {
    TLinkedList* nova = malloc(sizeof(TLinkedList));
    if(nova) nova->inicio = NULL;
    return nova;
}

// List.c
bool list_insert_sorted(TLinkedList* lista, Individuo info) {
    TNo* novo = TNo_createNFill(info);
    if (!novo) return false;
    
    // Caso especial: lista vazia ou novo fitness maior que o primeiro
    if (lista->inicio == NULL || novo->info.fitness > lista->inicio->info.fitness) {
        novo->prox = lista->inicio;
        lista->inicio = novo;
        return true;
    }

    // Percorrer lista para encontrar posição correta
    TNo* atual = lista->inicio;
    while (atual->prox != NULL && atual->prox->info.fitness >= novo->info.fitness) {
        atual = atual->prox;
    }
    
    novo->prox = atual->prox;
    atual->prox = novo;
    return true;
}

bool list_insert_begin(TLinkedList* lista, Individuo info){
    TNo* novo = TNo_createNFill(info);
    if(novo == NULL) return false;
    if(lista->inicio != NULL)
        novo->prox = lista->inicio;
    lista->inicio = novo;
    return true;
}

bool list_insert_end(TLinkedList* lista, Individuo info) {
    TNo* novo = malloc(sizeof(TNo));
    if(!novo) return false;
    
    novo->info = info;
    novo->prox = NULL;

    if(lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        TNo* aux = lista->inicio;
        while(aux->prox != NULL) aux = aux->prox;
        aux->prox = novo;
    }
    return true;
}

void list_destroy(TLinkedList* lista, int liberar_dados) {
    if (!lista) return;
    
    TNo* atual = lista->inicio;
    while (atual) {
        TNo* proximo = atual->prox;
        if (liberar_dados && atual->info.caminho) {
            free(atual->info.caminho->data);
            free(atual->info.caminho);
        }
        free(atual);
        atual = proximo;
    }
    free(lista);
}

void list_print(TLinkedList* lista){
    if(!lista) return;
    
    TNo* aux = lista->inicio;
    while(aux != NULL){
        printf("[Fitness: %d]->", aux->info.fitness);
        aux = aux->prox;
    }
    putchar('\n');
}

TNo* TNo_createNFill(Individuo info){
    TNo* novo = malloc(sizeof(TNo));
    if(novo){
        novo->info = info;
        novo->prox = NULL;
    }
    return novo;
}

TLinkedList* list_concatenate(TLinkedList* l1, TLinkedList* l2){
    TLinkedList* l_result = list_create();
    if(l_result){
        TNo* aux = l1->inicio;
        while(aux!=NULL){
            list_insert_end(l_result, aux->info);
            aux = aux->prox;
        }
        aux = l2->inicio;
        while(aux!=NULL){
            list_insert_end(l_result, aux->info);
            aux = aux->prox;
        }
    }
    return l_result;
}

bool list_delete_begin(TLinkedList* lista){
    if(lista->inicio == NULL)
        return false;
    TNo* aux = lista->inicio;
    lista->inicio = lista->inicio->prox;
    free(aux);
    return true;
}

int list_size(TLinkedList* lista) {
    if (!lista) return 0;
    int count = 0;
    TNo* aux = lista->inicio;
    while (aux) {
        count++;
        aux = aux->prox;
    }
    return count;
}

// Implementação correta da pilha
Stack* Stack_create(unsigned int capacidade) {
    Stack* nova = malloc(sizeof(Stack));
    if(!nova) return NULL;
    
    nova->data = malloc(capacidade * sizeof(char));
    if(!nova->data) {
        free(nova);
        return NULL;
    }
    
    nova->qty = 0;
    nova->capacidade = capacidade;
    return nova;
}

bool Stack_push(Stack* pilha, char valor) {
    if(!pilha || pilha->qty >= pilha->capacidade) return false;
    pilha->data[pilha->qty++] = valor;
    return true;
}

bool Stack_pop(Stack* pilha, char* info) {
    if(!pilha || Stack_is_empty(pilha)) return false;
    *info = pilha->data[--pilha->qty];
    return true;
}

bool Stack_is_empty(Stack* pilha) {
    return pilha ? pilha->qty == 0 : true;
}

bool Stack_is_full(Stack* pilha) {
    return pilha ? pilha->qty == pilha->capacidade : false;
}

unsigned int Stack_size(Stack* pilha) {
    return pilha ? pilha->qty : 0;
}

void Stack_print(Stack* pilha) {
    if(!pilha || pilha->qty == 0) {
        printf("[]");
        return;
    }
    
    printf("[");
    for(int i = 0; i < pilha->qty; i++) {
        printf("%c", pilha->data[i]);
        if(i < pilha->qty - 1) printf(", ");
    }
    printf("]");
}
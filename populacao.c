// populacao.c
#include "populacao.h"
#include "individuo.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Função auxiliar para copiar matriz
char** copiar_matriz(char** original, uint n, uint m) {
    char** copia = malloc(n * sizeof(char*));
    for(uint i = 0; i < n; i++) {
        copia[i] = malloc(m * sizeof(char));
        for(uint j = 0; j < m; j++) {
            copia[i][j] = original[i][j];
        }
    }
    return copia;
}
//separar aqui a geração de movimento aleatório e a de movimentos válidos, o de movimento aleatório, so o primeiro movimente será somente para a direira ou para baixo
//que é o caso desse labirinto, a o movimentos válidos não terá penalidade por colisão pois ele considera o movimento que vai bater na parede
Posicao simular_movimentos(const Labirinto* lab, Individuo* indiv, int* colisoes, char** lab_copia) {
    if(!lab || !indiv || !indiv->caminho) return lab->inicio;
    
    Posicao atual = lab->inicio;
    int colisao = 0;

    // Se lab_copia foi fornecido, marcar posição inicial
    if(lab_copia && lab_copia[atual.i][atual.j] != 'S') {
        lab_copia[atual.i][atual.j] = '@';
    }

    for(uint i = 0; i < indiv->caminho->qty; i++) {
        char mov = indiv->caminho->data[i];
        Posicao proxima = atual;
        
        switch(mov) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue;
        }
        
        // Verificar movimento válido
        if(proxima.i < lab->n && proxima.j < lab->m && proxima.i >= 0 && proxima.j >= 0 && lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
            // Se lab_copia foi fornecido, marcar posição
            if(lab_copia && lab_copia[atual.i][atual.j] != 'S' && lab_copia[atual.i][atual.j] != 'E') {
                lab_copia[atual.i][atual.j] = '@';
            }
        } else {
            colisao++;
        }
    }
    
    if(colisoes) *colisoes = colisao;
    return atual;
}

char gerar_movimento_aleatorio() {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    return movimentos[rand() % 4];
}

char movimento_valido(Labirinto* lab, Posicao atual) {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    char movimentos_validos[4];
    int num_validos = 0;
    
    for(int i = 0; i < 4; i++) {
        Posicao prox = atual;
        switch(movimentos[i]) {
            case 'C': prox.i--; break;
            case 'B': prox.i++; break;
            case 'E': prox.j--; break;
            case 'D': prox.j++; break;
        }
        
        if(prox.i < lab->n && prox.j < lab->m && prox.i >= 0 && prox.j >= 0 && lab->labirinto[prox.i][prox.j] != '#') {
            movimentos_validos[num_validos++] = movimentos[i];
        }
    }
    //caso não tiver opções de caminhos válidos retorna um caminho aleatório:
    return (num_validos > 0) ? 
        movimentos_validos[rand() % num_validos] : 
        gerar_movimento_aleatorio();
}

int calcular_distancia_manhattan(Posicao a, Posicao b) {
    return abs((int)a.i - (int)b.i) + abs((int)a.j - (int)b.j);
}

void calcular_fitness(const Labirinto* lab, Individuo* indiv, int w_distancia) {
    if(!lab || !indiv || !indiv->caminho) {
        if(indiv) indiv->fitness = 0;
        return;
    }
    
    int colisoes = 0;
    // Chamada única para simular movimentos
    Posicao final = simular_movimentos(lab, indiv, &colisoes, NULL);
    
    int distancia = calcular_distancia_manhattan(final, lab->saida);
    int fitness = w_distancia - distancia - (colisoes * lab->penalidade);
    indiv->fitness = (fitness > 0) ? fitness : 0;
}

TLinkedList* criar_populacao(Labirinto* lab, uint tamanho_populacao, FormaCaminho tipo, int distancia) {
    if(!lab) return NULL;
    int dist = calcular_distancia_manhattan(lab->inicio, lab->saida);
    TLinkedList* populacao = list_create();
    if(!populacao) return NULL;

    for(int i = 0; i < tamanho_populacao; i++) {
        Individuo ind;
        ind.tamanho_caminho = dist + (rand() % (dist + 1));
        ind.caminho = Stack_create(ind.tamanho_caminho);
        ind.fitness = 0;

        if(!ind.caminho) {
            free(populacao);
            return NULL;
        }

        // Gera primeiro movimento válido
        char mov_inicial = movimento_valido(lab, lab->inicio);
        Stack_push(ind.caminho, mov_inicial);
        
        
        // Gera movimentos subsequentes (podem ser aleatórios, inclusive inválidos)
        if (tipo == ALEATORIO){
            for(int j = 1; j < ind.tamanho_caminho; j++) {
            char mov = gerar_movimento_aleatorio();
            Stack_push(ind.caminho, mov);
            }

            calcular_fitness(lab, &ind, distancia);

            if(!list_insert_end(populacao, ind)) {
                free(ind.caminho);
                free(populacao);
                return NULL;
            }
        }

        if (tipo == MOV_VALIDOS){
            for(int j = 1; j < ind.tamanho_caminho; j++) {
            char mov = movimento_valido(lab, lab->inicio);
            Stack_push(ind.caminho, mov);
            }

            calcular_fitness(lab, &ind, distancia);

            if(!list_insert_end(populacao, ind)) {
                free(ind.caminho);
                free(populacao);
                return NULL;
            }
        }
        
        
    }
    return populacao;
}

void simular_populacao(const Labirinto* lab, TLinkedList* populacao) {
    if(!lab || !populacao) {
        printf("Erro: Labirinto ou populacao invalidos\n");
        return;
    }

    printf("\n=== Simulacao da Populacao (com Fitness) ===\n");
    printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
    printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);
    printf("Penalidade por colisao: %d\n\n", lab->penalidade);

    TNo* atual = populacao->inicio;
    int contador = 1;
    
    while(atual != NULL) {
        int colisoes = 0;
        
        // Cria cópia do labirinto para marcar caminho
        char** lab_copia = copiar_matriz(lab->labirinto, lab->n, lab->m);
        Posicao final = simular_movimentos(lab, &atual->info, &colisoes, lab_copia);
        
        printf("Individuo %03d\n", contador++);
        printf("Posicao final: (%u, %u)\n", final.i, final.j);
        printf("Fitness: %d\n", atual->info.fitness);
        printf("Status: ");
        
        if(final.i == lab->saida.i && final.j == lab->saida.j) {
            printf("Sucesso (atingiu o destino)\n");
        } else {
            printf("Falha (distancia: %d)\n", calcular_distancia_manhattan(final, lab->saida));
        }
        
        // Exibe labirinto com caminho marcado
        printf("Labirinto com caminho percorrido:\n");
        for(uint i = 0; i < lab->n; i++) {
            for(uint j = 0; j < lab->m; j++) {
                printf("%c", lab_copia[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        
        // Libera cópia do labirinto
        for(uint i = 0; i < lab->n; i++) free(lab_copia[i]);
        free(lab_copia);
        
        atual = atual->prox;
    }
}

void liberar_populacao(TLinkedList* populacao) {
    free(populacao);
}

void print_populacao(TLinkedList* populacao) {
    if(!populacao) {
        printf("População invalida!\n");
        return;
    }

    TNo* atual = populacao->inicio;
    int contador = 1;
    
    while(atual != NULL) {
        printf("Individuo %d:\n", contador++);
        printf("  Fitness: %d\n", atual->info.fitness);
        printf("  Caminho: ");
        Stack_print(atual->info.caminho);
        printf("\n\n");
        
        atual = atual->prox;
    }
}


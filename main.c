// main.c
#include "individuo.h"
#include "config.h"
#include "populacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

    // 1. Carregamento do labirinto
    FILE* file = fopen("labirinto.txt", "r");
    if (!file) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo do labirinto\n");
        return 1;
    }
    
    // Leitura das dimensões
    uint n, m;
    if (fscanf(file, "%u %u\n", &n, &m) != 2) {
        fprintf(stderr, "Erro: Formato invalido do arquivo\n");
        fclose(file);
        return 1;
    }

    // Alocação e leitura da matriz
    char** labirinto = alocar_matriz(n, m);
    if (!labirinto || !criar_matrizLab(labirinto, n, m, file)) {
        fprintf(stderr, "Erro ao carregar labirinto do arquivo\n");
        if (labirinto) liberar_matriz(labirinto, n);
        fclose(file);
        return 1;
    }
    fclose(file);

    // 2. Carregar configurações
    Config config = ler_config("config.yaml");

    // 3. Criação do contexto do labirinto
    Labirinto* lab = criar_contexto(labirinto, n, m, config.penalidade);
    if (!lab) {
        fprintf(stderr, "Erro: Falha ao criar contexto do labirinto\n");
        liberar_matriz(labirinto, n);
        return 1;
    }

    // 4. Exibição do labirinto
    labirinto_print(lab);

    // 5. Criação da população inicial
    TLinkedList* populacao = criar_populacao(lab, config.tamanho_populacao, config.forma_caminho, config.w_distancia);
    if (!populacao) {
        fprintf(stderr, "Erro: Falha ao criar populacao inicial\n");
        liberar_matriz(labirinto, n);
        free(lab);
        return 1;
    }

    printf("\n=== Populacao Inicial ===\n");
    print_populacao(populacao);

    // 6. Loop principal do algoritmo genético
    int geracao = 0;
    int encontrou_solucao = 0;

    printf("\n=== Iniciando Algoritmo Genetico ===\n");
    printf("Tamanho da Populacao: %d\n", config.tamanho_populacao);
    printf("Probabilidade de Crossover: %.2f\n", config.prob_cx);
    printf("Probabilidade de Mutacao: %.2f\n", config.prob_mut);
    printf("Taxa de Elitismo: %.2f\n", config.elitismo);
    printf("Maximo de Geracoes: %d\n\n", config.max_gen);

    while (geracao < config.max_gen && !encontrou_solucao) {
        printf("\n=== Geracao %d ===\n", geracao + 1);
        
        // 6.1 Selecionar elite
        TLinkedList* elite = elitismo(populacao, config.elitismo);
        if (!elite) {
            fprintf(stderr, "Falha ao aplicar elitismo na geracao %d\n", geracao);
            break;
        }
        
        // 6.2 Aplicar crossover
        TLinkedList* nova_geracao = crossover(elite, populacao, lab, config.w_distancia);
        if (!nova_geracao) {
            fprintf(stderr, "Falha no crossover na geracao %d\n", geracao);
            list_destroy(elite, 1);
            break;
        }
        
        // 6.4 Avaliar nova geração
        for (TNo* atual = nova_geracao->inicio; atual; atual = atual->prox) {
            calcular_fitness(lab, &atual->info, config.w_distancia);
        }

        // 6.5 Exibir melhor indivíduo
        if (nova_geracao->inicio) {
            Individuo* melhor = &nova_geracao->inicio->info;
            printf("\nMelhor individuo da geracao %d:\n", geracao + 1);
            printf("Fitness: %d\n", melhor->fitness);
            printf("Tamanho do caminho: %d\n", melhor->tamanho_caminho);
        }

        // 6.6 Liberar memória das estruturas antigas
        list_destroy(populacao, 1);
        list_destroy(elite, 1);
        
        // 6.7 Atualizar população
        populacao = nova_geracao;
        
        // 6.8 Verificar condição de parada
        encontrou_solucao = condicao_parada(lab, populacao);
        
        // 6.9 Exibir estatísticas
        printf("Melhor fitness: %d\n", populacao->inicio->info.fitness);
        printf("Media de fitness: %.2f\n", (float)somar_fitness(populacao) / config.tamanho_populacao);
        
        geracao++;
    }

    // 7. Exibir resultado final
    if (encontrou_solucao) {
        printf("\n=== SOLUCAO ENCONTRADA NA GERACAO %d! ===\n", geracao);
        
        // Exibir o melhor indivíduo
        if (populacao->inicio) {
            Individuo* solucao = &populacao->inicio->info;
            printf("\nMelhor solucao encontrada:\n");
            printf("Fitness: %d\n", solucao->fitness);
            printf("Tamanho do caminho: %d\n", solucao->tamanho_caminho);
            
            // Mostrar caminho percorrido
            char** lab_copia = copiar_matriz(lab->labirinto, lab->n, lab->m);
            int colisoes = 0;
            simular_movimentos(lab, solucao, &colisoes, lab_copia);
            
            printf("\nLabirinto com solucao:\n");
            for (uint i = 0; i < lab->n; i++) {
                for (uint j = 0; j < lab->m; j++) {
                    printf("%c", lab_copia[i][j]);
                }
                printf("\n");
            }
            
            // Liberar cópia do labirinto
            for (uint i = 0; i < lab->n; i++) free(lab_copia[i]);
            free(lab_copia);
        }
    } else {
        printf("\nNao foi encontrada solucao apos %d geracoes\n", geracao);
    }

    // 8. Liberação de recursos
    list_destroy(populacao, 1);
    liberar_matriz(labirinto, n);
    free(lab);

    return 0;
}
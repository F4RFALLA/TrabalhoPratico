#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Config ler_config(const char* filename) {
    Config cfg;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE); // Termina o programa em caso de erro
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '#' || line[0] == '\n') continue;

        int int_value;
        float float_value;
         char str_value[20];  // Buffer para valores de string

         /*
         sobre buffer: Extrair a palavra aleatorio -> Convertê-la para o valor da enumeração ALEATORIO
         O buffer nos permite armazenar essa string temporariamente para depois compará-la.
         */
        
        if (sscanf(line, "tamanho_populacao: %d", &int_value) == 1) {
            cfg.tamanho_populacao = int_value;
        } else if (sscanf(line, "prob_cx: %f", &float_value) == 1) {
            cfg.prob_cx = float_value;
        } else if (sscanf(line, "elitismo: %f", &float_value) == 1) {
            cfg.elitismo = float_value;
        } else if (sscanf(line, "w_distancia: %d", &int_value) == 1) {
            cfg.w_distancia = int_value;
        } else if (sscanf(line, "penalidade: %d", &int_value) == 1) {
            cfg.penalidade = int_value;
        } else if (sscanf(line, "max_gen: %d", &int_value) == 1) {
            cfg.max_gen = int_value;
        } else if (sscanf(line, "forma_caminho: %19s", str_value) == 1) {
            if (strcmp(str_value, "mov_validos") == 0) {
                cfg.forma_caminho = MOV_VALIDOS;
            } else if (strcmp(str_value, "aleatorio") == 0) {
                cfg.forma_caminho = ALEATORIO;
            } else {
                // Valor padrão caso seja desconhecido
                cfg.forma_caminho = MOV_VALIDOS;
            }
        }
    }

    fclose(file);
    return cfg;
}
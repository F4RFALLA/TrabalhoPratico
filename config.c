// config.c
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
        
        //A função sscanf() lê dados formatados de uma string. Ela retorna o número de itens que foram atribuídos com sucesso.
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
        } else if (strstr(line, "forma_caminho:")) {
            if (strstr(line, "mov_validos")) {
                cfg.forma_caminho = MOV_VALIDOS;
            } else if (strstr(line, "aleatorio")) {
                cfg.forma_caminho = ALEATORIO;
            }
        }
    }

    fclose(file);
    return cfg;
}
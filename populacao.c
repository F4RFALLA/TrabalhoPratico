// populacao.c
#include "populacao.h"
#include "config.h"
#include "individuo.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Posicao simular_movimentos(const Labirinto *lab, Individuo *indiv, int *colisoes, char **lab_copia) {
  if (!lab || !indiv || !indiv->caminho)
    return lab->inicio;

  Posicao atual = lab->inicio;
  int colisao = 0;

  // Marcar posição inicial se cópia fornecida
  if (lab_copia && lab_copia[atual.i][atual.j] != 'S') {
    lab_copia[atual.i][atual.j] = '@';
  }

  for (uint i = 0; i < indiv->caminho->qty; i++) {
    char mov = indiv->caminho->data[i];
    Posicao prox = atual;

    switch(mov) {
      case 'C': prox.i--; break;
      case 'B': prox.i++; break;
      case 'E': prox.j--; break;
      case 'D': prox.j++; break;
    }

    // Verificar movimento válido
    if (prox.i < lab->n && prox.j < lab->m && lab->labirinto[prox.i][prox.j] != '#') {
      atual = prox;
      if (atual.i == lab->saida.i && atual.j == lab->saida.j) {
          break; // Sai do loop se chegou na saída
      }
      // Marcar posição se cópia fornecida
      if (lab_copia && lab_copia[atual.i][atual.j] != 'S' &&lab_copia[atual.i][atual.j] != 'E') {
        lab_copia[atual.i][atual.j] = '@';
      }
    } else {
      colisao++;
    }
  }

  if (colisoes) *colisoes = colisao;
  return atual;
}

char gerar_movimento_aleatorio() {
  char movimentos[4] = {'C', 'B', 'E', 'D'};
  return movimentos[rand() % 4];
}

char obter_movimento_valido(Labirinto* lab, Posicao atual) {
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
        
    // Verifica se o movimento é válido
    if(prox.i >= 0 && prox.i < lab->n && 
      prox.j >= 0 && prox.j < lab->m && 
      lab->labirinto[prox.i][prox.j] != '#') {
      movimentos_validos[num_validos++] = movimentos[i];
    }
  }

  return (num_validos > 0) ? movimentos_validos[rand() % num_validos] : gerar_movimento_aleatorio();
}

int calcular_distancia_manhattan(Posicao a, Posicao b) {
  return abs((int)a.i - (int)b.i) + abs((int)a.j - (int)b.j);
}

void calcular_fitness(const Labirinto *lab, Individuo *indiv, int w_distancia) {
  if (!lab || !indiv || !indiv->caminho) {
    if (indiv) indiv->fitness = 0;
    return;
  }

  int colisoes = 0;
  Posicao final = simular_movimentos(lab, indiv, &colisoes, NULL);
  int distancia = calcular_distancia_manhattan(final, lab->saida);
  
  int fitness = w_distancia - distancia - (colisoes * lab->penalidade);
  indiv->fitness = (fitness > 0) ? fitness : 0;
}

// Função unificada para criação de indivíduo
// Corrigir a função criar_individuo
Individuo criar_individuo(Labirinto *lab, FormaCaminho tipo, int distancia) {
  Individuo ind;
    
  // Define o tamanho do caminho baseado na distância Manhattan
  int dist = calcular_distancia_manhattan(lab->inicio, lab->saida);
  ind.tamanho_caminho = dist + (rand() % (dist + 1));
  ind.caminho = Stack_create(ind.tamanho_caminho);
  ind.fitness = 0;

  /*
  if (!ind.caminho) return ind;

    if (tipo == MOV_VALIDOS) {
      printf("DEBUG: Individuo sendo criado com MOVIMENTOS VALIDOS.\n");
  } else if (tipo == ALEATORIO) {
      printf("DEBUG: Individuo sendo criado com MOVIMENTOS ALEATORIOS PUROS.\n");
  }
  */

  if (tipo == MOV_VALIDOS) {
    // Modo MOV_VALIDOS (mantido)
    Posicao atual = lab->inicio;
    for (int j = 0; j < ind.tamanho_caminho; j++) {
      char mov = obter_movimento_valido(lab, atual);
      Stack_push(ind.caminho, mov);
            
      switch(mov) {
        case 'C': if(atual.i > 0) atual.i--; break;
        case 'B': if(atual.i < lab->n-1) atual.i++; break;
        case 'E': if(atual.j > 0) atual.j--; break;
        case 'D': if(atual.j < lab->m-1) atual.j++; break;
      }
    }
  } else {// Modo ALEATORIO: gera movimentos aleatórios
      for (int j = 0; j < ind.tamanho_caminho; j++) {
        Stack_push(ind.caminho, gerar_movimento_aleatorio());
      }
    }
    
    // CALCULAR FITNESS PARA AMBOS OS TIPOS
    calcular_fitness(lab, &ind, distancia);
    return ind;
}

TLinkedList *criar_populacao(Labirinto *lab, uint tamanho_populacao, FormaCaminho tipo, int distancia) {
  if (!lab) return NULL;

  TLinkedList *populacao = list_create();
  if (!populacao) return NULL;

  for (int i = 0; i < tamanho_populacao; i++) {
    Individuo ind = criar_individuo(lab, tipo, distancia);
    
    if (!ind.caminho || !list_insert_sorted(populacao, ind)) {
      if (ind.caminho) free(ind.caminho);
      list_destroy(populacao, 1);
      return NULL;
    }
  }
  return populacao;
}

void simular_populacao(const Labirinto *lab, TLinkedList *populacao) {
  if (!lab || !populacao) return;

  printf("\n=== Simulacao da Populacao (com Fitness) ===\n");
  printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
  printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);
  printf("Penalidade por colisao: %d\n\n", lab->penalidade);

  TNo *atual = populacao->inicio;
  for (int contador = 1; atual != NULL; contador++, atual = atual->prox) {
    imprimir_individuo(lab, &atual->info, contador);
  }
}

int somar_fitness(TLinkedList *populacao) {
  if (!populacao || !populacao->inicio) return 0;

  int soma = 0;
  for (TNo *atual = populacao->inicio; atual != NULL; atual = atual->prox) {
    soma += atual->info.fitness;
  }
  return soma;
}

int sortear_valor_roleta(TLinkedList* populacao) {
  int soma_total = somar_fitness(populacao);
  return (soma_total > 0) ? (rand() % soma_total) + 1 : 1;
}

int selecionar_pai(TLinkedList* populacao) {
  int valor_sorteado = sortear_valor_roleta(populacao);
  int acumulado = 0;
  int indice = 0;

  for (TNo* atual = populacao->inicio; atual != NULL; atual = atual->prox, indice++) {
    acumulado += atual->info.fitness;
    if (acumulado >= valor_sorteado) return indice;
  }
  return 0;
}

Individuo clonar_individuo(const Individuo* original) {
  Individuo clone;
  clone.fitness = original->fitness;
  clone.tamanho_caminho = original->tamanho_caminho;
  clone.caminho = Stack_create(original->caminho->capacidade);
  
  for (int i = 0; i < original->caminho->qty; i++) {
    Stack_push(clone.caminho, original->caminho->data[i]);
  }
  return clone;
}

TLinkedList* elitismo(TLinkedList* populacao, float taxa_elitismo) {
  if (!populacao || taxa_elitismo <= 0.0f) return NULL;

  // Calcular tamanho da elite
  int tamanho_pop = 0;
  for (TNo* temp = populacao->inicio; temp != NULL; temp = temp->prox, tamanho_pop++);
  
  int n_elite = (int)(tamanho_pop * taxa_elitismo);
  if (n_elite < 1) n_elite = 1;

  TLinkedList* elite = list_create();
  if (!elite) return NULL;

  TNo* atual = populacao->inicio;
  for (int i = 0; i < n_elite && atual != NULL; i++, atual = atual->prox) {
    Individuo clone = clonar_individuo(&atual->info);
    list_insert_sorted(elite, clone);
  }
  return elite;
}

static void crossover_inds(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2) {
  const int size1 = pai1->tamanho_caminho;
  const int size2 = pai2->tamanho_caminho;
  const int corte1 = size1 / 2;
  const int corte2 = size2 / 2;
  const int max_size = (size1 > size2) ? size1 : size2;

  filho1->caminho = Stack_create(max_size);
  filho2->caminho = Stack_create(max_size);
  filho1->tamanho_caminho = filho2->tamanho_caminho = max_size;
  filho1->fitness = filho2->fitness = 0;

  for (int i = 0; i < max_size; i++) {
    char gene1 = (i < corte1) 
      ? ((i < size1) ? pai1->caminho->data[i] : gerar_movimento_aleatorio())
      : ((corte2 + (i - corte1) < size2) ? pai2->caminho->data[corte2 + (i - corte1)] : gerar_movimento_aleatorio());
    
    char gene2 = (i < corte2)
      ? ((i < size2) ? pai2->caminho->data[i] : gerar_movimento_aleatorio())
      : ((corte1 + (i - corte2) < size1) ? pai1->caminho->data[corte1 + (i - corte2)] : gerar_movimento_aleatorio());
    
    Stack_push(filho1->caminho, gene1);
    Stack_push(filho2->caminho, gene2);
  }
}

TLinkedList* crossover(TLinkedList* elite, TLinkedList* populacao1, Labirinto* lab, int w_distancia) {
  if (!elite || !populacao1) return NULL;

  TLinkedList* nova_geracao = list_create();
  if (!nova_geracao) return NULL;

  // Adicionar elite clonada
  for (TNo* atual_elite = elite->inicio; atual_elite != NULL; atual_elite = atual_elite->prox) {
    Individuo clone = clonar_individuo(&atual_elite->info);
    list_insert_sorted(nova_geracao, clone);
  }

  // Criar lista de não-elite
  TLinkedList* nao_elite = list_create();
  TNo* atual = populacao1->inicio;
  for (int i = 0; i < list_size(elite) && atual != NULL; i++, atual = atual->prox);
  for (; atual != NULL; atual = atual->prox) {
    list_insert_end(nao_elite, atual->info);
  }

  // CORREÇÃO: Calcular tamanho desejado
  int tamanho_desejado = list_size(populacao1);
  int filhos_necessarios = tamanho_desejado - list_size(elite);

  // Realizar cruzamentos controlados
  for (TNo* atual_elite = elite->inicio; 
       atual_elite != NULL && nao_elite->inicio != NULL && filhos_necessarios > 0; 
       atual_elite = atual_elite->prox) 
  {
    int idx_pai = selecionar_pai(nao_elite);
    TNo* pai_nao_elite = nao_elite->inicio;
    for (int i = 0; i < idx_pai && pai_nao_elite != NULL; i++, pai_nao_elite = pai_nao_elite->prox);
    if (!pai_nao_elite) pai_nao_elite = nao_elite->inicio;

    Individuo filho1, filho2;
    crossover_inds(&atual_elite->info, &pai_nao_elite->info, &filho1, &filho2);

    // CORREÇÃO: Inserir apenas até completar tamanho
    calcular_fitness(lab, &filho1, w_distancia);
    list_insert_sorted(nova_geracao, filho1);
    filhos_necessarios--;

    if (filhos_necessarios > 0) {
      calcular_fitness(lab, &filho2, w_distancia);
      list_insert_sorted(nova_geracao, filho2);
      filhos_necessarios--;
    } else {
      // Liberar filho excedente
      free(filho2.caminho->data);
      free(filho2.caminho);
    }
  }

  list_destroy(nao_elite, 0);
  return nova_geracao;
}

Individuo* condicao_parada(const Labirinto *lab, TLinkedList *populacao) {
  if (!lab || !populacao) return NULL;

  for (TNo *atual = populacao->inicio; atual != NULL; atual = atual->prox) {
    int colisoes = 0;
    Posicao final = simular_movimentos(lab, &atual->info, &colisoes, NULL);
    if (final.i == lab->saida.i && final.j == lab->saida.j){
      printf("\n--- SOLUCAO ENCONTRADA ---\n");
      printf("Individuo com fitness %d atingiu a saida (%u, %u)\n", 
      atual->info.fitness, final.i, final.j);
      return &atual->info; // Retorna o indivíduo que resolveu
    }
  }
  return NULL;
}

void imprimir_individuo(const Labirinto* lab, Individuo* indiv, int geracao) {
  char** lab_copia = copiar_matriz(lab->labirinto, lab->n, lab->m);
  int colisoes = 0;
  Posicao final = simular_movimentos(lab, indiv, &colisoes, lab_copia);

  printf("\n=== INDIVIDUO (Geracao %d) ===\n", geracao);
  printf("Fitness: %d\n", indiv->fitness);
  printf("Posicao final: (%u, %u)\n", final.i, final.j);
  printf("Distancia ate saida: %d\n", calcular_distancia_manhattan(final, lab->saida));
  printf("Colisoes: %d\n", colisoes);
  printf("Caminho: ");
  Stack_print(indiv->caminho);
  printf("\n");
  
  printf("Labirinto com caminho:\n");
  for (uint i = 0; i < lab->n; i++) {
    for (uint j = 0; j < lab->m; j++) {
      printf("%c", lab_copia[i][j]);
    }
    printf("\n");
  }
  
  for (uint i = 0; i < lab->n; i++) free(lab_copia[i]);
  free(lab_copia);
}

void print_populacao(TLinkedList *populacao) {
  if (!populacao) return;

  int contador = 1;
  for (TNo *atual = populacao->inicio; atual != NULL; atual = atual->prox, contador++) {
    printf("Individuo %d:\n", contador);
    printf("  Fitness: %d\n", atual->info.fitness);
    printf("  Caminho: ");
    Stack_print(atual->info.caminho);
    printf("\n\n");
  }
}

void liberar_populacao(TLinkedList *populacao) { free(populacao); }
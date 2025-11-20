#include <stdio.h>
#include "scoresREAL.h"

void salvar_scores(char *arquivo, Score *head) {
    FILE *f = fopen(arquivo, "w");
    if (!f) return;

    Score *atual = head;

    int contador = 0;
    while (atual && contador < 3) {   // salva só os 3 melhores
        fprintf(f, "%.2f\n", atual->tempo);
        atual = atual->prox; 
        contador++;
    }

    fclose(f);
}


void liberar_scores(Score *head) {
    while (head) {
        Score *atual = head;
        head = head->prox;
        free(atual);
    }
}

void imprimir_score(Score *head) {
    Score *atual = head;
    int posicao = 1;

    while (atual) {
        printf("%d. %.2f segundos\n", posicao, atual->tempo);
        atual = atual->prox;
        posicao++;
    }
}
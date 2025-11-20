#include <stdio.h>
#include "scoresREAL.h"

void imprimir_score(Score *head) {
    Score *atual = head;
    int posicao = 1;

    while (atual) {
        printf("%d. %.2f segundos\n", posicao, atual->tempo);
        atual = atual->prox;
        posicao++;
    }
}
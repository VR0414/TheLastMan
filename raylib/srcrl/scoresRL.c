#include "scoresRL.h"
#include <stdio.h>
#include <stdlib.h>
Score* carregar_scores(const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (!f) return NULL; // Se não existir, começa vazio

    Score *lista = NULL;
    float t;

    while (fscanf(f, "%f", &t) == 1) {
        adicionar_score_ordenado(&lista, t);
    }

    fclose(f);
    return lista;
}
void salvar_scores(const char *arquivo, Score *lista) {
    FILE *f = fopen(arquivo, "w");
    if (!f) return;

    Score *atual = lista;
    while (atual) {
        fprintf(f, "%.2f\n", atual->tempo);
        atual = atual->proximo;
    }

    fclose(f);
}
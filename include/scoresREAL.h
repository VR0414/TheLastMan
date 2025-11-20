#ifndef SCORE_H
#define SCORE_H

typedef struct Score {
    float tempo;
    struct Score *prox;
} Score;

void imprimir_score(Score *head);
void salvar_scores(char *arquivo, Score *head);
void adicionar_score_ordenado(Score **head, float tempo);
void liberar_scores(Score *head);

#endif
#ifndef SCORES_RL_H
#define SCORES_RL_H

typedef struct Score {
    float tempo;
    struct Score *prox;
} Score;

Score* carregar_scores(const char *arquivo);
void salvar_scores(const char *arquivo, Score *lista);
void adicionar_score_ordenado(Score **lista, float tempo);
void liberar_scores(Score *lista);

#endif

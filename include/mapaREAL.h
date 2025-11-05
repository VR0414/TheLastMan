#ifndef MAPA_H
#define MAPA_H

#define MAPA_LINHAS 24
#define MAPA_COLUNAS 80

typedef struct Mapa {

    int linhas;
    int colunas;
    char abrigo;
    char *celulas;

} Mapa;

Mapa *criar_mapa(int linhas, int colunas, char nulo, char abrigo); // Fun;ão para criar mapa e retornar ponteiro para o mesmo

void liberar_mapa(Mapa *mapa); // Função para liberar mapa criado

int preencher_limites(Mapa *mapa, char paredes); // Função para limitar mapa

void preencher_chao(Mapa *mapa, char nulo, char paredes, char abrigo);

void preencher_abrigo(Mapa *mapa, char abrigo);

void imprimir_mapa(Mapa *mapa);

#endif
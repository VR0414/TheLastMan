#ifndef MAPA_RL_H
#define MAPA_RL_H

#define MAPA_LINHAS 24
#define MAPA_COLUNAS 80

typedef struct Mapa {
    int linhas;
    int colunas;
    char abrigo;
    char *celulas;
} Mapa;

Mapa *criar_mapa(int linhas, int colunas, char abrigo);
void liberar_mapa(Mapa *mapa);
int preencher_limites(Mapa *mapa, char parede);
void preencher_chao(Mapa *mapa, char vazio, char parede, char abrigo);
void preencher_abrigo(Mapa *mapa, char abrigo);
void imprimir_mapa(Mapa *mapa);
void desenhar_mapa(Mapa *mapa, int tamanho_celula);  // Raylib

#endif

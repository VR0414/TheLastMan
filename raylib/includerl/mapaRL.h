#ifndef MAPA_RL_H
#define MAPA_RL_H

#define MAPA_LINHAS 24
#define MAPA_COLUNAS 80

// Caracteres padrão
#define CEL_PAREDE '#'
#define CEL_CHAO   '.'
#define CEL_ABRIGO '='

typedef struct Mapa {
    int linhas;
    int colunas;
    char abrigo;      // símbolo do abrigo
    char parede;      // símbolo da parede
    char vazio;       // símbolo do chão/solo
    char *celulas;    // grade 2D armazenada em vetor linear
} Mapa;

Mapa *criar_mapa(int linhas, int colunas, char abrigo, char parede, char vazio);
void liberar_mapa(Mapa *mapa);

// Construção
int preencher_limites(Mapa *mapa);
void preencher_chao(Mapa *mapa);
void preencher_abrigo(Mapa *mapa);

// Utilidades
void imprimir_mapa(Mapa *mapa);

// Raylib
void desenhar_mapa(Mapa *mapa, int tamanho_celula);

#endif

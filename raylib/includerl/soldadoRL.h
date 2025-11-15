#ifndef SOLDADO_RL_H
#define SOLDADO_RL_H

#include "mapaRL.h"
#include "balasRL.h"

typedef struct Soldado {
    float linha;     // posição vertical (grade)
    float coluna;    // posição horizontal (grade)
    int vida;
    char simbolo;    // só usado se desenhar no modo texto
} Soldado;

Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo);
void liberar_soldado(Soldado *s);
void iniciar_soldado(Soldado *s, float linha, float coluna, int vida, char simbolo);

// movimento com colisão
int mover_soldado(Soldado *s, Mapa *mapa, float deslocLinha, float deslocColuna);

// dano + reset
int aplicar_dano_soldado(Soldado *s, int dano, float inicioLinha, float inicioColuna);

// ➕ ADICIONADA:
void reset_posicao_soldado(Soldado *s, float linha, float coluna);

#endif

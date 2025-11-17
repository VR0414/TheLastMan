#ifndef SOLDADO_RL_H
#define SOLDADO_RL_H

#include "mapaRL.h"

typedef struct Soldado {
    float linha;     // posição vertical (grade)
    float coluna;    // posição horizontal (grade)
    int vida;
    char simbolo;    // usado apenas no modo texto
} Soldado;

// Criação / destruição
Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo);
void liberar_soldado(Soldado *s);

// Inicialização/reinício
void iniciar_soldado(Soldado *s, float linha, float coluna, int vida, char simbolo);
void reset_posicao_soldado(Soldado *s, float linha, float coluna);

// Movimento com verificação de colisão no mapa
// retorno 1 = moveu; 0 = bloqueado
int mover_soldado(Soldado *s, Mapa *mapa, float deslocLinha, float deslocColuna);

// Dano + reset após ser atingido (não reseta aqui se não quiser)
int aplicar_dano_soldado(Soldado *s, int dano, float inicioLinha, float inicioColuna);

#endif

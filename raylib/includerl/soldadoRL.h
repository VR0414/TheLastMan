#ifndef SOLDADORL_H
#define SOLDADORL_H

#include "mapaRL.h"
#include "balasRL.h"
#include "raylib.h"   // Para usar Vector2 e desenho (futuro)

typedef struct Soldado {

    Vector2 pos;      // posição em float (Raylib usa Vector2)
    int vida;
    char simbolo;

} Soldado;

/* Cria um soldado dinamicamente */
Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo);

/* Libera memória do soldado */
void liberar_soldado(Soldado *s);

/* Reinicia os valores do soldado */
void iniciar_soldado(Soldado *s, float linha, float coluna, int vida, char simbolo);

/* Move o soldado no mapa (retorna 1 se moveu, 0 se bloqueado) */
int mover_soldado(Soldado *s, Mapa *mapa, float deslocLinha, float deslocColuna);

/* Reseta a posição do soldado */
int reset_posicao_soldado(Soldado *s, float linha, float coluna);

/* Aplica dano e reseta posição se morrer */
int dano_soldado(Soldado *s, int dano, float inicioLinha, float inicioColuna);

/* NOVO — desenhar o soldado com Raylib */
void desenhar_soldado(Soldado *s, int tamanho_celula);

#endif // SOLDADORL_H

#ifndef BALAS_RL_H
#define BALAS_RL_H

#include "raylib.h"

typedef struct Bala {
    Vector2 posicao;
    Vector2 velocidade;
    int dano;
    Rectangle colisao;
    struct Bala *proxima;
} Bala;

typedef struct {
    Bala *head;
    int quantidade;
} ListaBalas;

Bala *criar_bala(Vector2 pos, Vector2 vel, int dano, float width, float height);
void inicio_lista_balas(ListaBalas *lista);
void adicionar_bala(ListaBalas *lista, Bala *b);
void remover_bala(ListaBalas *lista, Bala *alvo);
void atualizar_balas(ListaBalas *lista, int screenWidth, int screenHeight);
void desenhar_balas(ListaBalas *lista, Texture2D texturaBala);
void liberar_lista_balas(ListaBalas *lista);

#endif

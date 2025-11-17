#ifndef BALAS_RL_H
#define BALAS_RL_H

#include "mapaRL.h"

// ----------------------------------------------------
// STRUCT DA BALA
// ----------------------------------------------------
typedef struct Bala {
    float linha;
    float coluna;
    float velLinha;
    float velColuna;
    int dano;
    struct Bala *proxima;
} Bala;

// ----------------------------------------------------
// LISTA DE BALAS
// ----------------------------------------------------
typedef struct {
    Bala *head;
    int quantidade;
} ListaBalas;

// ----------------------------------------------------
// FUNÇÕES PRINCIPAIS
// ----------------------------------------------------
Bala *criar_bala(float l, float c, float velL, float velC, int dano);

void inicio_lista_balas(ListaBalas *lista);

void adicionar_bala(ListaBalas *lista, Bala *b);

void remover_bala(ListaBalas *lista, Bala *alvo);

void atualizar_balas(ListaBalas *lista, Mapa *mapa);

void desenhar_balas(ListaBalas *lista, int tamanho_celula);

// libertar toda a lista
void liberar_lista_balas(ListaBalas *lista);

#endif

#ifndef BALAS_RL_H
#define BALAS_RL_H

#include "mapaRL.h"

typedef struct Bala {
    float linha;
    float coluna;
    float velLinha;
    float velColuna;
    int contador_tempo;
    struct Bala *proxima;
} Bala;

typedef struct {
    Bala *head;
    int quantidade_balas;
} ListaBalas;

Bala *criar_bala(float l, float c, float velL, float velC, int tempo);
void inicio_lista_balas(ListaBalas *lista);
void inserir_bala_inicio(ListaBalas *lista, Bala *bala);
void remover_balas_lista(ListaBalas *lista, Bala *alvo);
void atualizar_balas(ListaBalas *lista, Mapa *mapa, char parede, char abrigo);
void desenhar_balas(ListaBalas *lista, int tamanho_celula); // Raylib

#endif


#ifndef BALAS_H
#define BALAS_H

#include "mapaREAL.h"

typedef struct Bala {

    float linha, coluna; // +1 para direita, -1 para esquerda;
    float velLinha, velColuna; 
    int contador_tempo; // "tick" para determinar movimento da bala
    struct Bala *proxima; 

} Bala;

typedef struct ListaBalas {

    Bala *head;
    int quantidade_balas;

} ListaBalas;

Bala *criar_bala(float l, float c, float velL, float velC, int tempo);

void inicio_lista_balas(ListaBalas *lista);

void liberar_lista_balas(ListaBalas *lista);    

void inserir_bala_inicio(ListaBalas *lista, Bala *bala);

void remover_balas_lista(ListaBalas *lista, Bala *bala_alvo);

void atualizar_balas(ListaBalas *lista, Mapa *mapa, char paredes, char abrigo);

void imprimir_mapa_com_balas(Mapa *mapa, ListaBalas *lista, char simbolo_bala);

int disparar_bala(ListaBalas *lista, float l, float c, float velL, float velC, int tempo);

#endif 
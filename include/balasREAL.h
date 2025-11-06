#ifndef BALAS_H
#define BALAS_H

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

void inserir_bala_inicio(ListaBalas *lista, Bala *bala);

void remover_balas_lista(ListaBalas *lista, Bala *bala_alvo);

#endif
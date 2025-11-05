#ifndef BALAS_H
#define BALAS_H

typedef struct Bala {

    int linha;
    int coluna;
    int direcao; // +1 para direita, -1 para esquerda;
    int velocidade;
    int contador_movimento; // "tick" para determinar movimento da bala
    struct Bala *proxima; 

} Bala;

typedef struct ListaBalas {

    Bala *head;
    int quantidade_balas;

} ListaBalas;






#endif
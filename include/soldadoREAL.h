#ifndef SOLDADO_H
#define SOLDADO_H

#include "mapaREAL.h"
#include "balasREAL.h"

typedef struct Soldado {

    float linha, coluna;
    int vida;
    char simbolo;

} Soldado;

Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo);

void liberar_soldado(Soldado *s);

void iniciar_soldado(Soldado *s, float linha, float coluna, int vida, char simbolo);

int mover_soldado(Soldado *s, Mapa *mapa, float deslocLinha, float deslocColuna, int entrada_abrigo);




#endif
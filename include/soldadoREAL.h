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

void inicia_soldado(Soldado *s, float linha, float coluna, int vida, char simbolo);





#endif
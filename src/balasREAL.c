#include <stdlib.h>
#include <stdio.h>
#include "balasREAL.h"  

Bala *criar_bala(float l, float c, float velL, float velC, int tempo) {

    Bala *bala = (Bala *)malloc(sizeof(Bala));

    bala->coluna = c;
    bala->linha = l;
    bala->velColuna = velC;
    bala->velLinha = velL;
    bala->contador_tempo = tempo;
    bala->proxima = NULL;

    return bala;
    
}
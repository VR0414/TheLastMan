#include "mapaREAL.h"
#include <stdio.h>
#include <stdlib.h>

Mapa *criar_mapa(int linhas, int colunas, char nulo, char abrigo) {

    if (linhas <= 0 || colunas <= 0)
        return NULL;

    Mapa *mapa = (Mapa *)malloc(sizeof(Mapa));

    mapa->linhas = linhas;
    mapa->colunas = colunas;
    mapa->abrigo = abrigo;

    mapa->celulas = (char *)malloc((size_t)linhas * (size_t)colunas * sizeof(char));

    for (int l = 0; l < linhas; l++) {
        for (int c = 0; c < colunas; c++) {
            int idx = l * colunas + c;
            mapa->celulas[idx] = nulo;
        }
    }
}
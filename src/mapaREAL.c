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
            int indice = l * colunas + c;
            mapa->celulas[indice] = nulo;
        }
    }

    int largura_abrigo = 7;
    int centro = colunas / 2;

    int inicio = centro - (largura_abrigo / 2);
    if (inicio < 0) 
        inicio = 0;

    int fim = centro + (largura_abrigo / 2); // int fim = inicio + largura_abrigo;
    if (fim > colunas)
        fim = colunas;

    for (int casa = inicio; casa < fim; casa++) {
        int a = casa;
        mapa->celulas[a] = abrigo;
    }

    return mapa;
    
}
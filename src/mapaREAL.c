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

    mapa->celulas = (char *)malloc(linhas * colunas * sizeof(char));

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
        inicio = 1; // Inicia abrigo na linha 1 

    int fim = centro + (largura_abrigo / 2); // int fim = inicio + largura_abrigo;
    if (fim > colunas)
        fim = colunas;

    int linha_abrigo = mapa->linhas - 1;

    for (int casa = inicio; casa < fim; casa++) { // casa = abrigo
        int a = linha_abrigo * colunas + casa; // "a" de Abrigo, apenas para fazer referência
        mapa->celulas[a] = abrigo;
    }

    return mapa;

}

void liberar_mapa(Mapa *mapa) {

    if (mapa->celulas != NULL) 
        free(mapa->celulas);
    free(mapa);

}

int mapa_limites(Mapa *mapa, char paredes) {

    if (mapa->linhas < 0 || mapa->colunas < 0) 
        return -1;

    // mapa->colunas = 80;
    for (int LinhaSuperior = 0; LinhaSuperior < mapa->colunas; LinhaSuperior++) { // Preenche Linha Superior do mapa
        int indice = LinhaSuperior;
        mapa->celulas[indice] = paredes;
    }

    // mapa->colunas = 80; 
    for (int LinhaInferior = 0; LinhaInferior < mapa->colunas; LinhaInferior++) { // Preenche Linha Inferior do mapa
        int indice = (mapa->colunas - 1) * mapa->colunas + LinhaInferior;
        mapa->celulas[indice] = paredes;
    }

    for (int Linhas = 0; Linhas < mapa->linhas; Linhas++) { // Preenche Colunas esquerda e direita do mapa
        int esquerda = mapa->linhas * mapa->colunas;
        int direita = mapa->linhas * mapa->colunas + (mapa->colunas - 1);
        mapa->celulas[esquerda] = paredes;
        mapa->celulas[direita] = paredes;
    }

    return 0;
}
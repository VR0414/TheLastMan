#include "mapaREAL.h"
#include <stdio.h>
#include <stdlib.h>

Mapa *criar_mapa(int linhas, int colunas, char abrigo) {

    if (linhas <= 0 || colunas <= 0)
        return NULL;

    Mapa *mapa = (Mapa *)malloc(sizeof(Mapa));

    mapa->linhas = linhas;
    mapa->colunas = colunas;
    mapa->abrigo = abrigo;

    mapa->celulas = (char *)malloc(linhas * colunas * sizeof(char)); // Aloca todas as células

    return mapa;

}

void liberar_mapa(Mapa *mapa) {

    if (mapa->celulas != NULL) 
        free(mapa->celulas);
    free(mapa);

}

int preencher_limites(Mapa *mapa, char paredes) {

    if (mapa->linhas < 0 || mapa->colunas < 0) 
        return -1;

    // Preenchimento Linha 0 | mapa->colunas = 80;
    for (int LinhaSuperior = 0; LinhaSuperior < mapa->colunas; LinhaSuperior++) { 
        mapa->celulas[LinhaSuperior] = paredes;
    }

    // Preenche Linha (mapa->linhas - 1) -> Última linha | mapa->colunas = 80; 
    for (int LinhaInferior = 0; LinhaInferior < mapa->colunas; LinhaInferior++) { 
        int indice = (mapa->linhas - 1) * mapa->colunas + LinhaInferior;
        mapa->celulas[indice] = paredes;
    }

    // Preenche coluna esquerda (mapa->colunas = 0) e coluna direita (mapa->colunas - 1)
    for (int l = 0; l < mapa->linhas; l++) { 
        int esquerda = l * mapa->colunas;
        int direita = l * mapa->colunas + (mapa->colunas - 1);
        mapa->celulas[esquerda] = paredes;
        mapa->celulas[direita] = paredes;
    }

    return 0;
}

void preencher_chao(Mapa *mapa, char nulo, char paredes, char abrigo) {

    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++) {

            int indice = l * mapa->colunas + c;

            if (mapa->celulas[indice] != paredes && mapa->celulas[indice] != abrigo) {
                mapa->celulas[indice] = nulo;
            }
        }
    }
}

void preencher_abrigo(Mapa *mapa, char abrigo) {

    int colunas = mapa->colunas; 
    int linhas = mapa->linhas;   

    int largura_abrigo = 7; 
    int centro = colunas / 2; 

    int inicio = centro - (largura_abrigo / 2); 
    if (inicio < 0) 
        inicio = 0; 

    int fim = inicio + largura_abrigo; 
    if (fim > colunas)
        fim = colunas; 

    int linha_abrigo = mapa->linhas - 2; // Define linha do abrigo como ante-penúltima

    for (int casa = inicio; casa < fim; casa++) { 
        if (casa >= 0 && casa < colunas && linha_abrigo >= 0 && linha_abrigo < linhas) {
            int indice = linha_abrigo * colunas + casa; 
            mapa->celulas[indice] = abrigo;
        }
    }
}

void imprimir_mapa(Mapa *mapa) {

    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++) {
            putchar(mapa->celulas[l * mapa->colunas + c]);
        }
        putchar('\n');
    }

}

#include "mapaREAL.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

Mapa *criar_mapa(int linhas, int colunas, char abrigo) {
    if (linhas <= 0 || colunas <= 0){
        return NULL;}

    Mapa *mapa = (Mapa *)malloc(sizeof(Mapa));
    if (!mapa) return NULL;

    mapa->linhas = linhas;
    mapa->colunas = colunas;
    mapa->abrigo = abrigo;

    mapa->celulas = (char *)calloc((size_t)linhas * (size_t)colunas, sizeof(char));
    if (!mapa->celulas) { 
        free(mapa); return NULL; }
    for (int i = 0; i < linhas * colunas; i++){
        mapa->celulas[i] = ' ';}
    return mapa;
}

void liberar_mapa(Mapa *mapa) {
    if (!mapa) return;
    if (mapa->celulas){
         free(mapa->celulas);}
    free(mapa);
}

int preencher_limites(Mapa *mapa, char parede) {
    if (mapa == NULL) return -1;
    if (mapa->linhas <= 0 || mapa->colunas <= 0) return -1;
    if (mapa->celulas == NULL) return -1;

    for (int c = 0; c < mapa->colunas; c++) {
        mapa->celulas[c] = parede;
        mapa->celulas[(mapa->linhas - 1) * mapa->colunas + c] = parede;
    }
    for (int l = 0; l < mapa->linhas; l++) {
        mapa->celulas[l * mapa->colunas] = parede;
        mapa->celulas[l * mapa->colunas + mapa->colunas - 1] = parede;
    }
    return 0;
}

void preencher_chao(Mapa *mapa, char vazio, char parede, char abrigo) {
    if (mapa == NULL) return;
    if (mapa->celulas == NULL) return;

    int total = mapa->linhas * mapa->colunas;
    for (int i = 0; i < total; i++) {
        if (mapa->celulas[i] != parede && mapa->celulas[i] != abrigo)
            mapa->celulas[i] = vazio;
    }
}

void preencher_abrigo(Mapa *mapa, char abrigo) {
    int colunas = mapa->colunas;
    int largura = 7;
    int centro = colunas / 2;
    int inicio = centro - largura / 2;
    int fim = inicio + largura;
    int linha = mapa->linhas - 2;

    for (int c = inicio; c < fim; c++) {
        mapa->celulas[linha * mapa->colunas + c] = abrigo;
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

// FUNÇÃO NOVA → desenhar o mapa com cores na tela
void desenhar_mapa(Mapa *mapa, int tamanho_celula) {
    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++) {
            char cel = mapa->celulas[l * mapa->colunas + c];
            Color cor;

            if (cel == '#') cor = DARKGRAY;
            else if (cel == '=') cor = ORANGE;
            else if (cel == '.') cor = BEIGE;
            else cor = WHITE;

            DrawRectangle(c * tamanho_celula, l * tamanho_celula, tamanho_celula, tamanho_celula, cor);
        }
    }
}

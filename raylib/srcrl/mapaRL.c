#include "mapaRL.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

static inline char mapa_get(Mapa *mapa, int l, int c) {
    return mapa->celulas[l * mapa->colunas + c];
}

static inline void mapa_set(Mapa *mapa, int l, int c, char v) {
    mapa->celulas[l * mapa->colunas + c] = v;
}

Mapa *criar_mapa(int linhas, int colunas, char abrigo, char parede, char vazio) {
    if (linhas <= 0 || colunas <= 0) return NULL;

    Mapa *mapa = malloc(sizeof(Mapa));
    if (!mapa) return NULL;

    mapa->linhas = linhas;
    mapa->colunas = colunas;
    mapa->abrigo = abrigo;
    mapa->parede = parede;
    mapa->vazio = vazio;

    int total = linhas * colunas;
    mapa->celulas = malloc((size_t)total * sizeof(char));
    if (!mapa->celulas) {
        free(mapa);
        return NULL;
    }

    for (int i = 0; i < total; i++) mapa->celulas[i] = vazio;
    return mapa;
}

void liberar_mapa(Mapa *mapa) {
    if (!mapa) return;
    free(mapa->celulas);
    free(mapa);
}

int preencher_limites(Mapa *mapa) {
    if (!mapa || !mapa->celulas) return -1;
    for (int c = 0; c < mapa->colunas; c++) {
        mapa_set(mapa, 0, c, mapa->parede);
        mapa_set(mapa, mapa->linhas - 1, c, mapa->parede);
    }
    for (int l = 0; l < mapa->linhas; l++) {
        mapa_set(mapa, l, 0, mapa->parede);
        mapa_set(mapa, l, mapa->colunas - 1, mapa->parede);
    }
    return 0;
}

void preencher_chao(Mapa *mapa) {
    if (!mapa || !mapa->celulas) return;
    int total = mapa->linhas * mapa->colunas;
    for (int i = 0; i < total; i++) {
        if (mapa->celulas[i] != mapa->parede && mapa->celulas[i] != mapa->abrigo)
            mapa->celulas[i] = mapa->vazio;
    }
}

void preencher_abrigo(Mapa *mapa) {
    if (!mapa || !mapa->celulas) return;
    if (mapa->linhas <= 1 || mapa->colunas <= 2) return;

    int largura = 7;
    if (mapa->colunas - 2 < largura) largura = mapa->colunas - 2;

    int centro = mapa->colunas / 2;
    int inicio = centro - largura / 2;
    if (inicio < 1) inicio = 1;
    int fim = inicio + largura;
    if (fim > mapa->colunas - 1) fim = mapa->colunas - 1;

    int linha = mapa->linhas - 2;
    for (int c = inicio; c < fim; c++)
        mapa_set(mapa, linha, c, mapa->abrigo);
}

void imprimir_mapa(Mapa *mapa) {
    if (!mapa || !mapa->celulas) return;
    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++)
            putchar(mapa_get(mapa, l, c));
        putchar('\n');
    }
}

void desenhar_mapa(Mapa *mapa, int tam) {
    if (!mapa) return;
    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++) {
            char cel = mapa_get(mapa, l, c);
            Color cor = WHITE;

            if (cel == mapa->parede) cor = DARKGRAY;
            else if (cel == mapa->abrigo) cor = ORANGE;
            else if (cel == mapa->vazio) cor = BEIGE;

            DrawRectangle(c * tam, l * tam, tam, tam, cor);
        }
    }
}


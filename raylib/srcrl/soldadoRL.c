#include "soldadoRL.h"
#include <stdlib.h>
#include <math.h>

Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo) {
    Soldado *s = malloc(sizeof(Soldado));
    if (!s) return NULL;
    s->linha = linha;
    s->coluna = coluna;
    s->vida = vida;
    s->simbolo = simbolo;
    return s;
}

void liberar_soldado(Soldado *s) {
    if (s) free(s);
}

void iniciar_soldado(Soldado *s, float linha, float coluna, int vida, char simbolo) {
    if (!s) return;
    s->linha = linha;
    s->coluna = coluna;
    s->vida = vida;
    s->simbolo = simbolo;
}

static inline char mapa_get(Mapa *mapa, int l, int c) {
    return mapa->celulas[l * mapa->colunas + c];
}

int mover_soldado(Soldado *s, Mapa *mapa, float dL, float dC) {
    if (!s || !mapa) return 0;

    float novaL = s->linha + dL;
    float novaC = s->coluna + dC;
    int L = (int)floorf(novaL);
    int C = (int)floorf(novaC);

    if (L < 0 || C < 0 || L >= mapa->linhas || C >= mapa->colunas)
        return 0;

    char cel = mapa_get(mapa, L, C);

    if (cel != mapa->parede) {
        s->linha = novaL;
        s->coluna = novaC;
        return 1;
    }

    return 0;
}

int aplicar_dano_soldado(Soldado *s, int dano, float inicioL, float inicioC) {
    if (!s) return 0;
    s->vida -= dano;
    if (s->vida < 0) s->vida = 0;

    // reset após dano
    s->linha = inicioL;
    s->coluna = inicioC;
    return s->vida;
}

void reset_posicao_soldado(Soldado *s, float linha, float coluna) {
    if (!s) return;
    s->linha = linha;
    s->coluna = coluna;
}

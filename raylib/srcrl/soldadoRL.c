#include <stdlib.h>
#include "soldadoRL.h"

Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo) {
    Soldado *s = malloc(sizeof(Soldado));
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
    s->linha = linha;
    s->coluna = coluna;
    s->vida = vida;
    s->simbolo = simbolo;
}

int mover_soldado(Soldado *s, Mapa *mapa, float dL, float dC) {
    float novaL = s->linha + dL;
    float novaC = s->coluna + dC;
    int L = (int)novaL;
    int C = (int)novaC;

    if (L < 0 || C < 0 || L >= mapa->linhas || C >= mapa->colunas)
        return 0;

    char cel = mapa->celulas[L * mapa->colunas + C];

    if (cel == mapa->abrigo || cel == '.')
    {
        s->linha = novaL;
        s->coluna = novaC;
        return 1;
    }

    return 0;
}

int aplicar_dano_soldado(Soldado *s, int dano, float inicioL, float inicioC) {
    s->vida -= dano;
    if (s->vida <= 0)
        s->vida = 0;

    // reset
    s->linha = inicioL;
    s->coluna = inicioC;

    return s->vida;
}

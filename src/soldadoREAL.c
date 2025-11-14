#include <stdio.h>
#include <stdlib.h>
#include "soldadoREAL.h"

Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo) {

    Soldado *s = (Soldado *)malloc(sizeof(Soldado));

    s->linha = linha;
    s->coluna = coluna;
    s->vida = vida;
    s->simbolo = simbolo;

    return s;
}

void liberar_soldado(Soldado *s) {

    if (s == NULL)
        return;
    free(s);

}

void iniciar_soldado( Soldado *s, float linha, float coluna, int vida, char simbolo) {

    s->linha = linha;
    s->coluna = coluna;
    s->vida = vida;
    s->simbolo = simbolo;
    
}

int mover_soldado(Soldado *s, Mapa *mapa, float deslocLinha, float deslocColuna) {

    float nova_linha = s->linha + deslocLinha; // Suavização de movimento
    float nova_coluna = s->coluna + deslocColuna;

    int intLinha = (int)nova_linha; // Converte para aplicar mudança correta após suavização
    int intColuna = (int)nova_coluna;

    if (intLinha < 0 || intLinha >= mapa->linhas || intColuna < 0 || intColuna >= mapa->colunas)
        return 0; // fora do mapa -> bloqueado

    char destino = mapa->celulas[intLinha * mapa->colunas + intColuna];

    if (destino == mapa->abrigo) {
        s->linha = nova_linha;
        s->coluna = nova_coluna;
        return 1;
    }
    
    if (destino != ' ')
        return 0;
    
    s->linha = nova_linha;
    s->coluna = nova_coluna;
    return 1;

}

void reset_posicao_soldado(Soldado *s, float linha, float coluna)  {

    s->linha = linha;
    s->coluna = coluna;

}

int aplicar_dano_soldado(Soldado *s, int dano, float inicioLinha, float inicioColuna) {

    s->vida -= dano;
    if (s->vida <= 0) {
        s->vida = 0;
        return 0;
    } 

    reset_posicao_soldado(s, inicioLinha, inicioColuna);

    return s->vida;

}
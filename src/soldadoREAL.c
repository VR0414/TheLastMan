#include <Stdio.h>
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

int mover_soldado(Soldado *s, Mapa *mapa, float deslocLinha, float deslocColuna, int entrada_abrigo) {

    float nova_linha = s->linha + deslocLinha;
    float nova_coluna = s->coluna + deslocColuna;

    int intLinha = (int)nova_linha; // Conversão após uso de suavização de movimento do soldao sobre linhas e colunas do mapa
     
    int intColuna = (int)nova_coluna;

    if (intLinha < 0 || intLinha >= mapa->linhas || intColuna < 0 || intColuna >= mapa->colunas)
        return 0;

    char destino = mapa->celulas[intLinha * mapa->colunas + intColuna];

    if (destino == mapa->abrigo ) 
        return 1;

    if (destino != ' ')
        return 0;
    return 1;

    s->linha = nova_linha;
    s->coluna = nova_coluna;
    return 1;

}

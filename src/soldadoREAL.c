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

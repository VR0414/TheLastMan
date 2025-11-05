#include <stdio.h>
#include <stdlib.h>
#include "mapaT.h"

Mapa *criar_mapa(int linhas, int colunas){
    Mapa *m = malloc(sizeof(Mapa));
    if(!m) return NULL;
    m->linhas = linhas;
    m->colunas = colunas;
    m->celulas = malloc(linhas * sizeof(char*));
    for(int i=0;i<linhas;i++){
        m->celulas[i] = malloc((colunas+1) * sizeof(char));
        for(int j=0;j<colunas;j++) m->celulas[i][j] = ' ';
        m->celulas[i][colunas] = '\0';
    }
    return m;
}

void liberar_mapa(Mapa *m){
    if(!m) return;
    for(int i=0;i<m->linhas;i++) free(m->celulas[i]);
    free(m->celulas);
    free(m);
}

void definir_celula(Mapa *m, int r, int c, char ch){
    if(!m) return;
    if(r<0 || r>=m->linhas || c<0 || c>=m->colunas) return;
    m->celulas[r][c] = ch;
}

char obter_celula(Mapa *m, int r, int c){
    if(!m) return 0;
    if(r<0 || r>=m->linhas || c<0 || c>=m->colunas) return 0;
    return m->celulas[r][c];
}

void desenhar_mapa(Mapa *m){
    // limpar tela e posicionar cursor no canto (ANSI)
    printf("\033[H\033[J");
    for(int i=0;i<m->linhas;i++){
        printf("%s\n", m->celulas[i]);
    }
    fflush(stdout);
}

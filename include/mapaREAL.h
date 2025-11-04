#ifndef MAPA_h
#define MAPA_H

#define MAPA_LINHAS 24
#define MAPA_COLUNAS 80

typedef struct Mapa {

    int linhas;
    int colunas;
    char abrigo;
    char *celulas;

} Mapa;

Mapa *criar_mapa(int linhas, int colunas, char nulo, char abrigo);







#endif
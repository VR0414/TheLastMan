#ifndef MAPA_H
#define MAPA_H

typedef struct Mapa {
    int linhas;
    int colunas;
    char **celulas;
} Mapa;

Mapa *criar_mapa(int linhas, int colunas);
void liberar_mapa(Mapa *m);
void definir_celula(Mapa *m, int r, int c, char ch);
char obter_celula(Mapa *m, int r, int c);
void desenhar_mapa(Mapa *m);

#endif // MAPA_H

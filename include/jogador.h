#ifndef JOGADOR_H
#define JOGADOR_H

typedef struct Soldado {
    int linha;
    int coluna;
    char simbolo;
} Soldado;

Soldado *criar_soldado(int linha, int coluna);
void liberar_soldado(Soldado *s);
void posicionar_soldado_no_mapa(Soldado *s, void *mapa); // mapa é Mapa*
int mover_soldado(Soldado *s, int dr, int dc, void *mapa);

#endif // JOGADOR_H

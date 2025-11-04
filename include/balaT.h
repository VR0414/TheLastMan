#ifndef BALA_H
#define BALA_H

typedef struct Bala {
    int linha;
    int coluna;
    int dx; // direção horizontal: -1 (vai para esquerda) ou +1 (vai para direita)
    struct Bala *proximo;
} Bala;

typedef struct ListaBalas {
    Bala *inicio;
} ListaBalas;

ListaBalas *criar_lista_balas();
void liberar_lista_balas(ListaBalas *l);
void adicionar_bala(ListaBalas *l, int linha, int coluna, int dx);
void atualizar_balas(ListaBalas *l, void *mapa, int *colisao, int px, int py);
void desenhar_balas_em_mapa(ListaBalas *l, void *mapa);

#endif // BALA_H

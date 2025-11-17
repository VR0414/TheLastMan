#include <raylib.h>
#include "balasRL.h"
#include <stdlib.h>
#include <math.h>

Bala *criar_bala(float l, float c, float velL, float velC, int dano) {
    Bala *b = malloc(sizeof(Bala));
    if (!b) return NULL;
    b->linha = l;
    b->coluna = c;
    b->velLinha = velL;
    b->velColuna = velC;
    b->dano = dano;
    b->proxima = NULL;
    return b;
}

void inicio_lista_balas(ListaBalas *lista) {
    if (!lista) return;
    lista->head = NULL;
    lista->quantidade = 0;
}

void adicionar_bala(ListaBalas *lista, Bala *b) {
    if (!lista || !b) return;
    b->proxima = lista->head;
    lista->head = b;
    lista->quantidade++;
}

void remover_bala(ListaBalas *lista, Bala *alvo) {
    if (!lista || !alvo) return;
    Bala *atual = lista->head, *anterior = NULL;
    while (atual) {
        if (atual == alvo) {
            if (anterior) anterior->proxima = atual->proxima;
            else lista->head = atual->proxima;
            free(atual);
            lista->quantidade--;
            return;
        }
        anterior = atual;
        atual = atual->proxima;
    }
}

static inline char mapa_get_safe(Mapa *mapa, int l, int c) {
    return mapa->celulas[l * mapa->colunas + c];
}

void atualizar_balas(ListaBalas *lista, Mapa *mapa) {
    if (!lista || !mapa) return;
    Bala *atual = lista->head;
    while (atual) {
        Bala *prox = atual->proxima;

        float novaL = atual->linha + atual->velLinha;
        float novaC = atual->coluna + atual->velColuna;

        int lin = (int)floorf(novaL);
        int col = (int)floorf(novaC);

        // Fora do mapa -> remover
        if (lin < 0 || col < 0 || lin >= mapa->linhas || col >= mapa->colunas) {
            remover_bala(lista, atual);
            atual = prox;
            continue;
        }

        // Colisões com parede/abrigo -> remover
        char cel = mapa_get_safe(mapa, lin, col);
        if (cel == mapa->parede || cel == mapa->abrigo) {
            remover_bala(lista, atual);
            atual = prox;
            continue;
        }

        // Movimento permitido
        atual->linha = novaL;
        atual->coluna = novaC;

        atual = prox;
    }
}

void desenhar_balas(ListaBalas *lista, int tamanho_celula) {
    if (!lista) return;
    Bala *b = lista->head;
    while (b) {
        float x = b->coluna * tamanho_celula + tamanho_celula / 2.0f;
        float y = b->linha * tamanho_celula + tamanho_celula / 2.0f;
        float raio = tamanho_celula / 3.0f;
        DrawCircleV((Vector2){ x, y }, raio, RED);
        b = b->proxima;
    }
}

void liberar_lista_balas(ListaBalas *lista) {
    if (!lista) return;
    Bala *b = lista->head;
    while (b) {
        Bala *next = b->proxima;
        free(b);
        b = next;
    }
    lista->head = NULL;
    lista->quantidade = 0;
}


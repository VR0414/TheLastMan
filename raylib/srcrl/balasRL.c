#include "balasRL.h"
#include "raylib.h"
#include <stdlib.h>

Bala *criar_bala(float l, float c, float velL, float velC, int tempo) {
    Bala *b = (Bala *)malloc(sizeof(Bala));
    b->linha = l;
    b->coluna = c;
    b->velLinha = velL;
    b->velColuna = velC;
    b->contador_tempo = tempo;
    b->proxima = NULL;
    return b;
}

void inicio_lista_balas(ListaBalas *lista) {
    lista->head = NULL;
    lista->quantidade_balas = 0;
}

void inserir_bala_inicio(ListaBalas *lista, Bala *bala) {
    if (!lista || !bala) return;
    bala->proxima = lista->head;
    lista->head = bala;
    lista->quantidade_balas++;
}

void remover_balas_lista(ListaBalas *lista, Bala *alvo) {
    if (!lista || !alvo) return;
    Bala *atual = lista->head, *anterior = NULL;

    while (atual) {
        if (atual == alvo) {
            if (anterior)
                anterior->proxima = atual->proxima;
            else
                lista->head = atual->proxima;
            free(atual);
            lista->quantidade_balas--;
            return;
        }
        anterior = atual;
        atual = atual->proxima;
    }
}

void atualizar_balas(ListaBalas *lista, Mapa *mapa, char parede, char abrigo) {
    Bala *atual = lista->head;
    while (atual) {
        Bala *prox = atual->proxima;

        if (--atual->contador_tempo <= 0) {
            remover_balas_lista(lista, atual);
            atual = prox;
            continue;
        }

        float novaL = atual->linha + atual->velLinha;
        float novaC = atual->coluna + atual->velColuna;
        int lin = (int)novaL, col = (int)novaC;

        if (lin < 0 || col < 0 || lin >= mapa->linhas || col >= mapa->colunas ||
            mapa->celulas[lin * mapa->colunas + col] == parede ||
            mapa->celulas[lin * mapa->colunas + col] == abrigo) {
            remover_balas_lista(lista, atual);
        } else {
            atual->linha = novaL;
            atual->coluna = novaC;
        }

        atual = prox;
    }
}

void desenhar_balas(ListaBalas *lista, int tamanho_celula) {
    Bala *b = lista->head;
    while (b) {
        DrawCircle(
            (int)(b->coluna * tamanho_celula + tamanho_celula / 2),
            (int)(b->linha * tamanho_celula + tamanho_celula / 2),
            tamanho_celula / 3,
            RED
        );
        b = b->proxima;
    }
}

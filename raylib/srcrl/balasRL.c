#include "balasRL.h"
#include "raylib.h"
#include <stdlib.h>

Bala *criar_bala(float l, float c, float velL, float velC, int tempo) {
    Bala *b = malloc(sizeof(Bala));
    if (!b) return NULL;  

    b->linha = l;
    b->coluna = c;
    b->velLinha = velL;
    b->velColuna = velC;
    b->contador_tempo = tempo;
    b->proxima = NULL;
    return b;
}


void inicio_lista_balas(ListaBalas *lista) {
    if (!lista) return;
    lista->head = NULL;
    lista->quantidade_balas = 0;
}

// Insere uma bala no início da lista de balas.
// Parâmetros:
//   lista - ponteiro para a lista de balas onde a bala será inserida
//   bala  - ponteiro para a bala a ser inserida
void inserir_bala_inicio(ListaBalas *lista, Bala *bala) {
    if (!lista || !bala) return;

    bala->proxima = lista->head;
    lista->head = bala;
    lista->quantidade_balas++;
}

//void inserir_bala_inicio(ListaBalas *lista, Bala *nova) {
//    if (!lista || !nova) return;      FIALHO TESTAR
//    nova->proxima = lista->head;
//    lista->head = nova;
//    lista->quantidade_balas++;
//}


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

// Atualiza a posição das balas na lista, removendo aquelas que colidem com paredes, abrigos ou saem do mapa.
// Parâmetros:
//   lista   - ponteiro para a lista de balas a ser atualizada
//   mapa    - ponteiro para o mapa do jogo
//   parede  - caractere representando uma parede no mapa
//   abrigo  - caractere representando um abrigo no mapa
void atualizar_balas(ListaBalas *lista, Mapa *mapa, char parede, char abrigo) {
    if (!lista || !mapa || !mapa->celulas) return;
    
    Bala *atual = lista->head;
    while (atual) {
        Bala *prox = atual->proxima;

        if (atual->contador_tempo <= 1) {
            remover_balas_lista(lista, atual);
            atual = prox;
            continue;
        } else {
            atual->contador_tempo--;
        }

        float novaL = atual->linha + atual->velLinha;
        float novaC = atual->coluna + atual->velColuna;
        int lin = (int)novaL, col = (int)novaC;

        if (lin < 0 || col < 0 || lin >= mapa->linhas || col >= mapa->colunas ||
            mapa->celulas[lin * mapa->colunas + col] == parede ||
            mapa->celulas[lin * mapa->colunas + col] == abrigo) {
            remover_balas_lista(lista, atual);
            atual = prox;
            continue;
        } else {
            atual->linha = novaL;
            atual->coluna = novaC;
        }

        atual = prox;
    }
}

// Desenha todas as balas presentes na lista na tela.
// Parâmetros:
//   lista          - ponteiro para a lista de balas a serem desenhadas
//   tamanho_celula - tamanho de cada célula no grid do jogo (usado para posicionamento)
void desenhar_balas(ListaBalas *lista, int tamanho_celula) {
    if (!lista) return;  

    Bala *b = lista->head;
    while (b) {
        DrawCircle(
            (b->coluna * tamanho_celula + tamanho_celula / 2),
            (b->linha * tamanho_celula + tamanho_celula / 2),
            tamanho_celula / 3,
            RED
        );
        b = b->proxima;
    }
}
        


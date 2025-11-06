#include <stdlib.h>
#include <stdio.h>
#include "balasREAL.h"  

Bala *criar_bala(float l, float c, float velL, float velC, int tempo) {

    Bala *bala = (Bala *)malloc(sizeof(Bala));

    bala->coluna = c;
    bala->linha = l;
    bala->velColuna = velC;
    bala->velLinha = velL;
    bala->contador_tempo = tempo;
    bala->proxima = NULL;

    return bala;

}

void inicio_lista_balas(ListaBalas *lista) {

    lista->head = NULL;
    lista->quantidade_balas = 0;

}

void inserir_bala_inicio(ListaBalas *lista, Bala *bala) { // "*lista" ponteiro para o início da lista encadeada de balas

    if (lista == NULL || bala == NULL)
        return;

    if (lista->head == NULL) {
        lista->head = bala;
    } else {
        bala->proxima = lista->head;
        lista->head = bala;
    }

    lista->quantidade_balas++;

}

void remover_balas_lista(ListaBalas *lista, Bala *bala_alvo) {

    if (lista == NULL || lista->head == NULL || bala_alvo == NULL)
        return;

    Bala *atual = lista->head;
    Bala *anterior = NULL;

    while (atual != NULL) {
        if (atual == bala_alvo) {
            if (anterior == NULL) { // Compara se for a primeira bala da lista a ser removida
                lista->head = atual->proxima;
            } else {
                anterior->proxima = atual->proxima; // Atualiza a bala anterior, apontando para duas a sua frente
            }

            free(atual);
            lista->quantidade_balas--;
            return; 
        }
        anterior = atual; // Incrementa se não ainda não houver colisão
        atual = atual->proxima;
    }
}

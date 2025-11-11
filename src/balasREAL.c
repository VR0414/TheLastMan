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

void liberar_lista_balas(ListaBalas *lista) {
    
    Bala *atual = lista->head;
    while (atual) {
        Bala *proxima = atual->proxima;
        free(atual);
        atual = proxima;
    }
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

void atualizar_balas(ListaBalas *lista, Mapa *mapa, char parede, char abrigo) {

    Bala *anterior = NULL;
    Bala *atual = lista->head;

    while (atual != NULL) {
        Bala *proxima = atual->proxima; 

        if (atual->contador_tempo > 0) { // verifica tempo de vida da bala após movimento
            atual->contador_tempo--;
            if (atual->contador_tempo == 0) { // Tempo acabou → remover a bala
                if (anterior)
                    anterior->proxima = proxima;
                else
                    lista->head = proxima;

                free(atual);
                lista->quantidade_balas--;
                atual = proxima;
                continue;
            }
        }

        // Cálculo de nova posição 
        float nova_linha = atual->linha + atual->velLinha;
        float nova_coluna = atual->coluna + atual->velColuna;

        int lin = (int)nova_linha;
        int col = (int)nova_coluna;

        if (lin < 0 || lin >= mapa->linhas || col < 0 || col >= mapa->colunas) {
            if (anterior)
                anterior->proxima = proxima;
            else
                lista->head = proxima;

            free(atual);
            lista->quantidade_balas--;
            atual = proxima;
            continue;
        }

        // Verifica colisão com parede ou abrigo
        char celula = mapa->celulas[lin * mapa->colunas + col];
        if (celula == parede || celula == abrigo) {
            if (anterior)
                anterior->proxima = proxima;
            else
                lista->head = proxima;

            free(atual);
            lista->quantidade_balas--;
            atual = proxima;
            continue;
        }

        atual->linha = nova_linha;
        atual->coluna = nova_coluna;

        anterior = atual;
        atual = proxima;
    }
}

int disparar_bala(ListaBalas *lista, float l, float c, float velL, float velC, int tempo) {
    
    Bala *b = criar_bala(l, c, velL, velC, tempo);
    if (!b) return -1;
    inserir_bala_inicio(lista, b);
    return 0;
    
}

void imprimir_mapa_com_balas(Mapa *mapa, ListaBalas *lista, char simbolo_bala) {

    for (int l = 0; l < mapa->linhas; l++) {
        for (int c = 0; c < mapa->colunas; c++) {
            char tiro = mapa->celulas[l * mapa->colunas + c];
            int desenhou_bala = 0;

            // Verifica se alguma bala está nessa posição
            Bala *bala = lista ? lista->head : NULL; // Ternário para verificar bala
            while (bala != NULL) {
                int balaL = (int)bala->linha;
                int balaC = (int)bala->coluna;

                if (balaL == l && balaC == c) {
                    putchar(simbolo_bala);
                    desenhou_bala = 1;
                    break;
                }
                bala = bala->proxima;
            }

            if (desenhou_bala == 0)
                putchar(tiro);
        }
        putchar('\n');
    }
}


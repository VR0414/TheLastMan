#include <stdlib.h>
#include "bala.h"
#include "mapa.Th"

// lista de balas (simples linked list)
ListaBalas *criar_lista_balas(){
    ListaBalas *l = malloc(sizeof(ListaBalas));
    if(!l) return NULL;
    l->inicio = NULL;
    return l;
}

void liberar_lista_balas(ListaBalas *l){
    if(!l) return;
    Bala *p = l->inicio;
    while(p){
        Bala *t = p;
        p = p->proximo;
        free(t);
    }
    free(l);
}

void adicionar_bala(ListaBalas *l, int linha, int coluna, int dx){
    if(!l) return;
    Bala *b = malloc(sizeof(Bala));
    if(!b) return;
    b->linha = linha;
    b->coluna = coluna;
    b->dx = dx;
    b->proximo = l->inicio;
    l->inicio = b;
}

void desenhar_balas_em_mapa(ListaBalas *l, void *mapa_void){
    Mapa *m = (Mapa*)mapa_void;
    if(!m || !l) return;
    Bala *p = l->inicio;
    while(p){
        if(p->linha >= 0 && p->linha < m->linhas && p->coluna >=0 && p->coluna < m->colunas)
            definir_celula(m, p->linha, p->coluna, '^');
        p = p->proximo;
    }
}

// move as balas horizontalmente (coluna += dx) — retorna colisao 1 se jogador foi atingido (pos px,py)
void atualizar_balas(ListaBalas *l, void *mapa_void, int *colisao, int px, int py){
    Mapa *m = (Mapa*)mapa_void;
    if(!m || !l) return;
    *colisao = 0;

    // limpa todas as balas do mapa (vamos redesenhar)
    Bala *p = l->inicio;
    while(p){
        if(p->linha >=0 && p->linha < m->linhas && p->coluna >=0 && p->coluna < m->colunas)
            definir_celula(m, p->linha, p->coluna, ' ');
        p = p->proximo;
    }

    // atualizar posições: mover horizontalmente; remover se saiu das colunas
    Bala *ant = NULL;
    p = l->inicio;
    while(p){
        p->coluna += p->dx; // move
        Bala *next = p->proximo;

        // se saiu das colunas -> remover
        if(p->coluna < 0 || p->coluna >= m->colunas){
            if(ant) ant->proximo = next;
            else l->inicio = next;
            free(p);
            p = next;
            continue;
        }

        // se atingiu jogador
        if(p->linha == px && p->coluna == py){
            *colisao = 1;
            // não removemos imediatamente; será redesenhado e o jogo detecta colisão
        }

        ant = p;
        p = next;
    }

    // redesenhar balas
    desenhar_balas_em_mapa(l, m);
}

// ===================================================
// ARQUIVO: balasRL.c (Com Hitbox Ativado)
// ===================================================
#include "balasRL.h"
#include <stdlib.h> 

Bala *criar_bala(Vector2 pos, Vector2 vel, int dano, float width, float height) {
    Bala *b = malloc(sizeof(Bala));
    if (!b) return NULL;
    
    b->posicao = pos;
    b->velocidade = vel;
    b->dano = dano;
    b->colisao = (Rectangle){ pos.x, pos.y, width, height };
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

void atualizar_balas(ListaBalas *lista, int screenWidth, int screenHeight) {
    if (!lista) return;
    
    float dt = GetFrameTime(); 
    
    Bala *atual = lista->head;
    while (atual) {
        Bala *prox = atual->proxima;

        atual->posicao.x += atual->velocidade.x * dt;
        atual->posicao.y += atual->velocidade.y * dt;

        atual->colisao.x = atual->posicao.x;
        atual->colisao.y = atual->posicao.y;

        if (atual->posicao.x < -100 ||
            atual->posicao.x > screenWidth + 100 ||
            atual->posicao.y < -100 ||
            atual->posicao.y > screenHeight + 100)
        {
            remover_bala(lista, atual);
            atual = prox;
            continue;
        }

        atual = prox;
    }
}

void desenhar_balas(ListaBalas *lista, Texture2D texturaBala) {
    if (!lista) return;
    
    Bala *b = lista->head;
    while (b) {
        DrawTextureV(texturaBala, b->posicao, WHITE);
        
        // --- HITBOX LIGADO PARA DIAGNÓSTICO ---
        DrawRectangleLinesEx(b->colisao, 1, RED);
        
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
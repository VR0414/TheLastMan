// ===================================================
// ARQUIVO: balasRL.h (NOVO)
// ===================================================
#ifndef BALAS_RL_H
#define BALAS_RL_H

#include "raylib.h" // A BASE AGORA É RAYLIB

// ----------------------------------------------------
// STRUCT DA BALA (Versão Raylib)
// ----------------------------------------------------
typedef struct Bala {
    Vector2 posicao;    // Posição x, y em pixels
    Vector2 velocidade; // Velocidade x, y em pixels/segundo
    int dano;
    Rectangle colisao;  // Hitbox da bala
    struct Bala *proxima;
} Bala;

// ----------------------------------------------------
// LISTA DE BALAS (Não muda)
// ----------------------------------------------------
typedef struct {
    Bala *head;
    int quantidade;
} ListaBalas;

// ----------------------------------------------------
// FUNÇÕES PRINCIPAIS (Assinaturas atualizadas)
// ----------------------------------------------------

// Agora recebe Vector2 e as dimensões da bala para o hitbox
Bala *criar_bala(Vector2 pos, Vector2 vel, int dano, float width, float height);

void inicio_lista_balas(ListaBalas *lista);
void adicionar_bala(ListaBalas *lista, Bala *b);
void remover_bala(ListaBalas *lista, Bala *alvo); // Não muda

// Agora recebe as dimensões da tela para saber quando sair
void atualizar_balas(ListaBalas *lista, int screenWidth, int screenHeight);

// Agora recebe a Textura da bala para desenhar
void desenhar_balas(ListaBalas *lista, Texture2D texturaBala);

// Não muda
void liberar_lista_balas(ListaBalas *lista);

#endif

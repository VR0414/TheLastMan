// ===================================================
// ARQUIVO: soldadoRL.h (NOVO)
// ===================================================
#ifndef SOLDADO_RL_H
#define SOLDADO_RL_H

#include "raylib.h"

// ----------------------------------------------------
// STRUCT DO SOLDADO (Versão Raylib)
// ----------------------------------------------------
typedef struct Soldado {
    Vector2 posicao;     // Posição x, y em pixels
    Vector2 posInicial;  // Posição para onde resetar
    int vida;
    float velocidade;    // Velocidade de movimento (pixels/segundo)
    Texture2D textura;   // Imagem do soldado
    Rectangle colisao;   // Hitbox do soldado
} Soldado;

// ----------------------------------------------------
// FUNÇÕES PRINCIPAIS (Assinaturas atualizadas)
// ----------------------------------------------------

// Criação: Carrega a textura e define o hitbox
Soldado *criar_soldado(Vector2 pos, int vida, float vel, float width, float height, const char *texturaPath);

// Liberação: Descarrega a textura
void liberar_soldado(Soldado *s);

// Lógica de update: Processa o input (W,A,S,D) e move o soldado
void atualizar_soldado(Soldado *s, int screenWidth, int screenHeight);

// Desenho: Desenha a textura do soldado
void desenhar_soldado(Soldado *s);

// Reseta o soldado para a posição inicial
void reset_posicao_soldado(Soldado *s);

// Aplica dano e reseta a posição
int aplicar_dano_soldado(Soldado *s, int dano);


#endif

// ===================================================
// ARQUIVO: soldadoRL.c (Com Hitbox Ativado)
// ===================================================
#include "soldadoRL.h"
#include <stdlib.h>

Soldado *criar_soldado(Vector2 pos, int vida, float vel, float width, float height, const char *texturaPath) {
    Soldado *s = malloc(sizeof(Soldado));
    if (!s) return NULL;
    
    s->posicao = pos;
    s->posInicial = pos;
    s->vida = vida;
    s->velocidade = vel;
    s->textura = LoadTexture(texturaPath);
    s->colisao = (Rectangle){ pos.x, pos.y, width, height };
    
    return s;
}

void liberar_soldado(Soldado *s) {
    if (s) {
        UnloadTexture(s->textura);
        free(s);
    }
}

void atualizar_soldado(Soldado *s, int screenWidth, int screenHeight) {
    if (!s) return;

    float dt = GetFrameTime();
    
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        s->posicao.y -= s->velocidade * dt;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        s->posicao.y += s->velocidade * dt;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        s->posicao.x -= s->velocidade * dt;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        s->posicao.x += s->velocidade * dt;
    }
    
    if (s->posicao.x < 0) s->posicao.x = 0;
    if (s->posicao.x > screenWidth - s->colisao.width) s->posicao.x = screenWidth - s->colisao.width;
    if (s->posicao.y < 0) s->posicao.y = 0;
    if (s->posicao.y > screenHeight - s->colisao.height) s->posicao.y = screenHeight - s->colisao.height;

    s->colisao.x = s->posicao.x;
    s->colisao.y = s->posicao.y;
}

void desenhar_soldado(Soldado *s) {
    if (!s) return;
    
    DrawTextureV(s->textura, s->posicao, WHITE);
    
    // --- HITBOX LIGADO PARA DIAGNÓSTICO ---
    DrawRectangleLinesEx(s->colisao, 2, GREEN);
}

void reset_posicao_soldado(Soldado *s) {
    if (!s) return;
    s->posicao = s->posInicial;
    s->colisao.x = s->posicao.x;
    s->colisao.y = s->posicao.y;
}

int aplicar_dano_soldado(Soldado *s, int dano) {
    if (!s) return 0;
    
    s->vida -= dano;
    if (s->vida < 0) s->vida = 0;
    
    reset_posicao_soldado(s);
    
    return s->vida;
}
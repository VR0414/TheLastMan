// ===================================================
// ARQUIVO: soldadoRL.c (NOVO)
// ===================================================
#include "soldadoRL.h"
#include <stdlib.h> // Para malloc/free

// --- criar_soldado ---
// Carrega a textura e prepara o soldado
Soldado *criar_soldado(Vector2 pos, int vida, float vel, float width, float height, const char *texturaPath) {
    Soldado *s = malloc(sizeof(Soldado));
    if (!s) return NULL;
    
    s->posicao = pos;
    s->posInicial = pos; // Guarda a posição inicial para o reset
    s->vida = vida;
    s->velocidade = vel;
    s->textura = LoadTexture(texturaPath); // Carrega a imagem do soldado
    
    // Define o hitbox
    s->colisao = (Rectangle){ pos.x, pos.y, width, height };
    
    return s;
}

// --- liberar_soldado ---
// Libera a memória e descarrega a textura
void liberar_soldado(Soldado *s) {
    if (s) {
        UnloadTexture(s->textura); // Muito importante!
        free(s);
    }
}

// --- atualizar_soldado ---
// Processa o input e move o soldado (independente do mapa)
void atualizar_soldado(Soldado *s, int screenWidth, int screenHeight) {
    if (!s) return;

    // Pega o tempo do frame para movimento suave
    float dt = GetFrameTime();
    
    // --- Movimento ---
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
    
    // --- Limites da Tela (Para o soldado não fugir) ---
    // (Pode ser melhorado com os limites do "campo de batalha")
    if (s->posicao.x < 0) s->posicao.x = 0;
    if (s->posicao.x > screenWidth - s->colisao.width) s->posicao.x = screenWidth - s->colisao.width;
    if (s->posicao.y < 0) s->posicao.y = 0;
    if (s->posicao.y > screenHeight - s->colisao.height) s->posicao.y = screenHeight - s->colisao.height;

    // --- Atualizar Hitbox ---
    // O hitbox sempre segue a posição do soldado
    s->colisao.x = s->posicao.x;
    s->colisao.y = s->posicao.y;
}

// --- desenhar_soldado ---
// Desenha o soldado na tela
void desenhar_soldado(Soldado *s) {
    if (!s) return;
    
    DrawTextureV(s->textura, s->posicao, WHITE);
    
    // (Opcional: descomente para ver o hitbox do soldado)
    // DrawRectangleLinesEx(s->colisao, 2, GREEN);
}

// --- reset_posicao_soldado ---
// Volta para o início
void reset_posicao_soldado(Soldado *s) {
    if (!s) return;
    s->posicao = s->posInicial;
    // Atualiza o hitbox junto
    s->colisao.x = s->posicao.x;
    s->colisao.y = s->posicao.y;
}

// --- aplicar_dano_soldado ---
// Reduz a vida e reseta a posição
int aplicar_dano_soldado(Soldado *s, int dano) {
    if (!s) return 0;
    
    s->vida -= dano;
    if (s->vida < 0) s->vida = 0;
    
    // Reseta a posição após ser atingido
    reset_posicao_soldado(s);
    
    return s->vida;
}

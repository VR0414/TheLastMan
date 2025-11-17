#include "balasRL.h"
#include <stdlib.h> // Para malloc/free

// --- criar_bala ---
// Agora usa Vector2 e recebe as dimensões do hitbox
Bala *criar_bala(Vector2 pos, Vector2 vel, int dano, float width, float height) {
    Bala *b = malloc(sizeof(Bala));
    if (!b) return NULL;
    
    b->posicao = pos;
    b->velocidade = vel;
    b->dano = dano;
    // O hitbox é criado na posição inicial
    b->colisao = (Rectangle){ pos.x, pos.y, width, height };
    b->proxima = NULL;
    
    return b;
}

// --- inicio_lista_balas ---
// (Sem mudanças)
void inicio_lista_balas(ListaBalas *lista) {
    if (!lista) return;
    lista->head = NULL;
    lista->quantidade = 0;
}

// --- adicionar_bala ---
// (Sem mudanças)
void adicionar_bala(ListaBalas *lista, Bala *b) {
    if (!lista || !b) return;
    b->proxima = lista->head;
    lista->head = b;
    lista->quantidade++;
}

// --- remover_bala ---
// (Sem mudanças)
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

// --- atualizar_balas ---
// Lógica 100% pixel. Não precisa mais do Mapa!
// Usa GetFrameTime() para movimento suave e independente de FPS.
void atualizar_balas(ListaBalas *lista, int screenWidth, int screenHeight) {
    if (!lista) return;
    
    // Pega o tempo que o último frame demorou (ex: 0.016s)
    float dt = GetFrameTime(); 
    
    Bala *atual = lista->head;
    while (atual) {
        Bala *prox = atual->proxima; // Guarda o próximo antes de possivelmente remover o atual

        // --- Movimento ---
        // (Posição = Posição + Velocidade * Tempo)
        atual->posicao.x += atual->velocidade.x * dt;
        atual->posicao.y += atual->velocidade.y * dt;

        // --- Atualizar Hitbox ---
        // O hitbox segue a posição da bala
        atual->colisao.x = atual->posicao.x;
        atual->colisao.y = atual->posicao.y;

        // --- Verificação de Limites ---
        // Se a bala saiu da tela (com uma margem), ela é removida.
        // Não checamos mais colisão com "paredes" aqui.
        if (atual->posicao.x < -100 || // Deixe 100 pixels de margem
            atual->posicao.x > screenWidth + 100 ||
            atual->posicao.y < -100 ||
            atual->posicao.y > screenHeight + 100)
        {
            remover_bala(lista, atual);
            atual = prox;
            continue; // Pula para a próxima bala
        }

        atual = prox; // Avança para a próxima bala
    }
}

// --- desenhar_balas ---
// Agora desenha a textura na posição exata da bala.
void desenhar_balas(ListaBalas *lista, Texture2D texturaBala) {
    if (!lista) return;
    
    Bala *b = lista->head;
    while (b) {
        // Desenha a textura na posição (x, y) da bala
        DrawTextureV(texturaBala, b->posicao, WHITE);
        
        // (Opcional: descomente para ver os hitboxes)
        // DrawRectangleLinesEx(b->colisao, 1, RED);
        
        b = b->proxima;
    }
}

// --- liberar_lista_balas ---
// (Sem mudanças)
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

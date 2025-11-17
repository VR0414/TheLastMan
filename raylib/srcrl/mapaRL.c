// ===================================================
// ARQUIVO: mapaRL.c (NOVO)
// ===================================================
#include "mapaRL.h"
#include <stdlib.h> // Para malloc/free

// --- criar_mapa ---
// Carrega a textura e define a área de vitória
Mapa *criar_mapa(const char *texturaFundoPath, Rectangle areaDeVitoria) {
    Mapa *mapa = malloc(sizeof(Mapa));
    if (!mapa) return NULL;

    mapa->texturaFundo = LoadTexture(texturaFundoPath);
    mapa->areaDeVitoria = areaDeVitoria;
    
    // (Opcional: Verifique se a textura carregou)
    // if (mapa->texturaFundo.id <= 0) { ... }

    return mapa;
}

// --- liberar_mapa ---
// Descarrega a textura
void liberar_mapa(Mapa *mapa) {
    if (mapa) {
        UnloadTexture(mapa->texturaFundo); // Muito importante!
        free(mapa);
    }
}

// --- desenhar_mapa ---
// Simplesmente desenha a imagem de fundo na tela
void desenhar_mapa(Mapa *mapa) {
    if (!mapa) return;
    
    // Desenha o fundo na posição (0, 0)
    DrawTexture(mapa->texturaFundo, 0, 0, WHITE);
    
    // (Opcional: descomente para ver a área de vitória)
    // DrawRectangleRec(mapa->areaDeVitoria, Fade(GREEN, 0.5f));
}

// --- checar_vitoria_mapa ---
// Retorna 1 (true) se o soldado está tocando a área de vitória
int checar_vitoria_mapa(Mapa *mapa, Rectangle colisaoSoldado) {
    if (!mapa) return 0;
    
    // Usa a função de colisão da raylib
    if (CheckCollisionRecs(mapa->areaDeVitoria, colisaoSoldado)) {
        return 1;
    }
    
    return 0;
}

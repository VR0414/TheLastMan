// ===================================================
// ARQUIVO: mapaRL.c
// ===================================================
#include "mapaRL.h"
#include <stdlib.h>

Mapa *criar_mapa(const char *texturaFundoPath, Rectangle areaDeVitoria) {
    Mapa *mapa = malloc(sizeof(Mapa));
    if (!mapa) return NULL;

    mapa->texturaFundo = LoadTexture(texturaFundoPath);
    mapa->areaDeVitoria = areaDeVitoria;
    
    return mapa;
}

void liberar_mapa(Mapa *mapa) {
    if (mapa) {
        UnloadTexture(mapa->texturaFundo);
        free(mapa);
    }
}

void desenhar_mapa(Mapa *mapa) {
    if (!mapa) return;
    
    DrawTexture(mapa->texturaFundo, 0, 0, WHITE);
}

int checar_vitoria_mapa(Mapa *mapa, Rectangle colisaoSoldado) {
    if (!mapa) return 0;
    
    if (CheckCollisionRecs(mapa->areaDeVitoria, colisaoSoldado)) {
        return 1;
    }
    
    return 0;
}
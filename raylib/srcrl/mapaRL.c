#include "mapaRL.h"
#include <stdlib.h>

Mapa *criar_mapa(const char *texturaFundoPath, Rectangle areaDeVitoria, int screenW, int screenH) {
    Mapa *mapa = malloc(sizeof(Mapa));
    if (!mapa) return NULL;

    Image img = LoadImage(texturaFundoPath);
    ImageResize(&img, screenW, screenH);
    mapa->texturaFundo = LoadTextureFromImage(img);
    UnloadImage(img);
    
    mapa->areaDeVitoria = areaDeVitoria;
    
    mapa->tileWidth = (float)screenW / MAPA_COLUNAS;
    mapa->tileHeight = (float)screenH / MAPA_LINHAS;

    for (int i = 0; i < MAPA_LINHAS; i++) {
        for (int j = 0; j < MAPA_COLUNAS; j++) {
            if (i == 0) mapa->grade[i][j] = 2; 
            else if (i >= 2 && i <= MAPA_LINHAS - 2) mapa->grade[i][j] = 1; 
            else mapa->grade[i][j] = 0; 
        }
    }
    
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

    DrawRectangleRec(mapa->areaDeVitoria, Fade(DARKGREEN, 0.6f));
    DrawLineEx((Vector2){0, mapa->areaDeVitoria.height}, 
               (Vector2){(float)GetScreenWidth(), mapa->areaDeVitoria.height}, 
               5, YELLOW);
}

int checar_vitoria_mapa(Mapa *mapa, Rectangle colisaoSoldado) {
    if (!mapa) return 0;
    if (CheckCollisionRecs(mapa->areaDeVitoria, colisaoSoldado)) {
        return 1;
    }
    return 0;
}
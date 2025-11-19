#ifndef MAPA_RL_H
#define MAPA_RL_H

#include "raylib.h"

#define MAPA_LINHAS 10
#define MAPA_COLUNAS 16

typedef struct Mapa {
    Texture2D texturaFundo;
    Rectangle areaDeVitoria;
    
    int grade[MAPA_LINHAS][MAPA_COLUNAS]; 
    
    float tileWidth;
    float tileHeight;
} Mapa;

Mapa *criar_mapa(const char *texturaFundoPath, Rectangle areaDeVitoria, int screenW, int screenH);
void liberar_mapa(Mapa *mapa);
void desenhar_mapa(Mapa *mapa);
int checar_vitoria_mapa(Mapa *mapa, Rectangle colisaoSoldado);

#endif
// ===================================================
// ARQUIVO: mapaRL.h
// ===================================================
#ifndef MAPA_RL_H
#define MAPA_RL_H

#include "raylib.h"

typedef struct Mapa {
    Texture2D texturaFundo;
    Rectangle areaDeVitoria;
} Mapa;

Mapa *criar_mapa(const char *texturaFundoPath, Rectangle areaDeVitoria);
void liberar_mapa(Mapa *mapa);
void desenhar_mapa(Mapa *mapa);
int checar_vitoria_mapa(Mapa *mapa, Rectangle colisaoSoldado);

#endif
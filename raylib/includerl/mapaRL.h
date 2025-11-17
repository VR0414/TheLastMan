// ===================================================
// ARQUIVO: mapaRL.h (NOVO)
// ===================================================
#ifndef MAPA_RL_H
#define MAPA_RL_H

#include "raylib.h"

// ----------------------------------------------------
// STRUCT DO MAPA (Versão Raylib)
// ----------------------------------------------------
typedef struct Mapa {
    Texture2D texturaFundo; // A imagem do "campo de batalha"
    Rectangle areaDeVitoria; // O "abrigo seguro" no topo
} Mapa;

// ----------------------------------------------------
// FUNÇÕES PRINCIPAIS (Assinaturas atualizadas)
// ----------------------------------------------------

// Criação: Carrega a imagem de fundo e define a zona de vitória
Mapa *criar_mapa(const char *texturaFundoPath, Rectangle areaDeVitoria);

// Liberação: Descarrega a textura
void liberar_mapa(Mapa *mapa);

// Desenho: Apenas desenha a imagem de fundo
void desenhar_mapa(Mapa *mapa);

// Checagem: Verifica se o soldado chegou no abrigo
int checar_vitoria_mapa(Mapa *mapa, Rectangle colisaoSoldado);

#endif

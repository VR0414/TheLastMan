#ifndef SOLDADO_RL_H
#define SOLDADO_RL_H

#include "raylib.h"

typedef struct Soldado {
    Vector2 posicao;
    Vector2 posInicial;
    int vida;
    float velocidade;
    Texture2D textura;
    Rectangle colisao;
} Soldado;

Soldado *criar_soldado(Vector2 pos, int vida, float vel, float width, float height, const char *texturaPath);
void liberar_soldado(Soldado *s);
void atualizar_soldado(Soldado *s, int screenWidth, int screenHeight);
void desenhar_soldado(Soldado *s);
void reset_posicao_soldado(Soldado *s);
int aplicar_dano_soldado(Soldado *s, int dano);

#endif

#include <stdio.h>
#include <stdlib.h>
#include "soldadoRL.h"

// -------------------------
// CRIA SOLDADO
// -------------------------
Soldado *criar_soldado(float linha, float coluna, int vida, char simbolo) {

    Soldado *s = (Soldado *)malloc(sizeof(Soldado));
    if (s == NULL)
        return NULL;

    s->pos.x = coluna;   // Vector2: x = coluna
    s->pos.y = linha;    // Vector2: y = linha
    s->vida = vida;
    s->simbolo = simbolo;

    return s;
}

// -------------------------
// LIBERA MEMÓRIA
// -------------------------
void liberar_soldado(Soldado *s) {

    if (s == NULL)
        return;

    free(s);
}

// -------------------------
// INICIA SOLDADO EXISTENTE
// -------------------------
void iniciar_soldado(Soldado *s, float linha, float coluna, int vida, char simbolo) {

    s->pos.x = coluna;
    s->pos.y = linha;
    s->vida = vida;
    s->simbolo = simbolo;
}

// -------------------------
// MOVIMENTO DO SOLDADO
// -------------------------
int mover_soldado(Soldado *s, Mapa *mapa, float deslocLinha, float deslocColuna) {

    float nova_linha  = s->pos.y + deslocLinha;
    float nova_coluna = s->pos.x + deslocColuna;

    int intLinha = (int)nova_linha;
    int intColuna = (int)nova_coluna;

    // Fora dos limites do mapa
    if (intLinha < 0 || intLinha >= mapa->linhas || intColuna < 0 || intColuna >= mapa->colunas)
        return 0;

    char destino = mapa->celulas[intLinha * mapa->colunas + intColuna];

    // Permite mover para abrigo
    if (destino == mapa->abrigo) {
        s->pos.x = nova_coluna;
        s->pos.y = nova_linha;
        return 1;
    }

    // Bloqueia parede / obstáculo
    if (destino != ' ')
        return 0;

    // Move normal
    s->pos.x = nova_coluna;
    s->pos.y = nova_linha;
    return 1;
}

// -------------------------
// RESETAR POSIÇÃO
// -------------------------
int reset_posicao_soldado(Soldado *s, float linha, float coluna) {
    s->pos.x = coluna;
    s->pos.y = linha;
    return 1;
}

// -------------------------
// DANO NO SOLDADO
// -------------------------
int dano_soldado(Soldado *s, int dano, float inicioLinha, float inicioColuna) {

    s->vida -= dano;

    if (s->vida <= 0) {
        s->vida = 0;
        return 0;   // morreu
    }

    // Se levou dano mas não morreu → volta pro inicio
    reset_posicao_soldado(s, inicioLinha, inicioColuna);

    return s->vida;
}

// -------------------------
// DESENHAR SOLDADO NO RAYLIB
// -------------------------
void desenhar_soldado(Soldado *s, int tamanho_celula) {

    DrawRectangle(
        s->pos.x * tamanho_celula,
        s->pos.y * tamanho_celula,
        tamanho_celula,
        tamanho_celula,
        RED
    );

    // opcional: desenhar o símbolo dentro
    DrawText(
        TextFormat("%c", s->simbolo),
        s->pos.x * tamanho_celula + 4,
        s->pos.y * tamanho_celula + 2,
        tamanho_celula - 4,
        WHITE
    );
}

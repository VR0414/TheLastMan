#include "raylib.h"
#include "mapaRL.h"
#include "balasRL.h"
#include "soldadoRL.h"
#include <stdlib.h>
#include <time.h>

// Tamanho do mapa no Raylib
#define CELULA 20

int main(void) {

    InitWindow(800, 600, "THE LAST MAN - Frogger Edition");
    SetTargetFPS(60);

    // ----------------------------
    // MAPA
    // ----------------------------
    Mapa *mapa = criar_mapa(MAPA_LINHAS, MAPA_COLUNAS, '=');
    preencher_limites(mapa, '#');
    preencher_chao(mapa, '.', '#', '=');
    preencher_abrigo(mapa, '=');

    // ----------------------------
    // SOLDADO (jogador)
    // ----------------------------
    Soldado jogador;
    iniciar_soldado(&jogador, mapa->linhas - 3, mapa->colunas / 2, 3, 'S');

    float spawnInicialL = jogador.linha;
    float spawnInicialC = jogador.coluna;

    // ----------------------------
    // LISTA DE BALAS (carros)
    // ----------------------------
    ListaBalas lista;
    inicio_lista_balas(&lista);

    srand(time(NULL));

    int pontos = 0;
    float tempoSpawn = 0;

    while (!WindowShouldClose()) {

        // ---------------------------------------------------------
        // 1. CONTROLE DO JOGADOR
        // ---------------------------------------------------------
        if (IsKeyDown(KEY_UP))    mover_soldado(&jogador, mapa, -0.1f, 0);
        if (IsKeyDown(KEY_DOWN))  mover_soldado(&jogador, mapa,  0.1f, 0);
        if (IsKeyDown(KEY_LEFT))  mover_soldado(&jogador, mapa, 0, -0.1f);
        if (IsKeyDown(KEY_RIGHT)) mover_soldado(&jogador, mapa, 0,  0.1f);

        // ---------------------------------------------------------
        // 2. GERAR BALAS AUTOMATICAMENTE (carros)
        // ---------------------------------------------------------
        tempoSpawn += GetFrameTime();

        if (tempoSpawn > 0.8f) {     // A cada 0.8s gera uma bala
            tempoSpawn = 0;

            int linha = 1 + rand() % (mapa->linhas - 4);

            Bala *b = criar_bala(linha, mapa->colunas - 2,
                                 0, -(0.15f + (rand() % 15) / 100.0f), 300);

            inserir_bala_inicio(&lista, b);
        }

        // ---------------------------------------------------------
        // 3. ATUALIZAR BALAS
        // ---------------------------------------------------------
        atualizar_balas(&lista, mapa, '#', '=');

        // ---------------------------------------------------------
        // 4. VERIFICAR COLISÃO BALAS x SOLDADO
        // ---------------------------------------------------------
        Bala *b = lista.head;
        while (b) {

            if ((int)b->linha == (int)jogador.linha &&
                (int)b->coluna == (int)jogador.coluna) {

                aplicar_dano_soldado(&jogador, 1, spawnInicialL, spawnInicialC);
                remover_balas_lista(&lista, b);
                break;
            }

            b = b->proxima;
        }

        // ---------------------------------------------------------
        // 5. META → chegou no topo?
        // ---------------------------------------------------------
        if ((int)jogador.linha <= 1) {
            pontos++;
            reset_posicao_soldado(&jogador, spawnInicialL, spawnInicialC);
        }

        // ---------------------------------------------------------
        // 6. DESENHO
        // ---------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // mapa
        desenhar_mapa(mapa, CELULA);

        // balas
        desenhar_balas(&lista, CELULA);

        // jogador
        DrawCircle(
            (int)(jogador.coluna * CELULA + CELULA / 2),
            (int)(jogador.linha * CELULA + CELULA / 2),
            CELULA / 2,
            BLUE
        );

        DrawText(TextFormat("VIDA: %d", jogador.vida), 10, 10, 20, WHITE);
        DrawText(TextFormat("PONTOS: %d", pontos), 10, 35, 20, WHITE);

        EndDrawing();
    }

    liberar_mapa(mapa);
    CloseWindow();
    return 0;
}
